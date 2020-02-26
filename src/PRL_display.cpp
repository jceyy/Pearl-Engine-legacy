#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "PRL_display.h"
#include "PRL_global.h"

using std::cout;
using std::cerr;
using std::endl;


/* ********************************************* */
/*              PRL_Displayable                  */
/* ********************************************* */

int PRL_Displayable :: dspCount = 0;

int PRL_Displayable :: getCount() noexcept
{
    return dspCount;
}

void _createBoxTexture(std::vector <SDL_Texture*>& createdTexture, SDL_Color& color1, SDL_Color& color2)
{
    createdTexture.clear();
    createdTexture.resize(2);

    for (size_t i(0); i < createdTexture.size(); ++i)
    {
        createdTexture[i] = nullptr;
        createdTexture[i] = SDL_CreateTexture(handler.display.renderer[0], SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
        if (createdTexture[i] == nullptr || SDL_SetTextureBlendMode(createdTexture[i], SDL_BLENDMODE_BLEND) != 0)
        {
            PRL_SetError(std::string("Error creating displayable: ") + std::string(SDL_GetError()), true);
        }
    }
    color1 = {192, 32, 0, 128}, color2 = {32, 192, 0, 128};
}

PRL_Displayable :: PRL_Displayable() :
dspRenderer(handler.display.renderer[0]), dspRotAngle(0.0), colIsColliding(false),
colType(PRL_COLTYPE_0), colGroup(PRL_COLTYPE_0), colPriority(0), dspShowBox(false), dspBoxTextureIndex(0),
dspBoxWidth(20), dspRefRenderRes(handler.config.renderResolution), dspDisplayerAddress(0),
dspDisplayerAdded(false), colColliderAddress(-1), dspVelocity(0.0f, 0.0f)
{
	PRL_FRect dst = {0.0f, 0.0f, 0, 0};
	SDL_Texture *texture(nullptr);
	dspTexture.push_back(texture);
	dspDst.push_back(dst);

	SDL_Color color1, color2;
	_createBoxTexture(dspBoxTexture, color1, color2);
	if (setBoxColor(color1, color2) != 0)
    {
        std::runtime_error e(PRL_GetError());
        throw e;
    }

    dspCount++;
}

PRL_Displayable :: PRL_Displayable(SDL_Texture *texture, SDL_Renderer *renderer):
dspRenderer(handler.display.renderer[0]), dspRotAngle(0.0), colIsColliding(false),
colType(PRL_COLTYPE_0), colGroup(PRL_COLTYPE_0), colPriority(0), dspShowBox(false), dspBoxTextureIndex(0),
dspBoxWidth(20), dspRefRenderRes(handler.config.renderResolution), dspDisplayerAddress(0),
dspDisplayerAdded(false), colColliderAddress(-1), dspVelocity(0.0f, 0.0f)
{
	PRL_FRect dst = {0.0f, 0.0f, 0, 0};
	dspDst.push_back(dst);

	if (texture == nullptr)
	{
		PRL_SetError("Invalid texture", true);
	}
	if (renderer == nullptr)
	{
		PRL_SetError("Invalid renderer", true);
	}

	dspRenderer = renderer;
	dspTexture.push_back(texture);

	SDL_Color color1, color2;
	_createBoxTexture(dspBoxTexture, color1, color2);
	if (setBoxColor(color1, color2) != 0)
    {
        std::runtime_error e(PRL_GetError());
        throw e;
    }

    dspCount++;
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

void PRL_Displayable :: toggleDisplay() noexcept
{
    dspEnabled = !dspEnabled;
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


void PRL_Displayable :: showBox() noexcept
{
    dspShowBox = true;
}

void PRL_Displayable :: hideBox() noexcept
{
    dspShowBox = false;
}

void PRL_Displayable :: toggleBoxDisplay() noexcept
{
    dspShowBox = !dspShowBox;
}

bool PRL_Displayable :: isBoxShown() const noexcept
{
    return dspShowBox;
}

int PRL_Displayable :: setBoxColor(SDL_Color const& color1, SDL_Color const& color2)
{
    SDL_Renderer *renderer = handler.display.renderer[0];
    SDL_Texture* renderTarget = SDL_GetRenderTarget(renderer);
    SDL_Color drawColor;

    if (SDL_GetRenderDrawColor(renderer, &drawColor.r, &drawColor.g, &drawColor.b, &drawColor.a) != 0)
    {
        PRL_SetError(SDL_GetError());
        return PRL_ERROR;
    }
    if (SDL_SetRenderTarget(renderer, dspBoxTexture[0]) != 0)
    {
        PRL_SetError(SDL_GetError());
        return PRL_ERROR;
    }
    if (SDL_SetRenderDrawColor(renderer, color1.r, color1.g, color1.b, color1.a) != 0)
    {
        PRL_SetError(SDL_GetError());
        return PRL_ERROR;
    }
    SDL_RenderClear(renderer);

    if (SDL_SetRenderTarget(renderer, dspBoxTexture[1]) != 0)
    {
        PRL_SetError(SDL_GetError());
        return PRL_ERROR;
    }
    if (SDL_SetRenderDrawColor(renderer, color2.r, color2.g, color2.b, color2.a) != 0)
    {
        PRL_SetError(SDL_GetError());
        return PRL_ERROR;
    }
    SDL_RenderClear(renderer);

    if (SDL_SetRenderTarget(renderer, renderTarget) != 0)
    {
        PRL_SetError(SDL_GetError());
        return PRL_ERROR;
    }
    if (SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a) != 0)
    {
        PRL_SetError(SDL_GetError());
        return PRL_ERROR;
    }
    return 0;
}

void PRL_Displayable :: setBoxWidth(int width, PRL_Point refRend) noexcept
{
    if (width > 0 && refRend.x > 0 && refRend.y > 0)
    {
        dspRefRenderRes = refRend;
        dspBoxWidth = width;
    }
}

/// TEMPORARY, ONLY FOR DEBUG PURPOSES
void PRL_Displayable :: tempSetIndex(size_t index)
{
    dspBoxTextureIndex = index;
}

// Collision
void PRL_Displayable :: enableCol() noexcept
{
	colEnabled = true;
}

void PRL_Displayable :: disableCol() noexcept
{
	colEnabled = false;
}

bool PRL_Displayable :: isColEnabled() const noexcept
{
	return colEnabled;
}

bool PRL_Displayable :: isColliding() const noexcept
{
	return colIsColliding;
}

void PRL_Displayable :: setColType(PRL_ColType type) noexcept
{
    colType = type;
}

void PRL_Displayable :: setColGroup(PRL_ColGroup group) noexcept
{
    colGroup = group;
}

void PRL_Displayable :: setColPriority(int p) noexcept
{
	colPriority = p;
}

int PRL_Displayable :: addHitBox(PRL_HitBox* hb)
{
	if (hb == nullptr)
	{
		PRL_SetError("Invalid hitbox");
		return PRL_ERROR;
	}
    else
	{
		colHitbox.push_back(hb);
		return 0;
	}
}

void PRL_Displayable :: addHitPoint(PRL_FPoint const& p)
{
    colHitPoint.push_back(p);
}



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

int PRL_Displayer :: getCount() noexcept
{
    return dsprCount;
}

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

void frect2Rect(PRL_FRect const& frect, SDL_Rect& rect) noexcept
{
    rect = {(int) floor(frect.x), (int) floor(frect.y), (int) floor(frect.w), (int) floor(frect.h)};
}

void PRL_Displayer :: computeBoxDest(const SDL_Rect* dst, SDL_Rect dstbox[], const PRL_Point* scaledWidth) const noexcept
{
    // Up bar
    dstbox[0].x = dst->x - scaledWidth->x;
    dstbox[0].y = dst->y - scaledWidth->y;
    dstbox[0].w = dst->w + 2 * scaledWidth->x;
    dstbox[0].h = scaledWidth->y;

    // Right bar
    dstbox[1].x = dst->x + dst->w;
    dstbox[1].y = dst->y;
    dstbox[1].w = scaledWidth->x;
    dstbox[1].h = dst->h;

    // Down bar
    dstbox[2].x = dst->x - scaledWidth->x;
    dstbox[2].y = dst->y + dst->h;
    dstbox[2].w = dst->w + 2 * scaledWidth->x;
    dstbox[2].h = scaledWidth->y;

    // Left bar
    dstbox[3].x = dst->x - scaledWidth->x;
    dstbox[3].y = dst->y;
    dstbox[3].w = scaledWidth->x;
    dstbox[3].h = dst->h;
}

int PRL_Displayer :: display(bool enableBoxes) const
{
    SDL_Rect dst;
    if (enableBoxes)
    {
        for (size_t i(0); i < displayable.size(); i++)
        {
            if (displayable[i]->dspEnabled)
            {
                frect2Rect(displayable[i]->dspDst[0], dst);
                if (SDL_RenderCopyEx(displayable[i]->dspRenderer, displayable[i]->dspTexture[0], nullptr,
                    &dst, displayable[i]->dspRotAngle, NULL, SDL_FLIP_NONE) != 0)
                {
                    PRL_SetError(SDL_GetError());
                    return PRL_ERROR;
                }

                if (displayable[i]->dspShowBox)
                {
                    SDL_Rect dstbox[4];
                    PRL_Point scaledWidth(displayable[i]->dspBoxWidth * handler.config.renderResolution.x / displayable[i]->dspRefRenderRes.x,
                                          displayable[i]->dspBoxWidth * handler.config.renderResolution.y / displayable[i]->dspRefRenderRes.y);

                    computeBoxDest((const SDL_Rect*) &dst, dstbox, (const PRL_Point*) &scaledWidth);

                    for (size_t j(0); j < 4; ++j)
                    {
                        if(SDL_RenderCopy(displayable[i]->dspRenderer, displayable[i]->dspBoxTexture[displayable[i]->dspBoxTextureIndex],
                                          nullptr, &dstbox[j]) != 0)
                        {
                            PRL_SetError(string("Error displaying a box: ") + string(SDL_GetError()));
                            return PRL_ERROR;
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (size_t i(0); i < displayable.size(); i++)
        {
            if (displayable[i]->dspEnabled)
            {
                frect2Rect(displayable[i]->dspDst[0], dst);
                if (SDL_RenderCopyEx(displayable[i]->dspRenderer, displayable[i]->dspTexture[0], nullptr,
                    &dst, displayable[i]->dspRotAngle, NULL, SDL_FLIP_NONE) != 0)
                {
                    PRL_SetError(SDL_GetError());
                    return PRL_ERROR;
                }
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

