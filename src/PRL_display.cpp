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

PRL_Displayable :: PRL_Displayable() : dspMainDst(0.0f, 0.0f, 0, 0), dspMainTexture(nullptr),
dspRenderer(handler.display.renderer[0]), dspRotAngle(0.0), dspDisplayerAddress(0), dspDisplayerAdded(false),
dspVelocity(0.0f, 0.0f)
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
	dspMainTexture = texture;
}

PRL_Displayable :: ~PRL_Displayable()
{
    dspCount--;
}

SDL_Texture* PRL_Displayable :: getTexture() const noexcept
{
    return dspMainTexture;
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
    dspMainTexture = texture;
    return 0;
}

void PRL_Displayable :: enableDisplay() noexcept
{
    dspDisplayEnabled = true;
}

void PRL_Displayable :: disableDisplay() noexcept
{
    dspDisplayEnabled = false;
}

bool PRL_Displayable :: isDisplayEnabled() const noexcept
{
    return dspDisplayEnabled;
}

void PRL_Displayable :: setRotAngle(double angle) noexcept
{
    dspRotAngle = angle;
}

double PRL_Displayable :: getRotAngle() const noexcept
{
    return dspRotAngle;
}

void PRL_Displayable :: setPos(float x, float y) noexcept
{
	dspDesiredPos.set(x, y);
    dspMainDst.x = x;
    dspMainDst.y = y;
}

void PRL_Displayable :: setPos(PRL_FPoint const& pos) noexcept
{
	dspDesiredPos = pos;
    dspMainDst.x = pos.x;
    dspMainDst.y = pos.y;
}

void PRL_Displayable :: setCenterPos(float x, float y) noexcept
{
    dspMainDst.x = x - dspMainDst.w/2.0;
    dspMainDst.y = y - dspMainDst.h/2.0;
    dspDesiredPos.set(dspMainDst.x, dspMainDst.y);
}

void PRL_Displayable :: setCenterPos(PRL_FPoint const& pos) noexcept
{
    dspMainDst.x = pos.x - dspMainDst.w/2.0;
    dspMainDst.y = pos.y - dspMainDst.h/2.0;
    dspDesiredPos.set(dspMainDst.x, dspMainDst.y);
}

PRL_FRect const& PRL_Displayable :: getDstRect() const noexcept
{
    return dspMainDst;
}

// Motion
void PRL_Displayable :: enableMotion() noexcept
{
	dspMotionEnabled = true;
}

void PRL_Displayable :: disableMotion() noexcept
{
	dspMotionEnabled = false;
}

bool PRL_Displayable :: isMotionEnabled() const noexcept
{
	return dspMotionEnabled;
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
    displayable.clear();
    dsprCount--;
}

int PRL_Displayer :: add(PRL_Displayable *obj)
{
    if (obj == nullptr)
    {
        PRL_SetError("Invalid object (nullptr)");
        return PRL_ERROR;
    }
    else if (obj->dspDisplayerAdded)
    {
        PRL_SetError("Object already added");
        return PRL_ERROR;
    }

    displayable.push_back(obj);
    obj->dspDisplayerAddress = displayable.size() - 1;
    obj->dspDisplayerAdded = true;
    return 0;
}

int PRL_Displayer :: remove(PRL_Displayable *obj)
{
    if (obj == nullptr)
    {
        PRL_SetError("Invalid object (nullptr)");
        return PRL_ERROR;
    }
    else if (!obj->dspDisplayerAdded)
    {
        PRL_SetError("Invalid object (not added)");
        return PRL_ERROR;
    }

    displayable.erase(displayable.begin() + obj->dspDisplayerAddress);
    obj->dspDisplayerAdded = false;

    for (size_t i(obj->dspDisplayerAddress); i < displayable.size(); i++)
    {
        displayable[i]->dspDisplayerAddress--;
    }
    return 0;
}

int PRL_Displayer :: getAddedNumber() const noexcept
{
    return (int)displayable.size();
}

void rect2FRect(PRL_FRect const& frect, SDL_Rect& rect) noexcept
{
    rect = {(int) floor(frect.x), (int) floor(frect.y), (int) floor(frect.w), (int) floor(frect.h)};
}

int PRL_Displayer :: display() const
{
    SDL_Rect dst;
    for (size_t i(0); i < displayable.size(); i++)
    {
        if (displayable[i]->dspDisplayEnabled)
        {
            rect2FRect(displayable[i]->dspMainDst, dst);
            if (SDL_RenderCopyEx(displayable[i]->dspRenderer, displayable[i]->dspMainTexture, nullptr,
                &dst, displayable[i]->dspRotAngle, NULL, SDL_FLIP_NONE) != 0)
            {
                PRL_SetError(SDL_GetError());
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

    for (size_t i(0); i < displayable.size(); i++)
    {
        if (displayable[i]->dspDisplayEnabled)
        {
            dst.x = zoom * (displayable[i]->dspMainDst.x - camera->getUpLeftCorner().x);
            dst.y = zoom * (displayable[i]->dspMainDst.y - camera->getUpLeftCorner().y);
            dst.w = zoom * displayable[i]->dspMainDst.w;
            dst.h = zoom * displayable[i]->dspMainDst.h;

            if (SDL_RenderCopyEx(displayable[i]->dspRenderer, displayable[i]->dspMainTexture, nullptr,
                &dst, displayable[i]->dspRotAngle, NULL, SDL_FLIP_NONE) != 0)
            {
                PRL_SetError(SDL_GetError());
                return PRL_ERROR;
            }
        }
    }
    return 0;
}

void PRL_Displayer :: setCamera(PRL_Camera *cam)
{
    if (cam != nullptr) camera = cam;
    else PRL_SetError("Invalid camera (nullptr)");
}

int PRL_Displayer :: getDsprCount() noexcept
{
    return dsprCount;
}
