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

int PRL_Displayable :: getCount() noexcept
{
    return dspCount;
}

PRL_Displayable :: PRL_Displayable() : //dspMainDst(0.0f, 0.0f, 0, 0), dspMainTexture(nullptr),
dspRenderer(handler.display.renderer[0]), dspRotAngle(0.0), dspDisplayerAddress(0), dspDisplayerAdded(false),
dspVelocity(0.0f, 0.0f)
{
	PRL_FRect dst = {0.0f, 0.0f, 0, 0};
	SDL_Texture *texture(nullptr);
	dspTexture.push_back(texture);
	dspDst.push_back(dst);
    dspCount++;
}

PRL_Displayable :: PRL_Displayable(SDL_Texture *texture, SDL_Renderer *renderer):
dspRenderer(handler.display.renderer[0]), dspRotAngle(0.0), dspDisplayerAddress(0), dspDisplayerAdded(false),
dspVelocity(0.0f, 0.0f)
{
	PRL_FRect dst = {0.0f, 0.0f, 0, 0};
	dspDst.push_back(dst);
    dspCount++;

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
	dspTexture.push_back(texture);
}

PRL_Displayable :: ~PRL_Displayable()
{
    dspCount--;
}

SDL_Texture* PRL_Displayable :: getTexture() const noexcept
{
    return dspTexture[0];
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
    dspTexture[0] = texture;
    return 0;
}

void PRL_Displayable :: show() noexcept
{
    dspEnabled = true;
}

void PRL_Displayable :: hide() noexcept
{
    dspEnabled = false;
}

bool PRL_Displayable :: isDisplayEnabled() const noexcept
{
    return dspEnabled;
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
    dspDst[0].x = x;
    dspDst[0].y = y;
}

void PRL_Displayable :: setPos(PRL_FPoint const& pos) noexcept
{
	dspDesiredPos = pos;
    dspDst[0].x = pos.x;
    dspDst[0].y = pos.y;
}

void PRL_Displayable :: setCenterPos(float x, float y) noexcept
{
    dspDst[0].x = x - dspDst[0].w/2.0;
    dspDst[0].y = y - dspDst[0].h/2.0;
    dspDesiredPos.set(dspDst[0].x, dspDst[0].y);
}

void PRL_Displayable :: setCenterPos(PRL_FPoint const& pos) noexcept
{
    dspDst[0].x = pos.x - dspDst[0].w/2.0;
    dspDst[0].y = pos.y - dspDst[0].h/2.0;
    dspDesiredPos.set(dspDst[0].x, dspDst[0].y);
}

PRL_FRect const& PRL_Displayable :: getDstRect() const noexcept
{
    return dspDst[0];
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
        if (displayable[i]->dspEnabled)
        {
            rect2FRect(displayable[i]->dspDst[0], dst);
            if (SDL_RenderCopyEx(displayable[i]->dspRenderer, displayable[i]->dspTexture[0], nullptr,
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
        if (displayable[i]->dspEnabled)
        {
            dst.x = zoom * (displayable[i]->dspDst[0].x - camera->getUpLeftCorner().x);
            dst.y = zoom * (displayable[i]->dspDst[0].y - camera->getUpLeftCorner().y);
            dst.w = zoom * displayable[i]->dspDst[0].w;
            dst.h = zoom * displayable[i]->dspDst[0].h;

            if (SDL_RenderCopyEx(displayable[i]->dspRenderer, displayable[i]->dspTexture[0], nullptr,
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
