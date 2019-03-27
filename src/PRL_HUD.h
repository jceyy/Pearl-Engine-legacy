#ifndef PRL_HUD_H_INCLUDED
#define PRL_HUD_H_INCLUDED

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <thread>
#include "PRL_display.h"
#include "PRL_input.h"
/*
#define PRL_DEFAULT_TEXT_COLOR {255, 255, 255, 255}
#define PRL_DEFAULT_TEXT_INPUT_BKGD_COLOR {255, 255, 255, 128}
#define PRL_HUD_MAX_BORDER_SIZE 200 // in pixels
#define PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT 5 // in pixels

/// Destroy and fill the provided texture with the appropriate rectangle background with a border.
SDL_Texture* PRL_CreateRectangle(SDL_Renderer *renderer, int w, int h, SDL_Color const& bkgd, int borderSz, SDL_Color const& border);
/// Destroy and fill the provided texture with the appropriate rectangle background without border.
SDL_Texture* PRL_CreateRectangle(SDL_Renderer *renderer, int w, int h, SDL_Color const& bkgd);

enum PRL_TextRenderMethod
{
    PRL_TEXTRENDER_SOLID, PRL_TEXTRENDER_BLENDED, PRL_TEXTRENDER_DEFAULT = PRL_TEXTRENDER_BLENDED
};



class PRL_TextLabel : public PRL_Displayable
{
public:
    PRL_TextLabel();
    PRL_TextLabel(std::string txt);
    PRL_TextLabel(const char *txt);
    ~PRL_TextLabel();

    ///Prepare the font needed by providing the font path, font size (in pt for 72 DPI) and the line's max length (in pxls)
    int setFont(const char *fontPath, int fontSize);
    /// Set the input to use if displacement is allowed
    void setInput(PRL_Input *in);
    /// Set text as a string
    void setText(std::string txt);
    /// Set text as a C like char string
    void setText(const char *txt);
    /// Set whether to use the solid (PRL_TEXTRENDER_SOLID, rough) or blended (PRL_TEXTRENDER_BLENDED, smoothed) text rendering method
    void setTextRenderMethod(PRL_TextRenderMethod method);
    /// Set the text color. By default PRL_DEFAULT_TEXT_COLOR
    void setTextColor(SDL_Color color);
    /// Set font style between TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_STRIKETHROUGH, TTF_STYLE_UNDERLINE and TTF_STYLE_NORMAL. By default TTF_STYLE_NORMAL
    void setFontStyle(int style);
    /// Set a background color and start using it
    void useBackground(SDL_Color bkgd);
    /// Set a background color with borders and start using it
    void useBackground(SDL_Color bkgd, SDL_Color border, int borderSize);
    /// Stop using a background for the text box
    void stopUsingBackground();
    /// Allow or not the text box to be moved around
    void allowDisplacement(bool b);
    /// Sets the default distance between the border and the text in pixels. By default PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT
    void setSpaceBetweenBorderAndText(int pxl);
    /// Sets the reference renderer. This has to be done before loading the font or it may result in no effect
    void setReferenceRenderer(int x, int y);
    /// Render text and update position if displacement is allowed
    void update();

private:
    PRL_Input *input;
    TTF_Font *font;
    SDL_Color fontColor;
    SDL_Texture *texture, *bkgdtarget;
    SDL_Surface *surface;
    char *path; // file path of the font
    std::string text;
    int fontSz;
    int fontStyle;

    bool useBkgd; // use background or not
    bool allowDisplacement_, beingMoved;
    PRL_FPoint relativeClickDistance; // used when moving the text label

    SDL_Color bkgdColor, borderColor, bkgdTempColor;
    int borderSz, spaceBetweenBorderAndText;
    PRL_TextRenderMethod renderingMethod;

    PRL_Point referenceRenderer;

    #if PRL_MULTITHREADING == 1
    bool isFirstRenderingDone;
    std::thread t1;
    #endif // PRL_MULTITHREADING

    int loadFont(const char *fontPath);
    void setFontSize(int sz);
    void renderText();
    void renderText_surfaceManipulation();
};





/// Simple line text box without visual cursor handling
class PRL_TextBoxSimple : public PRL_Displayable
{
public:
    PRL_TextBoxSimple();
    ~PRL_TextBoxSimple();

    ///Prepare the font needed by providing the font path, font size (in pt for 72 DPI) and the line's max length (in pxls)
    int setFont(const char *fontPath, int fontSize, int lineMaxLength);
    /// Set the input to use
    void setInput(PRL_Input *in);
    /// Set text as a string
    void setText(std::string txt);
    /// Set text as a C like char string
    void setText(const char *txt);
    /// Set whether to use the solid (PRL_TEXTRENDER_SOLID, rough) or blended (PRL_TEXTRENDER_BLENDED, smoothed) text rendering method
    void setTextRenderMethod(PRL_TextRenderMethod method);
    /// Set the text color. By default PRL_DEFAULT_TEXT_COLOR
    void setTextColor(SDL_Color color);
    /// When using a background, set the background color used on text input
    void setTextInputBkgColor(SDL_Color color);
    /// Set font style between TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_STRIKETHROUGH, TTF_STYLE_UNDERLINE and TTF_STYLE_NORMAL. By default TTF_STYLE_NORMAL
    void setFontStyle(int style);
    /// Set the line max length in pixels
    void setLineMaxLength(int lenth);
    /// Set a background color and start using it
    void useBackground(SDL_Color bkgd);
    /// Set a background color with borders and start using it
    void useBackground(SDL_Color bkgd, SDL_Color border, int borderSize);
    /// Stop using a background for the text box
    void stopUsingBackground();
    /// Start tracking text input. NB: the pointer on a PRL_Input class has to be already provided (or use void startTextInputTracking(PRL_Input *in))
    void startTextInputTracking();
    /// Set the input class to use and start tracking text input
    void startTextInputTracking(PRL_Input *in);
    ///Stop tracking text input
    void stopTextInputTracking();
    /// Allow or not the text box to be moved around
    void allowDisplacement(bool b);
    /// Sets the default distance between the border and the text in pixels. By default PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT
    void setSpaceBetweenBorderAndText(int pxl);
    /// Sets the reference renderer. This has to be done before loading the font or it may result in no effect
    void setReferenceRenderer(int x, int y);
    /// If tracking text input, update the text and render the text. Otherwise it just renders the text.
    void update();


private:
    PRL_Input *input;
    TTF_Font *font;
    SDL_Color fontColor;
    SDL_Texture *texture, *bkgdtarget;
    SDL_Surface *surface;
    char *path; // file path of the font
    std::string text;
    int fontSz;
    int fontStyle;
    int maxLineLength; // in pixels

    bool isTrackingInput, allowTracking;
    bool useBkgd; // use background or not
    bool allowDisplacement_, beingMoved;
    PRL_FPoint relativeClickDistance; // used when moving the text box

    SDL_Color bkgdColor, bkgdTextInputColor, borderColor, bkgdTempColor;
    int borderSz, spaceBetweenBorderAndText;
    PRL_TextRenderMethod renderingMethod;

    PRL_Point referenceRenderer;

    #if PRL_MULTITHREADING == 1
    bool isFirstRenderingDone;
    std::thread t1;
    #endif // PRL_MULTITHREADING

    int loadFont(const char *fontPath);
    void setFontSize(int sz);
    void renderText();
    void renderText_surfaceManipulation();
};



#define PRL_DEFAULT_LINE_SPACING 5 // in pixels
#define PRL_DEFAULT_SELECTED_TEXT_COLOR {220, 220, 220, 220}

class PRL_TextBox : public PRL_Displayable
{
public:
    PRL_TextBox();
    ~PRL_TextBox();

    ///Prepare the font needed by providing the font path, font size (in pt for 72 DPI) and the line's max length (in pxls)
    int setFont(const char *fontPath, int fontSize, int lineMaxLength, int maxLinesNb);
    /// Set the input to use
    void setInput(PRL_Input *in);
    /// Set text as a string
    void setText(std::string txt);
    /// Set text as a C like char string
    void setText(const char *txt);
    /// Set whether to use the solid (PRL_TEXTRENDER_SOLID, rough) or blended (PRL_TEXTRENDER_BLENDED, smoothed) text rendering method
    void setTextRenderMethod(PRL_TextRenderMethod method);
    /// Set the text color. By default PRL_DEFAULT_TEXT_COLOR
    void setTextColor(SDL_Color color);
    /// When using a background, set the background color used on text input
    void setTextInputBkgColor(SDL_Color color);
    /// Color behind the text when selected. By default PRL_DEFAULT_SELECTED_TEXT_COLOR
    void setSelectedTextColor(SDL_Color color);
    /// Set font style between TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_STRIKETHROUGH, TTF_STYLE_UNDERLINE and TTF_STYLE_NORMAL. By default TTF_STYLE_NORMAL
    void setFontStyle(int style);

    void setLineMaxLength(int lenth);

    void setMaxLinesNb(int nb);
    /// Set a background color and start using it
    void useBackground(SDL_Color bkgd);
    /// Set a background color with borders and start using it
    void useBackground(SDL_Color bkgd, SDL_Color border, int borderSize);
    /// Stop using a background for the text box
    void stopUsingBackground();
    /// Start tracking text input. NB: the pointer on a PRL_Input class has to be already provided (or use void startTextInputTracking(PRL_Input *in))
    void startTextInputTracking();
    /// Set the input class to use and start tracking text input
    void startTextInputTracking(PRL_Input *in);
    ///Stop tracking text input
    void stopTextInputTracking();
    /// Allow or not the text box to be moved around
    void allowDisplacement(bool b);
    /// Sets the default distance between the border and the text in pixels. By default PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT
    void setSpaceBetweenBorderAndText(int pxl);
    /// Sets the reference renderer. This has to be done before loading the font or it may result in no effect
    void setReferenceRenderer(int x, int y);
    /// If tracking text input, update the text and render the text. Otherwise it just renders the text.
    void update(); // TO BECOME INT TYPE


private:
    PRL_Input *input;
    TTF_Font *font;
    SDL_Color fontColor;
    SDL_Texture **texture, *bkgdtarget;
    SDL_Surface **surface; // dynamic array of surfaces to allow multi-threading
    bool *lineHasChanged;
    std::string *textLine;
    SDL_Surface *sampleText; // store a simple space in current
    char *path; // file path of the font
    std::string text, selectedText;
    int fontSz;
    int fontStyle;
    int maxLineLength; // in pixels
    int currentLine, linesNumber, maxLinesNumber, lineSpacing;

    bool isTrackingInput, allowTracking;
    bool useBkgd; // use background or not
    bool allowDisplacement_, beingMoved;
    PRL_FPoint relativeClickDistance; // used when moving the text box

    SDL_Color bkgdColor, bkgdTextInputColor, borderColor, bkgdTempColor, selectedColor;
    int borderSz, spaceBetweenBorderAndText;
    PRL_TextRenderMethod renderingMethod;

    PRL_Point referenceRenderer;

    bool isFirstRenderingDone; // no multi-threading define to reduce code complexity
    std::thread **t;

    int loadFont(const char *fontPath);
    void setFontSize(int sz);
    int renderText();
    void renderText_surfaceManipulation(int lineNb);
    void moveLastWordToNextLine(int workingLine);
};



class PRL_VScrollBar : public PRL_Displayable
{
public:
    PRL_VScrollBar(const PRL_Input *in, int barLength);
    ~PRL_VScrollBar();
    /// Set a pointer on the desired PRL_Input class
    void setInput(const PRL_Input *in);
    /// Set the position (upper left corner)
    void setPosition(int x, int y);
    /// Set the position (upper left corner)
    void setPosition(float x, float y);
    /// Set the position of the center. Use this method only after loading!
    void setCenterPosition(int x, int y);
    /// Set the position of the center. Use this method only after loading!
    void setCenterPosition(float x, float y);
    /// Tell whether the bar is selected or not
    bool isBarSelected() const;
    /// Load the vertical scroll bar with a .hud file
    int load(const char file_path[]);
    /// Update the bar
    void update();
    /// Get the bar output between 0.0 and 1.0 (0.0 is up, 1.0 is down)
    float getBarOutput() const;
    /// Tell whether the bar has moved or not
    bool hasMoved() const;

private:
    float barOutput; // between 0.0 and 1.0
    PRL_Input *input;

    SDL_Texture *idle, *selected, *rail, *target;

    SDL_Rect absBarPos, barPos, railPos;
    SDL_Rect reference_renderer;
    int miny, maxy, distFromTop;
    int length; // the actual length of the bar

    bool loaded, positionHasBeenProvided;
    bool barSelected; // The bar has been selected and moves with the mouse
    bool hasMoved_; // The bar has moved

    void renderBar();
    void updateOutput();
};



class PRL_HScrollBar : public PRL_Displayable
{
public:
    /// Constructor: provide a pointer on the PRL_Input object used and the bar length (pxl)
    PRL_HScrollBar(const PRL_Input *in, int barLength);
    ~PRL_HScrollBar();
    /// Set a pointer on the desired PRL_Input class
    void setInput(const PRL_Input *in);
    /// Set the position (upper left corner)
    void setPosition(int x, int y);
    /// Set the position (upper left corner)
    void setPosition(float x, float y);
    /// Set the position of the center. Use this method only after loading!
    void setCenterPosition(int x, int y);
    /// Set the position of the center. Use this method only after loading!
    void setCenterPosition(float x, float y);
    /// Tell whether the bar is selected or not
    bool isBarSelected() const;
    /// Load the horizontal scroll bar with a .hud file
    int load(const char file_path[]);
    /// Update the bar
    void update();
    /// Get the bar output between 0.0 and 1.0 (0.0 is left, 1.0 is right)
    float getBarOutput() const;
    /// Tell whether the bar has moved or not
    bool hasMoved() const;

private:
    float barOutput; // between 0.0 and 1.0
    PRL_Input *input;

    SDL_Texture *idle, *selected, *rail, *target;

    SDL_Rect absBarPos, barPos, railPos;
    SDL_Rect reference_renderer;
    int minx, maxx, distFromLeft;
    int length; // the actual length of the bar

    bool loaded, positionHasBeenProvided;
    bool barSelected; // The bar has been selected and moves with the mouse
    bool hasMoved_; // The bar has moved

    void renderBar();
    void updateOutput();
};
*/
#endif // PRL_HUD_H_INCLUDED
