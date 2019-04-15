#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "PRL_display.h"
#include "PRL_global.h"

using std::cout;
using std::cerr;
using std::endl;

#define PRL_DISPLAYABLE_DEFAULT_ADDRESS -1 // a negative value is important!

/* ********************************************* */
/*              PRL_Displayable                  */
/* ********************************************* */

int PRL_Displayable :: dspCount = 0;

PRL_Displayable :: PRL_Displayable() : dspDst({0.0f, 0.0f, 0, 0}), dspTexture(nullptr),
dspRenderer(handler.display.renderer[0]), dspVelocity({0.0f, 0.0f}), dspIsActive(true), dspAngle(0.0),
dspDisplayerAddress(0), dspDisplayerAdded(false)
{
    dspCount++;
}

PRL_Displayable :: PRL_Displayable(SDL_Texture *texture, SDL_Renderer *renderer)
{
	PRL_Displayable();

	if (texture == nullptr)
	{
		PRL_SetError("Invalid texture");
		std::runtime_error e(PRL_GetError());
		throw(e);
	}
	if (renderer == nullptr)
	{
		PRL_SetError("Invalid renderer");
		std::runtime_error e(PRL_GetError());
		throw(e);
	}

	dspRenderer = renderer;
	dspTexture = texture;
}

PRL_Displayable :: ~PRL_Displayable()
{
    dspCount--;
}

SDL_Texture* PRL_Displayable :: getTexture() const noexcept
{
    return dspTexture;
}

int PRL_Displayable :: set(SDL_Texture *texture, SDL_Renderer *renderer) noexcept
{
    if (texture == nullptr)
    {
        PRL_SetError("Invalid texture (nullptr)");
        return PRL_ERROR;
    }

    if (renderer == nullptr)
    {
        PRL_SetError("Invalid renderer (nullptr)");
        return PRL_ERROR;
    }
	dspRenderer = renderer;
    dspTexture = texture;
    return 0;
}

void PRL_Displayable :: makeActive(bool b) noexcept
{
    dspIsActive = b;
}

void PRL_Displayable :: makeStatic(bool b) noexcept
{
	dspIsStatic = b;
}

bool PRL_Displayable :: isActive() const noexcept
{
    return dspIsActive;
}

bool PRL_Displayable :: isStatic() const noexcept
{
	return dspIsStatic;
}

void PRL_Displayable :: setAngle(double angle) noexcept
{
    dspAngle = angle;
}

double PRL_Displayable :: getAngle() const noexcept
{
    return dspAngle;
}

void PRL_Displayable :: setPos(float x, float y) noexcept
{
    dspDst.x = x;
    dspDst.y = y;
}

void PRL_Displayable :: setPos(PRL_FPoint pos) noexcept
{
    dspDst.x = pos.x;
    dspDst.y = pos.y;
}

void PRL_Displayable :: setCenterPos(float x, float y) noexcept
{
    dspDst.x = x - dspDst.w/2.0;
    dspDst.y = y - dspDst.h/2.0;
}

void PRL_Displayable :: setCenterPos(PRL_FPoint pos) noexcept
{
    dspDst.x = pos.x - dspDst.w/2.0;
    dspDst.y = pos.y - dspDst.h/2.0;
}

PRL_FRect const& PRL_Displayable :: getDstRect() const noexcept
{
    return dspDst;
}

void PRL_Displayable :: setVelocity(PRL_FPoint const& velocity) noexcept
{
	dspVelocity = velocity;
}

PRL_FPoint const& PRL_Displayable :: getVelocity() const noexcept
{
	return dspVelocity;
}

int PRL_Displayable :: getCount() noexcept
{
    return dspCount;
}


/* ********************************************* */
/*               PRL_Displayer                   */
/* ********************************************* */

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

void rect2FRect(PRL_FRect const& frect, SDL_Rect& rect)
{
    rect = {(int) floor(frect.x), (int) floor(frect.y), (int) floor(frect.w), (int) floor(frect.h)};
}

int PRL_Displayer :: display() const
{
    SDL_Rect dst;
    for (size_t i(0); i < object.size(); i++)
    {
        if (object[i]->dspIsActive)
        {
            rect2FRect(object[i]->dspDst, dst);
            if (SDL_RenderCopyEx(object[i]->dspRenderer, object[i]->dspTexture, nullptr,
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

int PRL_Displayer :: displayWithCamera() const
{
    SDL_Rect dst;
    double zoom = camera->getZoom();

    for (size_t i(0); i < object.size(); i++)
    {
        if (object[i]->dspIsActive)
        {
            dst.x = zoom * (object[i]->dspDst.x - camera->getUpLeftCorner().x);
            dst.y = zoom * (object[i]->dspDst.y - camera->getUpLeftCorner().y);
            dst.w = zoom * object[i]->dspDst.w;
            dst.h = zoom * object[i]->dspDst.h;

            if (SDL_RenderCopyEx(object[i]->dspRenderer, object[i]->dspTexture, nullptr,
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
