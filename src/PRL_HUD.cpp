#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <thread>
#include <functional>

#include "PRL_math.h"
#include "PRL_HUD.h"
#include "PRL_types.h"
#include "PRL_collision.h"
#include "PRL_input.h"
#include "PRL_global_variables.h"

using std::cout;
using std::cerr;
using std::endl;

SDL_Texture* PRL_CreateRectangle(SDL_Renderer *renderer, int w, int h, SDL_Color const& bkgd, int borderSz, SDL_Color const& border)
{
    if (borderSz > PRL_HUD_MAX_BORDER_SIZE) borderSz = PRL_HUD_MAX_BORDER_SIZE; // maximum limitation
    else if (borderSz < 0) borderSz = 0; // minimum limitation
    if (w - 2*borderSz < 0 || h - 2*borderSz < 0) borderSz = 0; // check whether the border size creates a negative value for the second texture "texture2"
    if (strcmp(SDL_GetError(), "Invalid Texture") == 0) // from documentation, error message when the texture is not valid
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_CreateRectangle, Invalid texture" << endl;
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_Texture *texture2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);

    if (texture == NULL || texture2 == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_CreateRectangle, " << SDL_GetError() << endl;
        return NULL;
    }

    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0 || SDL_SetTextureBlendMode(texture2, SDL_BLENDMODE_BLEND) != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_CreateRectangle, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    Uint8 r, g, b, a; // save previous renderer settings
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    SDL_Rect rect;

    rect = {borderSz, borderSz, w - 2*borderSz, h - 2*borderSz}; // first the background
    SDL_SetRenderTarget(renderer, texture2); // prepare the background (bkgd) color
    SDL_SetRenderDrawColor(renderer, bkgd.r, bkgd.g, bkgd.b, bkgd.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture2, NULL, &rect);

    SDL_SetRenderTarget(renderer, texture2); // now the borders
    SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    rect = {0, 0, w - borderSz, borderSz}; // top bar
    SDL_RenderCopy(renderer, texture2, NULL, &rect);

    rect = {w - borderSz, 0, borderSz, h - borderSz}; // right bar
    SDL_RenderCopy(renderer, texture2, NULL, &rect);

    rect = {borderSz, h - borderSz, w - borderSz, borderSz}; // bottom bar
    SDL_RenderCopy(renderer, texture2, NULL, &rect);

    rect = {0, borderSz, borderSz, h - borderSz}; // left bar
    SDL_RenderCopy(renderer, texture2, NULL, &rect);


    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    SDL_DestroyTexture(texture2);

    return texture;
}

SDL_Texture* PRL_CreateRectangle(SDL_Renderer *renderer, int w, int h, SDL_Color const& bkgd)
{
    return PRL_CreateRectangle(renderer, w, h, bkgd, 0, bkgd);
}

/** ********************************************* **/
/**               PRL_TextLabel                   **/
/** ********************************************* **/

PRL_TextLabel :: PRL_TextLabel() : input(NULL), font(NULL), fontColor({255, 255, 255, 255}), texture(NULL), bkgdtarget(NULL), surface(NULL),
path(NULL), fontSz(0), fontStyle(TTF_STYLE_NORMAL), useBkgd(false), allowDisplacement_(false), beingMoved(false), relativeClickDistance({0}),
bkgdColor({0}), borderColor({0}), bkgdTempColor({0}), borderSz(0), spaceBetweenBorderAndText(PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT),
renderingMethod(PRL_TEXTRENDER_BLENDED), referenceRenderer({config_GLOBAL.renderResolution.x, config_GLOBAL.renderResolution.y})
{
    text = "";
    dspSrc.x = dspSrc.y = 0;
    updateDisplayRect();
    #if PRL_MULTITHREADING == 1
    isFirstRenderingDone = false;
    #endif // PRL_MULTITHREADING
}

PRL_TextLabel :: PRL_TextLabel(std::string txt)
{
    PRL_TextLabel();
    text = txt;
}

PRL_TextLabel :: PRL_TextLabel(const char *txt)
{
    PRL_TextLabel();
    text = txt;
}

PRL_TextLabel :: ~PRL_TextLabel()
{
    #if PRL_MULTITHREADING == 1
    t1.join();
    #endif // PRL_MULTITHREADING
    if (font != NULL) TTF_CloseFont(font);
    if (path != NULL) free(path);
    if (surface != NULL) SDL_FreeSurface(surface);
    if (texture != NULL) SDL_DestroyTexture(texture);
    if (bkgdtarget != NULL) SDL_DestroyTexture(bkgdtarget);
}

int PRL_TextLabel :: setFont(const char *fontPath, int fontSize)
{
    setFontSize(fontSize);
    return loadFont(fontPath);
}

int PRL_TextLabel :: loadFont(const char *fontPath)
{
    char *pathtemp = (char*) malloc(strlen(fontPath) * sizeof(char));

    if (pathtemp == NULL)
    {
        return PRL_ERROR;
    }

    if (path != NULL) free(path);

    path = pathtemp;
    strcpy(path, fontPath);

    if (font != NULL)
    {
        TTF_CloseFont(font);
        font = NULL;
    }

    font = TTF_OpenFont(path, fontSz*config_GLOBAL.renderResolution.y/referenceRenderer.y); // take account of the scaling

    if (font == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextLabel, while attempting to load the font  " << path << ": " << TTF_GetError() << endl;
        return PRL_ERROR;
    }
    TTF_SetFontStyle(font, fontStyle);
    return 0;
}

void PRL_TextLabel :: setFontSize(int sz)
{
    if (sz < 1)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextLabel, tried to set a font size smaller than 1 (" << sz << ")" << endl;
        #endif
        fontSz = 1;
    }
    else
        fontSz = sz;
}

void PRL_TextLabel :: setInput(PRL_Input *in)
{
    if (in != NULL) input = (PRL_Input*) in;
}

void PRL_TextLabel :: setText(std::string txt)
{
    text = txt;
}

void PRL_TextLabel :: setText(const char* txt)
{
    text = txt;
}

void PRL_TextLabel :: setTextRenderMethod(PRL_TextRenderMethod method)
{
    renderingMethod = method;
}

void PRL_TextLabel :: setTextColor(SDL_Color color)
{
    fontColor = color;
}

void PRL_TextLabel :: setFontStyle(int style)
{
    fontStyle = style;
    if (font != NULL) TTF_SetFontStyle(font, fontStyle);
}

void PRL_TextLabel :: useBackground(SDL_Color bkgd)
{
    useBkgd = true;
    bkgdColor = bkgd;
}

void PRL_TextLabel :: useBackground(SDL_Color bkgd, SDL_Color border, int borderSize)
{
    useBkgd = true;
    bkgdColor = bkgd;
    borderColor = border;
    borderSz = borderSize;
}

void PRL_TextLabel :: stopUsingBackground()
{
    useBkgd = false;
    borderSz = 0;
}

void PRL_TextLabel :: renderText_surfaceManipulation() // split the work between surface manipulation and texture manipulation to allow multi-threading optimization
{
    if (surface != NULL)
    {
        SDL_FreeSurface(surface); surface = NULL;
    }

    char* c_str(NULL); c_str = (char*) malloc ((text.length()+1) * sizeof(char));
    if (text.length() == 0) surface = TTF_RenderText_Solid(font, " ", fontColor);
    else
    {
        strcpy(c_str, text.c_str());
        if (renderingMethod == PRL_TEXTRENDER_SOLID) // solid rendering method
        {
            surface = TTF_RenderUTF8_Solid(font, c_str, fontColor);
        }
        else // blended rendering method
        {
            surface = TTF_RenderUTF8_Blended(font, c_str, fontColor);
        }
    }
    free(c_str);
}

void PRL_TextLabel :: renderText()
{
    #if PRL_MULTITHREADING == 0
    renderText_surfaceManipulation();
    #elif PRL_MULTITHREADING == 1
    if (!isFirstRenderingDone)
    {
        isFirstRenderingDone = true;
        renderText_surfaceManipulation();
    }
    #endif // PRL_MULTITHREADING

    if (texture != NULL)
    {
        SDL_DestroyTexture(texture); texture = NULL;
    }
    //if (surface == NULL) printf("ERROR surface NULL\n");

    texture = SDL_CreateTextureFromSurface(dspRenderer, surface);
    if (texture == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << "  In PRL_TextLabel, the text texture creation failed: " << SDL_GetError() << endl;
    }

    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextLabel, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    if (useBkgd)
    {
        int factor = 1; // ignore the additional space if border size is 0
        if (borderSz == 0) factor = 0;

        if (bkgdtarget != NULL)
        {
            SDL_DestroyTexture(bkgdtarget); bkgdtarget = NULL;
        }

        bkgdtarget = PRL_CreateRectangle(dspRenderer, surface->w + 2*factor*(borderSz+spaceBetweenBorderAndText),
                                surface->h + 2*factor*(borderSz+spaceBetweenBorderAndText), bkgdColor, borderSz, borderColor);
        if (bkgdtarget == NULL)
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextLabel, unable to create a background texture (current text: " << text << ")" << endl;
        else
        {
            SDL_Rect rect = {borderSz+factor*spaceBetweenBorderAndText, borderSz+factor*spaceBetweenBorderAndText, surface->w, surface->h};
            SDL_SetRenderTarget(dspRenderer, bkgdtarget);
            SDL_RenderCopy(dspRenderer, texture, NULL, &rect);
            SDL_SetRenderTarget(dspRenderer, NULL);
            dspTexture = bkgdtarget;
            // update all the rect needed
            dspDst.w = dspSrc.w = surface->w + 2*factor*(borderSz+spaceBetweenBorderAndText);
            dspDst.h = dspSrc.h = surface->h + 2*factor*(borderSz+spaceBetweenBorderAndText);
            dspSrc.x = dspSrc.y = 0;
            updateDisplayRect();
        }
    }
    else
    {
        dspDst.w = surface->w;
        dspDst.h = surface->h;
        dspSrc.w = surface->w;
        dspSrc.h = surface->h;
        updateDisplayRect();
        dspTexture = texture;
    }

    #if PRL_MULTITHREADING == 1
    if (t1.joinable()) t1.join();
    t1 = std::thread(&renderText_surfaceManipulation, this);
    #endif // PRL_MULTITHREADING
}

void PRL_TextLabel :: allowDisplacement(bool b)
{
    allowDisplacement_ = b;
}

void PRL_TextLabel :: setReferenceRenderer(int x, int y)
{
    if (x > 0 && y > 0) referenceRenderer = {x, y};
}

void PRL_TextLabel :: update()
{
    if (allowDisplacement_ && input != NULL)
    {
        if (!beingMoved && input->isMouseButtonPressed(SDL_BUTTON_LEFT) && PRL_Coll_PointRect(input->getLastClickPoint(), dspDst))
        {
            beingMoved = true;
            relativeClickDistance = input->getLastClickFPoint();
            relativeClickDistance.x -= dspDst.x;
            relativeClickDistance.y -= dspDst.y;
        }
        else if (beingMoved && !input->isMouseButtonPressed(SDL_BUTTON_LEFT))
            beingMoved = false;

        if (beingMoved)
        {
            dspDst.x = input->getMouseF().x - relativeClickDistance.x;
            dspDst.y = input->getMouseF().y - relativeClickDistance.y;
            updateDisplayRect();
        }
    }
    renderText();
}


/** ********************************************* **/
/**             PRL_TextBoxSimple                 **/
/** ********************************************* **/

PRL_TextBoxSimple :: PRL_TextBoxSimple() : input(NULL), font(NULL), fontColor({255, 255, 255, 255}), texture(NULL), bkgdtarget(NULL), surface(NULL),
path(NULL), fontSz(0), fontStyle(TTF_STYLE_NORMAL), maxLineLength(200), isTrackingInput(false), allowTracking(false),
useBkgd(false), allowDisplacement_(false), beingMoved(false), relativeClickDistance({0}), bkgdColor({0}), bkgdTextInputColor(PRL_DEFAULT_TEXT_INPUT_BKGD_COLOR), borderColor({0}),
bkgdTempColor({0}), borderSz(0), spaceBetweenBorderAndText(PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT), renderingMethod(PRL_TEXTRENDER_BLENDED),
referenceRenderer({config_GLOBAL.renderResolution.x, config_GLOBAL.renderResolution.y})
{
    text = "";
    dspSrc.x = dspSrc.y = 0;
    updateDisplayRect();
    #if PRL_MULTITHREADING == 1
    isFirstRenderingDone = false;
    #endif // PRL_MULTITHREADING
}

PRL_TextBoxSimple :: ~PRL_TextBoxSimple()
{
    #if PRL_MULTITHREADING == 1
    t1.join();
    #endif // PRL_MULTITHREADING
    if (font != NULL) TTF_CloseFont(font);
    if (path != NULL) free(path);
    if (surface != NULL) SDL_FreeSurface(surface);
    if (texture != NULL) SDL_DestroyTexture(texture);
    if (bkgdtarget != NULL) SDL_DestroyTexture(bkgdtarget);
    if (isTrackingInput) input->disableTextInput();
}

int PRL_TextBoxSimple :: setFont(const char *fontPath, int fontSize, int lineMaxLength)
{
    setFontSize(fontSize);
    setLineMaxLength(lineMaxLength);
    return loadFont(fontPath);
}

int PRL_TextBoxSimple :: loadFont(const char *fontPath)
{
    char *pathtemp = (char*) malloc(strlen(fontPath) * sizeof(char));

    if (pathtemp == NULL)
    {
        return PRL_ERROR;
    }

    if (path != NULL) free(path);

    path = pathtemp;
    strcpy(path, fontPath);

    if (font != NULL)
    {
        TTF_CloseFont(font);
        font = NULL;
    }

    font = TTF_OpenFont(path, fontSz*config_GLOBAL.renderResolution.y/referenceRenderer.y); // take account of the scaling

    if (font == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBoxSimple, while attempting to load the font  " << path << ": " << TTF_GetError() << endl;
        return PRL_ERROR;
    }
    TTF_SetFontStyle(font, fontStyle);
    return 0;
}

void PRL_TextBoxSimple :: setFontSize(int sz)
{
    if (sz < 1)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBoxSimple, tried to set a font size smaller than 1 (" << sz << ")" << endl;
        #endif
        fontSz = 1;
    }
    else
        fontSz = sz;
}

void PRL_TextBoxSimple :: setInput(PRL_Input *in)
{
    if (in != NULL) input = (PRL_Input*) in;
}

void PRL_TextBoxSimple :: setLineMaxLength(int length)
{
    if (length < 1)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBoxSimple, tried to set a line length smaller than 1 (" << length << ")" << endl;
        #endif
        maxLineLength = 1;
    }
    else
        maxLineLength = length;
}

void PRL_TextBoxSimple :: setText(std::string txt)
{
    text = txt;
}

void PRL_TextBoxSimple :: setText(const char* txt)
{
    text = txt;
}

void PRL_TextBoxSimple :: setTextRenderMethod(PRL_TextRenderMethod method)
{
    renderingMethod = method;
}

void PRL_TextBoxSimple :: setTextColor(SDL_Color color)
{
    fontColor = color;
}

void PRL_TextBoxSimple :: setTextInputBkgColor(SDL_Color color)
{
    bkgdTextInputColor = color;
}

void PRL_TextBoxSimple :: setFontStyle(int style)
{
    fontStyle = style;
    if (font != NULL) TTF_SetFontStyle(font, fontStyle);
}

void PRL_TextBoxSimple :: useBackground(SDL_Color bkgd)
{
    useBkgd = true;
    bkgdColor = bkgd;
}

void PRL_TextBoxSimple :: useBackground(SDL_Color bkgd, SDL_Color border, int borderSize)
{
    useBkgd = true;
    bkgdColor = bkgd;
    borderColor = border;
    borderSz = borderSize;
}

void PRL_TextBoxSimple :: stopUsingBackground()
{
    useBkgd = false;
    borderSz = 0;
}

void PRL_TextBoxSimple :: renderText_surfaceManipulation() // split the work between surface manipulation and texture manipulation to allow multi-threading optimization
{
    if (surface != NULL)
    {
        SDL_FreeSurface(surface); surface = NULL;
    }

    bool gotRightLength(false);
    char* c_str = NULL; c_str = (char*) malloc ((text.length()+1) * sizeof(char));
    if (text.length() == 0) surface = TTF_RenderText_Solid(font, " ", fontColor);
    else
    {
        strcpy(c_str, text.c_str());
        if (renderingMethod == PRL_TEXTRENDER_SOLID) // solid rendering method
        {
            while (!gotRightLength)
            {
                surface = TTF_RenderUTF8_Solid(font, c_str, fontColor);

                if (surface->w > maxLineLength * config_GLOBAL.renderResolution.x/referenceRenderer.x) // scaling
                {
                    if (text.length() > 0) text.pop_back();
                    else gotRightLength = true; // kinda
                    if (isTrackingInput) input->setTextInputBeginning(text);
                    strcpy(c_str, text.c_str());
                }
                else
                    gotRightLength = true;
            }
        }
        else // blended rendering method
        {
            while (!gotRightLength)
            {
                surface = TTF_RenderUTF8_Blended(font, c_str, fontColor);

                if (surface->w > maxLineLength * config_GLOBAL.renderResolution.x/referenceRenderer.x) // scaling
                {
                    if (text.length() > 0) text.pop_back();
                    else gotRightLength = true; // kinda
                    if (isTrackingInput) input->setTextInputBeginning(text);
                    strcpy(c_str, text.c_str());
                }
                else
                    gotRightLength = true;
            }
        }
    }
    free(c_str);
}

void PRL_TextBoxSimple :: renderText()
{
    #if PRL_MULTITHREADING == 0
    renderText_surfaceManipulation();
    #elif PRL_MULTITHREADING == 1
    if (!isFirstRenderingDone)
    {
        isFirstRenderingDone = true;
        renderText_surfaceManipulation();
    }
    #endif // PRL_MULTITHREADING

    if (texture != NULL)
    {
        SDL_DestroyTexture(texture); texture = NULL;
    }
    if (surface == NULL) printf("ERROR surface NULL\n");

    texture = SDL_CreateTextureFromSurface(dspRenderer, surface);
    if (texture == NULL)
    {
        cerr << "Error: in PRL_TextBoxSimple, the text texture creation failed: " << SDL_GetError() << endl;
    }

    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBoxSimple, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    if (useBkgd)
    {
        int factor = 1; // ignore the additional space if border size is 0
        if (borderSz == 0) factor = 0;

        if (bkgdtarget != NULL)
        {
            SDL_DestroyTexture(bkgdtarget); bkgdtarget = NULL;
        }
        bkgdtarget = PRL_CreateRectangle(dspRenderer, surface->w + 2*factor*(borderSz+spaceBetweenBorderAndText),
                                surface->h + 2*factor*(borderSz+spaceBetweenBorderAndText), bkgdColor, borderSz, borderColor);
        if (bkgdtarget == NULL)
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBoxSimple, unable to create a background texture (current text: " << text << ")" << endl;
        else
        {
            SDL_Rect rect = {borderSz+factor*spaceBetweenBorderAndText, borderSz+factor*spaceBetweenBorderAndText, surface->w, surface->h};
            SDL_SetRenderTarget(dspRenderer, bkgdtarget);
            SDL_RenderCopy(dspRenderer, texture, NULL, &rect);
            SDL_SetRenderTarget(dspRenderer, NULL);
            dspTexture = bkgdtarget;
            // update all the rect needed
            dspDst.w = dspSrc.w = surface->w + 2*factor*(borderSz+spaceBetweenBorderAndText);
            dspDst.h = dspSrc.h = surface->h + 2*factor*(borderSz+spaceBetweenBorderAndText);
            dspSrc.x = dspSrc.y = 0;
            updateDisplayRect();
        }
    }
    else
    {
        dspDst.w = surface->w;
        dspDst.h = surface->h;
        dspSrc.w = surface->w;
        dspSrc.h = surface->h;
        updateDisplayRect();
        dspTexture = texture;
    }

    #if PRL_MULTITHREADING == 1
    //std::thread t1(&renderText_surfaceManipulation, this);
    if (t1.joinable()) t1.join();
    t1 = std::thread(&renderText_surfaceManipulation, this);
    #endif // PRL_MULTITHREADING
}

void PRL_TextBoxSimple :: startTextInputTracking()
{
    if (input != NULL)
    {
        allowTracking = true;
    }
    else
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBoxSimple, the pointer on PRL_Input has not been provided" << endl;
        #endif // PRL_USE_WARNINGS
    }
}

void PRL_TextBoxSimple :: startTextInputTracking(PRL_Input *in)
{
    if (in != NULL)
    {
        allowTracking = true;
        input = in;
    }
    else
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBoxSimple, the pointer on PRL_Input has not been provided" << endl;
        #endif // PRL_USE_WARNINGS
    }
}

void PRL_TextBoxSimple :: stopTextInputTracking()
{
    allowTracking = false;
}

void PRL_TextBoxSimple :: allowDisplacement(bool b)
{
    allowDisplacement_ = b;
}

void PRL_TextBoxSimple :: setReferenceRenderer(int x, int y)
{
    if (x > 0 && y > 0) referenceRenderer = {x, y};
}

void PRL_TextBoxSimple :: update()
{
    if (allowTracking && input != NULL)
    {
        if (!isTrackingInput && input->isMouseButtonPressed(SDL_BUTTON_LEFT)
            && PRL_Coll_PointRect(input->getLastClickPoint(), dspDst))
        { // start tracking
            isTrackingInput = true;
            input->enableTextInput();
            input->setTextInputBeginning(text);
            bkgdTempColor = bkgdColor;
            bkgdColor = bkgdTextInputColor;
        }
        else if ((isTrackingInput && input->isMouseButtonPressed(SDL_BUTTON_LEFT) && !PRL_Coll_PointRect(input->getLastClickPoint(), dspDst)) ||
                 (isTrackingInput && (input->isKeyPressed(SDL_SCANCODE_RETURN) || input->isKeyPressed(SDL_SCANCODE_RETURN2))))
        { // stop tracking
            isTrackingInput = false;
            input->disableTextInput();
            text = input->getTextInput();
            input->resetTextInput();
            bkgdColor = bkgdTempColor;
        }

        if (isTrackingInput && input->hasTextInputChanged()) // update the text corresponding to the input
        {
            text = input->getTextInput();
        }
    } // allowTracking


    if (allowDisplacement_ && input != NULL)
    {
        if (!beingMoved && input->isMouseButtonPressed(SDL_BUTTON_LEFT) && PRL_Coll_PointRect(input->getLastClickPoint(), dspDst))
        {
            beingMoved = true;
            relativeClickDistance = input->getLastClickFPoint();
            relativeClickDistance.x -= dspDst.x;
            relativeClickDistance.y -= dspDst.y;
        }
        else if (beingMoved && !input->isMouseButtonPressed(SDL_BUTTON_LEFT))
            beingMoved = false;

        if (beingMoved)
        {
            dspDst.x = input->getMouseF().x - relativeClickDistance.x;
            dspDst.y = input->getMouseF().y - relativeClickDistance.y;
            updateDisplayRect();
        }
    }
    renderText();
}


/** ********************************************* **/
/**                PRL_TextBox                    **/
/** ********************************************* **/

PRL_TextBox :: PRL_TextBox() : input(NULL), font(NULL), fontColor({255, 255, 255, 255}), texture(NULL), bkgdtarget(NULL), surface(NULL),
path(NULL), fontSz(0), fontStyle(TTF_STYLE_NORMAL), maxLineLength(200), isTrackingInput(false), allowTracking(false),
useBkgd(false), allowDisplacement_(false), beingMoved(false), relativeClickDistance({0}), bkgdColor({0}), bkgdTextInputColor(PRL_DEFAULT_TEXT_INPUT_BKGD_COLOR), borderColor({0}),
bkgdTempColor({0}), borderSz(0), spaceBetweenBorderAndText(PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT), renderingMethod(PRL_TEXTRENDER_BLENDED),
referenceRenderer({config_GLOBAL.renderResolution.x, config_GLOBAL.renderResolution.y})
{
    text = "";
    dspSrc.x = dspSrc.y = 0;
    updateDisplayRect();
    #if PRL_MULTITHREADING == 1
    isFirstRenderingDone = false;
    #endif // PRL_MULTITHREADING
}

PRL_TextBox :: ~PRL_TextBox()
{
    #if PRL_MULTITHREADING == 1
    for (int i(0); i<linesNumber; i++)
    {
        while (!t[i]->joinable()); t[i]->join();
    }
    if(surface != NULL) free(surface);
    #endif // PRL_MULTITHREADING
    if (font != NULL) TTF_CloseFont(font);
    if (path != NULL) free(path);
    for (int i(0); i<linesNumber; i++)
    {
        if (surface[i] != NULL) SDL_FreeSurface(surface[i]);
    }
    if(surface != NULL) free(surface);

    for (int i(0); i<linesNumber; i++)
    {
        if (texture[i] != NULL) SDL_DestroyTexture(texture[i]);
    }
    if(texture != NULL) free(texture);

    if (bkgdtarget != NULL) SDL_DestroyTexture(bkgdtarget);
    if (isTrackingInput) input->disableTextInput();
}

int PRL_TextBox :: setFont(const char *fontPath, int fontSize, int lineMaxLength, int maxLinesNb)
{
    setFontSize(fontSize);
    setLineMaxLength(lineMaxLength);
    setMaxLinesNb(maxLinesNb);
    return loadFont(fontPath);
}

int PRL_TextBox :: loadFont(const char *fontPath)
{
    char *pathtemp = (char*) malloc(strlen(fontPath) * sizeof(char));

    if (pathtemp == NULL)
    {
        return PRL_ERROR;
    }

    if (path != NULL) free(path);

    path = pathtemp;
    strcpy(path, fontPath);

    if (font != NULL)
    {
        TTF_CloseFont(font);
        font = NULL;
    }

    font = TTF_OpenFont(path, fontSz*config_GLOBAL.renderResolution.y/referenceRenderer.y); // take account of the scaling

    if (font == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, while attempting to load the font  " << path << ": " << TTF_GetError() << endl;
        return PRL_ERROR;
    }
    TTF_SetFontStyle(font, fontStyle);
    return 0;
}

void PRL_TextBox :: setFontSize(int sz)
{
    if (sz < 1)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, tried to set a font size smaller than 1 (" << sz << ")" << endl;
        #endif
        fontSz = 1;
    }
    else
        fontSz = sz;
}

void PRL_TextBox :: setInput(PRL_Input *in)
{
    if (in != NULL) input = (PRL_Input*) in;
}

void PRL_TextBox :: setLineMaxLength(int length)
{
    if (length < 1)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, tried to set a line length smaller than 1 (" << length << ")" << endl;
        #endif
        maxLineLength = 1;
    }
    else
        maxLineLength = length;
}

void PRL_TextBox :: setMaxLinesNb(int nb)
{
    if (nb < 1)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, tried to set a line number smaller than 1 (" << nb << ")" << endl;
        #endif
        maxLinesNumber = 1;
    }
    else
        maxLinesNumber = nb;
}

void PRL_TextBox :: setText(std::string txt)
{
    text = txt;
}

void PRL_TextBox :: setText(const char* txt)
{
    text = txt;
}

void PRL_TextBox :: setTextRenderMethod(PRL_TextRenderMethod method)
{
    renderingMethod = method;
}

void PRL_TextBox :: setTextColor(SDL_Color color)
{
    fontColor = color;
}

void PRL_TextBox :: setTextInputBkgColor(SDL_Color color)
{
    bkgdTextInputColor = color;
}

void PRL_TextBox :: setSelectedTextColor(SDL_Color color)
{
    selectedColor = color;
}

void PRL_TextBox :: setFontStyle(int style)
{
    fontStyle = style;
    if (font != NULL) TTF_SetFontStyle(font, fontStyle);
}

void PRL_TextBox :: useBackground(SDL_Color bkgd)
{
    useBkgd = true;
    bkgdColor = bkgd;
}

void PRL_TextBox :: useBackground(SDL_Color bkgd, SDL_Color border, int borderSize)
{
    useBkgd = true;
    bkgdColor = bkgd;
    borderColor = border;
    borderSz = borderSize;
}

void PRL_TextBox :: stopUsingBackground()
{
    useBkgd = false;
    borderSz = 0;
}

void PRL_TextBox :: renderText_surfaceManipulation(int lineNb) // split the work between surface manipulation and texture manipulation to allow multi-threading optimization
{
    /*if (lineNb >= linesNumber)
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, invalid line number (" << lineNb << ")" << endl;
    if (surface != NULL)
    {
        SDL_FreeSurface(*surface); surface = NULL;
    }

    bool gotRightLength(false);
    char* c_str = NULL; c_str = (char*) malloc ((text.length()+1) * sizeof(char));
    if (text.length() == 0) *surface = TTF_RenderText_Solid(font, " ", fontColor);
    else
    {
        strcpy(c_str, text.c_str());
        if (renderingMethod == PRL_TEXTRENDER_SOLID) // solid rendering method
        {
            while (!gotRightLength)
            {
                *surface = TTF_RenderUTF8_Solid(font, c_str, fontColor);

                if (*surface->w > maxLineLength * config_GLOBAL.renderResolution.x/referenceRenderer.x) // scaling
                {
                    if (text.length() > 0) text.pop_back();
                    else gotRightLength = true; // kinda
                    if (isTrackingInput) input->setTextInputBeginning(text);
                    strcpy(c_str, text.c_str());
                }
                else
                    gotRightLength = true;
            }
        }
        else // blended rendering method
        {
            while (!gotRightLength)
            {
                *surface = TTF_RenderUTF8_Blended(font, c_str, fontColor);

                if (*surface->w > maxLineLength * config_GLOBAL.renderResolution.x/referenceRenderer.x) // scaling
                {
                    if (text.length() > 0) text.pop_back();
                    else gotRightLength = true; // kinda
                    if (isTrackingInput) input->setTextInputBeginning(text);
                    strcpy(c_str, text.c_str());
                }
                else
                    gotRightLength = true;
            }
        }
    }
    free(c_str);*/
}

void PRL_TextBox :: moveLastWordToNextLine(int workingLine)
{
    if (workingLine < 0 || workingLine > linesNumber - 1)
        cerr << "";

    std::string lastWord = ""; bool done(false);
    int i(textLine[workingLine].length() - 1);
    for (; i >= 0 && !done; i--) // first loop to handle spaces
    {
        if (textLine[workingLine][i] == ' ')
            lastWord += " ";
        else done = true;
    }

    done = false;
    for (; i >= 0 && !done ; i--) // second loop to handle the actual word
    {
        if (textLine[workingLine][i] != ' ')
            lastWord = textLine[workingLine][i] + lastWord;
        else done = true;
    }

    if (!done) // if the word is larger than the whole line max length, cut it
    {
        lastWord = "";
        std::string temp = "";
        int positionInLine(textLine[workingLine].length()/2), remainingLength(positionInLine), length(0);
        while (!done)
        {
            for (i = 0; i < positionInLine; i++)
            {
                temp += textLine[i];
            }
            if (TTF_SizeUTF8(font, temp.c_str(), &length, NULL) == 0)
            {
                if (length < maxLineLength)
                    positionInLine += remainingLength / 2;
                else if (length > maxLineLength)
                    positionInLine -= remainingLength / 2;
                else if (length == maxLineLength || remainingLength == 0)
                    done = true;
            }
            else
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, unable to calculate line length: keeping the line this way, " << TTF_GetError() << endl;
                done = true;
            }
        }

        for (i = temp.length(); i<textLine[workingLine].length(); i++)
        {
            lastWord[i-temp.length()] = textLine[workingLine][i];
        }
        goto move_to_next_line_jump_point;
    }
    else // remove the lastWord from the working line
    {
        lineHasChanged[workingLine] = false; // no needs to update this line at the next update call
        // jump point
        move_to_next_line_jump_point:

        std::string lineCopy = textLine[workingLine]; // backup in case of a failure
        for (i=0; i<lastWord.length(); i++)
            textLine[workingLine].pop_back();

        if (workingLine + 1 < linesNumber)
            textLine[workingLine + 1] = lastWord + textLine[workingLine + 1]; // add the last word to the next line
        else // if the next line doesn't exist yet
        {
            linesNumber++;
            std::string *p(NULL); p = (std::string*) malloc(linesNumber * sizeof(std::string));
            bool *p2(NULL); p2 = (bool*) malloc(linesNumber * sizeof(bool));
            SDL_Surface **p3(NULL); p3 = (SDL_Surface**) malloc(linesNumber * sizeof(SDL_Surface*));
            std::thread **p4(NULL); p4 = (std::thread**) malloc(linesNumber * sizeof(std::thread));

            if (p == NULL || p2 == NULL || p3 == NULL || p4 == NULL)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, unable to allocate memory in order to create a new line: keeping the line this way" << endl;
                lineHasChanged[workingLine] = true;
                textLine[workingLine] = lineCopy;
                linesNumber--;
                if (p != NULL) free(p);
                if (p2 != NULL) free(p2);
                if (p3 != NULL) free(p3);
                if (p4 != NULL) free(p4);
            }
            else
            {
                for (i=0; i<linesNumber-1; i++)
                {
                    p[i] = textLine[i];
                    textLine[i] = ""; // empty the string
                    p2[i] = lineHasChanged[i];
                    p3[i] = surface[i];
                    p4[i] = t[i];
                }
                p[i] = lastWord;
                free(textLine);
                textLine = p;

                p2[i] = true; // update the next line too!
                free(lineHasChanged);
                lineHasChanged = p2;

                p3[i] = NULL;
                free(surface);
                surface = p3;

                p4[i] = new std::thread;
                free(t);
                t = p4;
            }
        }
    }
}

int PRL_TextBox :: renderText()
{
    // adapt lines length
    int length(0);
    for (int i(0); i<currentLine; i++)
    {
        if (TTF_SizeUTF8(font, textLine[i].c_str(), &length, NULL) == 0)
        {
            while (length > maxLineLength)
            {
                moveLastWordToNextLine(i);
                TTF_SizeUTF8(font, textLine[i].c_str(), &length, NULL); // assume it will give 0
            }
        }
        else
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, unable to calculate line length: " << TTF_GetError() << endl;
            return PRL_ERROR;
        }
    }



    #if PRL_MULTITHREADING == 0
    for (int i(0); i<linesNumber; i++)
    {
        if (lineHasChanged[i]) renderText_surfaceManipulation(i);
    }
    #elif PRL_MULTITHREADING == 1
    if (!isFirstRenderingDone)
    {
        isFirstRenderingDone = true;
        for (int i(0); i<linesNumber; i++)
        {
            if (lineHasChanged[i]) renderText_surfaceManipulation(i);
        }
    }
    #endif // PRL_MULTITHREADING

    if (texture != NULL)
    {
        ///SDL_DestroyTexture(texture); texture = NULL;
    }
    if (surface == NULL) printf("ERROR surface NULL\n");

    ///texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, the text texture creation failed: " << SDL_GetError() << endl;
    }

    ///if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    /*if (useBkgd)
    {
        int factor = 1; // ignore the additional space if border size is 0
        if (borderSz == 0) factor = 0;
        bkgdtarget = PRL_CreateRectangle(renderer, surface->w + 2*factor*(borderSz+spaceBetweenBorderAndText),
                                surface->h + 2*factor*(borderSz+spaceBetweenBorderAndText), bkgdColor, borderSz, borderColor);
        if (bkgdtarget == NULL)
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, unable to create a background texture (current text: " << text << ")" << endl;
        else
        {
            SDL_Rect rect = {borderSz+factor*spaceBetweenBorderAndText, borderSz+factor*spaceBetweenBorderAndText, surface->w, surface->h};
            SDL_SetRenderTarget(renderer, bkgdtarget);
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_SetRenderTarget(renderer, NULL);
            dspTexture = bkgdtarget;
            // update all the rect needed
            display_dst_int.w = display_dst.w = display_src_int.w = display_src.w = surface->w + 2*factor*(borderSz+spaceBetweenBorderAndText);
            display_dst_int.h = display_dst.h = display_src_int.h = display_src.h = surface->h + 2*factor*(borderSz+spaceBetweenBorderAndText);
            display_src_int.x = display_src_int.y = display_src.x = display_src.y = 0;
        }
    }
    else
    {
        display_dst_int.w = display_dst.w = surface->w;
        display_dst_int.h = display_dst.h = surface->h;
        display_src_int.w = display_src.w = surface->w;
        display_src_int.h = display_src.h = surface->h;
        display_texture = texture;
    }*/

    #if PRL_MULTITHREADING == 1
    for (int i(0); i<linesNumber; i++)
    {
        if (t[i]->joinable()) t[i]->join();
        *t[i] = std::thread(&renderText_surfaceManipulation, this, i);
    }
    #endif // PRL_MULTITHREADING
    return 0;
}

void PRL_TextBox :: startTextInputTracking()
{
    if (input != NULL)
    {
        allowTracking = true;
    }
    else
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, the pointer on PRL_Input has not been provided" << endl;
        #endif // PRL_USE_WARNINGS
    }
}

void PRL_TextBox :: startTextInputTracking(PRL_Input *in)
{
    if (in != NULL)
    {
        allowTracking = true;
        input = in;
    }
    else
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_TextBox, the pointer on PRL_Input has not been provided" << endl;
        #endif // PRL_USE_WARNINGS
    }
}

void PRL_TextBox :: stopTextInputTracking()
{
    allowTracking = false;
}

void PRL_TextBox :: allowDisplacement(bool b)
{
    allowDisplacement_ = b;
}

void PRL_TextBox :: setReferenceRenderer(int x, int y)
{
    if (x > 0 && y > 0) referenceRenderer = {x, y};
}

void PRL_TextBox :: update()
{
    if (allowTracking && input != NULL)
    {
        if (!isTrackingInput && input->isMouseButtonPressed(SDL_BUTTON_LEFT) && PRL_Coll_PointRect(input->getLastClickPoint(), dspDst))
        { // start tracking
            isTrackingInput = true;
            input->enableTextInput();
            input->setTextInputBeginning(text);
            bkgdTempColor = bkgdColor;
            bkgdColor = bkgdTextInputColor;
        }
        else if ((isTrackingInput && input->isMouseButtonPressed(SDL_BUTTON_LEFT) && !PRL_Coll_PointRect(input->getLastClickPoint(), dspDst)) ||
                 (isTrackingInput && (input->isKeyPressed(SDL_SCANCODE_RETURN) || input->isKeyPressed(SDL_SCANCODE_RETURN2))))
        { // stop tracking
            isTrackingInput = false;
            input->disableTextInput();
            text = input->getTextInput();
            input->resetTextInput();
            bkgdColor = bkgdTempColor;
        }

        if (isTrackingInput && input->hasTextInputChanged()) // update the text corresponding to the input
        {
            text = input->getTextInput();
        }
    } // allowTracking


    if (allowDisplacement_ && input != NULL)
    {
        if (!beingMoved && input->isMouseButtonPressed(SDL_BUTTON_LEFT) && PRL_Coll_PointRect(input->getLastClickPoint(), dspDst))
        {
            beingMoved = true;
            relativeClickDistance = input->getLastClickFPoint();
            relativeClickDistance.x -= dspDst.x;
            relativeClickDistance.y -= dspDst.y;
        }
        else if (beingMoved && !input->isMouseButtonPressed(SDL_BUTTON_LEFT))
            beingMoved = false;

        if (beingMoved)
        {
            dspDst.x = input->getMouseF().x - relativeClickDistance.x;
            dspDst.y = input->getMouseF().y - relativeClickDistance.y;
            updateDisplayRect();
        }
    }
    renderText();
}



// Parameters for char strings names
const int FILENAME_LENGTH(400), FORMAT_LENGTH(20), HUDELEMENT_TYPE_LENGTH(100);

/** ********************************************* **/
/**               PRL_VScrollBar                  **/
/** ********************************************* **/

PRL_VScrollBar :: PRL_VScrollBar(const PRL_Input *in, int barLength) : barOutput(0.0), input(NULL), idle(NULL), selected(NULL), rail(NULL),
target(NULL), miny(0), maxy(0), distFromTop(0), loaded(false), positionHasBeenProvided(false), barSelected(false), hasMoved_(false)
{
    input = (PRL_Input*) in;
    length = barLength;
    absBarPos = barPos = railPos = reference_renderer = {0};
}

PRL_VScrollBar :: ~PRL_VScrollBar()
{
    SDL_DestroyTexture(idle);
    SDL_DestroyTexture(selected);
    SDL_DestroyTexture(rail);
    SDL_DestroyTexture(target);
    SDL_SetRenderTarget(dspRenderer, NULL);
}

void PRL_VScrollBar :: setInput(const PRL_Input* in)
{
    if (in != NULL) input = (PRL_Input*) in;
}

void PRL_VScrollBar :: setPosition(int x, int y) // overloading PRL_Displayable method
{
    dspDst.x = x;
    dspDst.y = y;
    updateDisplayRect();

    miny = y;
    positionHasBeenProvided = true;
}

void PRL_VScrollBar :: setPosition(float x, float y) // overloading PRL_Displayable method
{
    dspDst.x = x;
    dspDst.y = y;
    updateDisplayRect();

    miny = y;
    positionHasBeenProvided = true;
}

void PRL_VScrollBar :: setCenterPosition(float x, float y) // overloading PRL_Displayable method
{
    if (positionHasBeenProvided)
    {
        dspDst.x = x - dspDst.w/2.0;
        dspDst.y = y - dspDst.h/2.0;
        updateDisplayRect();

        miny = y;
    }
}

void PRL_VScrollBar :: setCenterPosition(int x, int y) // overloading PRL_Displayable method
{
    if (positionHasBeenProvided)
    {
        dspDst.x = x - dspDst.w/2.0;
        dspDst.y = y - dspDst.h/2.0;
        updateDisplayRect();

        miny = y;
    }
}

bool PRL_VScrollBar :: isBarSelected() const
{
    return barSelected;
}

int PRL_VScrollBar :: load(const char file[])
{
    if (loaded) // if already loaded, free the memory
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, the files are already loaded" << endl;
        #endif // PRL_USE_WARNINGS
        return 0;
    }
    if (!positionHasBeenProvided)
    {
        positionHasBeenProvided = true;
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, the bar position has not been provided before loading. Setting default position to (0, 0)" << endl;
        #endif // PRL_USE_WARNINGS
    }
    FILE* filep = fopen(file, "r");
    if (filep == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " Could no open " << file << ": the file may be missing, locked or used" << endl;
        return PRL_ERROR;
    }

    char hudElementType[HUDELEMENT_TYPE_LENGTH] = "";

    char filename[3][FILENAME_LENGTH], path[FILENAME_LENGTH] = "", temp[FILENAME_LENGTH] = "", c = -1;
    int filename_count(0), pathlength(0);
    bool reading(true);

    while (reading) // reading hud element type
    {
        c = fgetc(filep);
        if (c == EOF)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, the .hud file " << file << " is badly defined" << endl;
            return PRL_ERROR;
        }
        else if (c == '\n') // end of the current char variable
        {
            reading = false;
        }
        else
        {
            if (filename_count == HUDELEMENT_TYPE_LENGTH)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, the .hud file " << file << " contains an animation file name of more than " << HUDELEMENT_TYPE_LENGTH << " characters" << endl;
                return PRL_ERROR;
            }
            strcpy(temp, "");
            sprintf(temp, "%c", c);
            strcat(hudElementType, temp);
            filename_count++;
        }
    }

    if (strcmp(hudElementType, "scrollbar") != 0) // warning : use a ANSI encoding!
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, the .hud file provided is not a scrollbar file (read: " << hudElementType << ")" << endl;
        return PRL_ERROR;
    }

    for (int i(0); i<3; i++)
    {
        reading = true;
        strcpy(filename[i], "");
        while (reading) // reading the 3 filenames
        {
            c = fgetc(filep);
            if (c == EOF)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, the .hud file " << file << " is badly defined" << endl;
                return PRL_ERROR;
            }
            else if (c == PRL_FILE_SEPARATION_CHARACTER || c == '\n') // end of the current char variable
            {
                reading = false;
            }
            else
            {
                if (filename_count == FILENAME_LENGTH)
                {
                    cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, the .hud file " << file << " contains an animation file name of more than " << FILENAME_LENGTH << " characters" << endl;
                    return PRL_ERROR;
                }
                sprintf(temp, "%c", c);
                strcat(filename[i], temp);
                filename_count++;
            }
        }
    }

    fscanf(filep, "%d:%d:%d", &reference_renderer.x, &reference_renderer.y, &railPos.w);

    reference_renderer.w = reference_renderer.x;
    reference_renderer.h = reference_renderer.y;

    float xratio = 1.0 * config_GLOBAL.renderResolution.x / reference_renderer.x; // prepares the ratio to change the rect size for correct display
    float yratio = 1.0 * config_GLOBAL.renderResolution.y / reference_renderer.y;

    for (int i(strlen(file)-1); i > 0; i--) // extract path
    {
        if (file[i] == '/' || file[i] == '\\') // if we reached the end of the file name
        {
            pathlength = i+1; // +1 to have the '/' character at the end of the string
            i = -1;
            char temp_path[4] = ""; strcpy(temp, "");
            for (int j(0); j < pathlength; j++)
            {
                sprintf(temp_path, "%c", file[j]);
                strcat(temp, temp_path);
            }
            strcat(temp, path);
            strcpy(path, temp);
        }
    }

    fclose(filep); // close the file

    char loadingName[2*FILENAME_LENGTH] = ""; // + 1 for the point and + 9 for the number string length (000001)
    SDL_Surface *surface = NULL;
    SDL_Texture *texture[3] = {NULL};

    for (int i(0); i<3; i++)
    {
        strcpy(loadingName, path); // add the path
        strcat(loadingName, filename[i]); // add the number . format
        surface = IMG_Load(loadingName);

        if (surface == NULL)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar: " << SDL_GetError() << endl;
            return PRL_ERROR;
        }
        texture[i] = SDL_CreateTextureFromSurface(dspRenderer, surface);

        if (SDL_SetTextureBlendMode(texture[i], SDL_BLENDMODE_BLEND) != 0)
        {
            #if PRL_USE_WARNINGS == 1
            cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
            #endif // PRL_USE_WARNINGS
        }

        if (texture[i] == NULL)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar, could not create the texture: " << SDL_GetError() << endl;
            SDL_FreeSurface(surface);
            return PRL_ERROR;
        }

        if (i == 1) // loading idle texture
        {
            barPos.w = (int) surface->w * xratio;
            barPos.h = (int) surface->h * yratio;
        }

        SDL_FreeSurface(surface); surface = NULL;
    }
    rail = texture[0];
    idle = texture[1];
    selected = texture[2];

    target = SDL_CreateTexture(dspRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, barPos.w, length);
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
    dspTexture = target;

    dspDst.w = barPos.w;
    dspDst.h = length;
    dspSrc.w = dspDst.w;// / xratio;
    dspSrc.h = dspDst.h;// / yratio;
    updateDisplayRect();

    barPos.x = barPos.y = 0;
    railPos.w *= xratio; // apply the scaling
    railPos.h = length; // not scaled!
    railPos.x = (barPos.w - railPos.w)/2;
    railPos.y = 0;

    maxy = miny + length - barPos.h - 1; // absolute measurement, minus 1 to avoid the single pixel down-shift. NB: minus 1 removed (for the moment)

    loaded = true;
    return 0;
}

void PRL_VScrollBar :: update()
{
    hasMoved_ = false;
    if (!positionHasBeenProvided)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_VScrollBar: the position has not been provided yet" << endl;
        #endif
    }
    else
    {
        if (!barSelected)
        {
            absBarPos = barPos;
            absBarPos.x += dspDst.x;
            absBarPos.y += dspDst.y;
            if (input->isMouseButtonPressed(SDL_BUTTON_LEFT) && PRL_Coll_PointRect(input->getLastClickPoint(), absBarPos))
            {
                barSelected = true;
                distFromTop = input->getMouse().y - (barPos.y + dspDst.y);
            }
        }

        if (barSelected)
        {
            if (!input->isMouseButtonPressed(SDL_BUTTON_LEFT))
                barSelected = false;
            else if (input->mouseHasMoved())
            {
                if (input->getMouse().y - distFromTop > maxy) barPos.y = length - barPos.h;
                else if (input->getMouse().y - distFromTop < miny) barPos.y = 0;
                else barPos.y = input->getMouse().y - distFromTop - dspDst.y;
                updateOutput();
                hasMoved_ = true;

            }
        }
        renderBar();
    }
}

float PRL_VScrollBar :: getBarOutput() const
{
    return barOutput;
}

bool PRL_VScrollBar :: hasMoved() const
{
    return hasMoved_;
}

void PRL_VScrollBar :: renderBar()
{
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_SetRenderTarget(dspRenderer, target);
    SDL_RenderClear(dspRenderer);
    SDL_RenderCopy(dspRenderer, rail, NULL, &railPos);

    if (barSelected) SDL_RenderCopy(dspRenderer, selected, NULL, &barPos);
    else SDL_RenderCopy(dspRenderer, idle, NULL, &barPos);
    SDL_SetRenderTarget(dspRenderer, NULL);
}

void PRL_VScrollBar :: updateOutput()
{
    barOutput = 1.0 * (barPos.y + dspDst.y - miny) / (maxy - miny + 1);
}



/** ********************************************* **/
/**               PRL_HScrollBar                  **/
/** ********************************************* **/


PRL_HScrollBar :: PRL_HScrollBar(const PRL_Input *in, int barLength) : barOutput(0.0), idle(NULL), selected(NULL), rail(NULL),
target(NULL), minx(0), maxx(0), distFromLeft(0), loaded(false), positionHasBeenProvided(false), barSelected(false), hasMoved_(false)
{
    input = (PRL_Input*) in;
    length = barLength;
    absBarPos = barPos = railPos = reference_renderer = {0};
}

PRL_HScrollBar :: ~PRL_HScrollBar()
{
    SDL_DestroyTexture(idle);
    SDL_DestroyTexture(selected);
    SDL_DestroyTexture(rail);
    SDL_DestroyTexture(target);
    SDL_SetRenderTarget(renderer_GLOBAL[0], NULL);
}

void PRL_HScrollBar :: setInput(const PRL_Input* in)
{
    if (in != NULL) input = (PRL_Input*) in;
}

void PRL_HScrollBar :: setPosition(int x, int y) // overloading PRL_Displayable method
{
    dspDst.x = x;
    dspDst.y = y;
    updateDisplayRect();

    minx = x;
    positionHasBeenProvided = true;
}

void PRL_HScrollBar :: setPosition(float x, float y) // overloading PRL_Displayable method
{
    dspDst.x = x;
    dspDst.y = y;
    updateDisplayRect();

    minx = x;
    positionHasBeenProvided = true;
}

void PRL_HScrollBar :: setCenterPosition(float x, float y) // overloading PRL_Displayable method
{
    if (positionHasBeenProvided)
    {
        dspDst.x = x - dspDst.w/2.0;
        dspDst.y = y - dspDst.h/2.0;
        updateDisplayRect();

        minx = x;
    }
}

void PRL_HScrollBar :: setCenterPosition(int x, int y) // overloading PRL_Displayable method
{
    if (positionHasBeenProvided)
    {
        dspDst.x = x - dspDst.w/2.0;
        dspDst.y = y - dspDst.h/2.0;
        updateDisplayRect();

        minx = x;
    }
}

bool PRL_HScrollBar :: isBarSelected() const
{
    return barSelected;
}

int PRL_HScrollBar :: load(const char file[])
{
    if (loaded) // if already loaded, free the memory
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, the files are already loaded" << endl;
        #endif // PRL_USE_WARNINGS
        return 0;
    }
    if (!positionHasBeenProvided)
    {
        positionHasBeenProvided = true;
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, the bar position has not been provided before loading. Setting default position to (0, 0)" << endl;
        #endif // PRL_USE_WARNINGS
    }
    FILE* filep = fopen(file, "r");
    if (filep == NULL)
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " Could no open " << file << ": the file may be missing, locked or used" << endl;
        return PRL_ERROR;
    }

    char hudElementType[HUDELEMENT_TYPE_LENGTH] = "";

    char filename[3][FILENAME_LENGTH], path[FILENAME_LENGTH] = "", temp[FILENAME_LENGTH] = "", c = -1;
    int filename_count(0), pathlength(0);
    bool reading(true);

    while (reading) // reading hud element type
    {
        c = fgetc(filep);
        if (c == EOF)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, the .hud file " << file << " is badly defined" << endl;
            return PRL_ERROR;
        }
        else if (c == '\n') // end of the current char variable
        {
            reading = false;
        }
        else
        {
            if (filename_count == HUDELEMENT_TYPE_LENGTH)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, the .hud file " << file << " contains an animation file name of more than " << HUDELEMENT_TYPE_LENGTH << " characters" << endl;
                return PRL_ERROR;
            }
            strcpy(temp, "");
            sprintf(temp, "%c", c);
            strcat(hudElementType, temp);
            filename_count++;
        }
    }

    if (strcmp(hudElementType, "scrollbar") != 0) // warning : use a ANSI encoding!
    {
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, the .hud file provided is not a scrollbar file (read: " << hudElementType << ". File encoding?)" << endl;
        return PRL_ERROR;
    }

    for (int i(0); i<3; i++)
    {
        reading = true;
        strcpy(filename[i], "");
        while (reading) // reading the 3 filenames
        {
            c = fgetc(filep);
            if (c == EOF)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, the .hud file " << file << " is badly defined" << endl;
                return PRL_ERROR;
            }
            else if (c == PRL_FILE_SEPARATION_CHARACTER || c == '\n') // end of the current char variable
            {
                reading = false;
            }
            else
            {
                if (filename_count == FILENAME_LENGTH)
                {
                    cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, the .hud file " << file << " contains an animation file name of more than " << FILENAME_LENGTH << " characters" << endl;
                    return PRL_ERROR;
                }
                sprintf(temp, "%c", c);
                strcat(filename[i], temp);
                filename_count++;
            }
        }
    }

    fscanf(filep, "%d:%d:%d", &reference_renderer.x, &reference_renderer.y, &railPos.h);

    reference_renderer.w = reference_renderer.x;
    reference_renderer.h = reference_renderer.y;

    float xratio = 1.0 * config_GLOBAL.renderResolution.x / reference_renderer.x; // prepares the ratio to change the rect size for correct display
    float yratio = 1.0 * config_GLOBAL.renderResolution.y / reference_renderer.y;

    for (int i(strlen(file)-1); i > 0; i--) // extract path
    {
        if (file[i] == '/' || file[i] == '\\') // if we reached the end of the file name
        {
            pathlength = i+1; // +1 to have the '/' character at the end of the string
            i = -1;
            char temp_path[4] = ""; strcpy(temp, "");
            for (int j(0); j < pathlength; j++)
            {
                sprintf(temp_path, "%c", file[j]);
                strcat(temp, temp_path);
            }
            strcat(temp, path);
            strcpy(path, temp);
        }
    }

    fclose(filep); // close the file

    char loadingName[2*FILENAME_LENGTH] = ""; // + 1 for the point and + 9 for the number string length (000001)
    SDL_Surface *surface = NULL;
    SDL_Texture *texture[3] = {NULL};

    for (int i(0); i<3; i++)
    {
        strcpy(loadingName, path); // add the path
        strcat(loadingName, filename[i]); // add the number . format
        surface = IMG_Load(loadingName);

        if (surface == NULL)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar: " << SDL_GetError() << endl;
            return PRL_ERROR;
        }
        texture[i] = SDL_CreateTextureFromSurface(dspRenderer, surface);

        if (SDL_SetTextureBlendMode(texture[i], SDL_BLENDMODE_BLEND) != 0)
        {
            #if PRL_USE_WARNINGS == 1
            cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
            #endif // PRL_USE_WARNINGS
        }

        if (texture[i] == NULL)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar, could not create the texture: " << SDL_GetError() << endl;
            SDL_FreeSurface(surface);
            return PRL_ERROR;
        }

        if (i == 1) // using idle texture to set the dst rect properly
        {
            barPos.w = (int) surface->w * xratio;
            barPos.h = (int) surface->h * yratio;
        }
        SDL_FreeSurface(surface); surface = NULL;
    }
    rail = texture[0];
    idle = texture[1];
    selected = texture[2];

    target = SDL_CreateTexture(dspRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, length, barPos.h);
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
    dspTexture = target;

    dspDst.w = length;
    dspDst.h = barPos.h;
    dspSrc.w = dspDst.w;// / xratio;
    dspSrc.h = dspDst.h;// / yratio;
    updateDisplayRect();

    barPos.x = barPos.y = 0;

    railPos.w = length; // not scaled
    railPos.h *= yratio; // apply the scaling
    railPos.x = 0;
    railPos.y = (barPos.h - railPos.h)/2;

    maxx = minx + length - barPos.w - 1; // absolute measurement, minus 1 to avoid the single pixel left-shift NB: minus 1 removed for the moment

    loaded = true;
    return 0;
}

void PRL_HScrollBar :: update()
{
    hasMoved_ = false;
    if (!positionHasBeenProvided)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_HScrollBar: the position has not been provided yet" << endl;
        #endif
    }
    else
    {
        if (!barSelected)
        {
            absBarPos = barPos;
            absBarPos.x += dspDst.x;
            absBarPos.y += dspDst.y;
            if (input->isMouseButtonPressed(SDL_BUTTON_LEFT) && PRL_Coll_PointRect(input->getLastClickPoint(), absBarPos))
            {
                barSelected = true;
                distFromLeft = input->getMouse().x - (barPos.x + dspDst.x);
            }
        }

        if (barSelected)
        {
            if (!input->isMouseButtonPressed(SDL_BUTTON_LEFT))
                barSelected = false;
            else if (input->mouseHasMoved())
            {
                if (input->getMouse().x - distFromLeft > maxx) barPos.x = length - barPos.w;
                else if (input->getMouse().x - distFromLeft < minx) barPos.x = 0;
                else barPos.x = input->getMouse().x - distFromLeft - dspDst.x;
                updateOutput();
                hasMoved_ = true;
            }
        }
        renderBar();
    }
}

float PRL_HScrollBar :: getBarOutput() const
{
    return barOutput;
}

bool PRL_HScrollBar :: hasMoved() const
{
    return hasMoved_;
}

void PRL_HScrollBar :: renderBar()
{
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_SetRenderTarget(dspRenderer, target);
    SDL_RenderClear(dspRenderer);
    SDL_RenderCopy(dspRenderer, rail, NULL, &railPos);

    if (barSelected) SDL_RenderCopy(dspRenderer, selected, NULL, &barPos);
    else SDL_RenderCopy(dspRenderer, idle, NULL, &barPos);
    SDL_SetRenderTarget(dspRenderer, NULL);
}

void PRL_HScrollBar :: updateOutput()
{
    barOutput = 1.0 * (barPos.x + dspDst.x - minx) / (maxx - minx + 1);
}

