#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "PRL_display.h"
#include "PRL_global_variables.h"

using std::cout;
using std::cerr;
using std::endl;


/** ********************************************* **/
/**              PRL_Displayable                  **/
/** ********************************************* **/

int PRL_Displayable :: dspCount = 0;

PRL_Displayable :: PRL_Displayable() : dspSrc({0}), dspDst({0}), dspTexture(nullptr), dspRenderer(renderer_GLOBAL[0]), dspIsActive(true),
dspAngle(0.0), dspSrc_int({0}), dspDst_int({0}), dspDisplayerAddress(0), dspDisplayerAdded(false)
{
    dspCount++;
}

PRL_Displayable :: ~PRL_Displayable()
{
    dspCount--;
}

SDL_Texture* PRL_Displayable :: getTexture() const
{
    return dspTexture;
}

int PRL_Displayable :: setTexture(SDL_Texture* texture)
{
    if (texture == nullptr)
    {
        PRL_SetError("Invalid texture (nullptr)");
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << PRL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        return PRL_ERROR;
    }

    dspTexture = texture;
    return 0;
}

int PRL_Displayable :: setRenderer(SDL_Renderer *renderer)
{
    if (renderer == nullptr)
    {
        PRL_SetError("Invalid renderer (nullptr)");
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << PRL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        return PRL_ERROR;
    }

    dspRenderer = renderer;
    return 0;
}

void PRL_Displayable :: makeActive(bool b)
{
    dspIsActive = b;
}

bool PRL_Displayable :: isActive() const
{
    return dspIsActive;
}

void PRL_Displayable :: setAngle(double angle)
{
    dspAngle = angle;
}
double PRL_Displayable :: getAngle() const
{
    return dspAngle;
}

void PRL_Displayable :: setPos(float x, float y)
{
    dspDst.x = x;
    dspDst.y = y;
}

void PRL_Displayable :: setPos(PRL_FPoint pos)
{
    dspDst.x = pos.x;
    dspDst.y = pos.y;
}

void PRL_Displayable :: setCenterPos(float x, float y)
{
    dspDst.x = x - dspDst.w/2.0;
    dspDst.y = y - dspDst.h/2.0;
}

void PRL_Displayable :: setCenterPos(PRL_FPoint pos)
{
    dspDst.x = pos.x - dspDst.w/2.0;
    dspDst.y = pos.y - dspDst.h/2.0;
}

PRL_FRect const& PRL_Displayable :: getDstRect() const
{
    return dspDst;
}

PRL_FRect const& PRL_Displayable :: getSrcRect() const
{
    return dspSrc;
}

int PRL_Displayable :: getDspCount()
{
    return dspCount;
}

void PRL_Displayable :: updateDisplayRect()
{
    dspDst_int = {(int) floor(dspDst.x), (int) floor(dspDst.y), (int) floor(dspDst.w), (int) floor(dspDst.h)};
    dspSrc_int = {(int) floor(dspSrc.x), (int) floor(dspSrc.y), (int) floor(dspSrc.w), (int) floor(dspSrc.h)};
}

/** ********************************************* **/
/**               PRL_Displayer                   **/
/** ********************************************* **/

int PRL_Displayer :: dsprCount = 0;

PRL_Displayer :: PRL_Displayer() :  camera(nullptr)
{
    dsprCount++;
}

PRL_Displayer :: ~PRL_Displayer()
{
    object.clear();
    dsprCount--;
}

int PRL_Displayer :: add(PRL_Displayable *obj)
{
    if (obj == nullptr)
    {
        PRL_SetError("Invalid object (nullptr)");
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << PRL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        return PRL_ERROR;
    }
    else if (obj->dspDisplayerAdded)
    {
        PRL_SetError("Object already added");
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << PRL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        return PRL_ERROR;
    }

    object.push_back(obj);
    obj->dspDisplayerAddress = object.size() - 1;
    obj->dspDisplayerAdded = true;
    return 0;
}

int PRL_Displayer :: remove(PRL_Displayable *obj)
{
    if (obj == nullptr)
    {
        PRL_SetError("Invalid object (nullptr)");
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << PRL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        return PRL_ERROR;
    }
    else if (!obj->dspDisplayerAdded)
    {
        PRL_SetError("Invalid object (not added)");
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << PRL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        return PRL_ERROR;
    }

    object.erase(object.begin() + obj->dspDisplayerAddress);
    obj->dspDisplayerAdded = false;
    for (size_t i(obj->dspDisplayerAddress); i < object.size(); i++)
    {
        object[i]->dspDisplayerAddress--;
    }
    return 0;
}

size_t PRL_Displayer :: getObjectsNumber() const
{
    return object.size();
}

int PRL_Displayer :: display() const
{
    for (int i(0); i < object.size(); i++)
    {
        if (object[i]->dspIsActive)
        {
            if (SDL_RenderCopyEx(object[i]->dspRenderer, object[i]->dspTexture, &(object[i]->dspSrc_int),
                &(object[i]->dspDst_int), object[i]->dspAngle, NULL, SDL_FLIP_NONE) != 0)
            {
                PRL_SetError(SDL_GetError());
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << PRL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                return PRL_ERROR;
            }
        }
    }
    return 0;
}

int PRL_Displayer :: displayWithCamera() const
{
    SDL_Rect dst;
    double zoom = camera->getZoom();

    for (int i(0); i < object.size(); i++)
    {
        if (object[i]->dspIsActive)
        {
            dst.x = zoom * (object[i]->dspDst.x - camera->getUpLeftCorner().x);
            dst.y = zoom * (object[i]->dspDst.y - camera->getUpLeftCorner().y);
            dst.w = zoom * object[i]->dspDst.w;
            dst.h = zoom * object[i]->dspDst.h;

            if (SDL_RenderCopyEx(object[i]->dspRenderer, object[i]->dspTexture, &(object[i]->dspSrc_int),
                &dst, object[i]->dspAngle, NULL, SDL_FLIP_NONE) != 0)
            {
                PRL_SetError(SDL_GetError());
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << PRL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                return PRL_ERROR;
            }
        }
    }
    return 0;
}

void PRL_Displayer :: setCamera(PRL_Camera *cam)
{
    if (cam != nullptr) camera = cam;
    else
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Displayer, tried to set a NULL pointer on a camera" << endl;
        #endif // PRL_USE_WARNINGS
}

int PRL_Displayer :: getDsprCount()
{
    return dsprCount;
}
