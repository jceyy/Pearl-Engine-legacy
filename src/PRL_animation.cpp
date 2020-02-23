#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "PRL_defines.h"
#include "PRL_global.h"
#include "PRL_animation.h"

#include "PRL_display.h"
#include "PRL_collision.h"


using std::cout;
using std::cerr;
using std::endl;
using std::string;


/* ********************************************* */
/*               PRL_FPointList                  */
/* ********************************************* */
/*
PRL_FPointList :: PRL_FPointList()
{
    ;
}

PRL_FPointList :: ~PRL_FPointList()
{
    points.clear();
}

void PRL_FPointList :: add(PRL_FPoint& point)
{
    points.push_back(point);
    checkRectAroundWithPoint(point);
}

void PRL_FPointList :: add(PRL_Point& point)
{
    PRL_FPoint p({(float) point.x, (float) point.y});
    points.push_back(p);
    checkRectAroundWithPoint(p);
}

void PRL_FPointList :: insert(PRL_FPoint& point, unsigned int index)
{
    points.insert(points.begin() + index, point);
    checkRectAroundWithPoint(point);
}

void PRL_FPointList :: insert(PRL_Point& point, unsigned int index)
{
    PRL_FPoint p({(float) point.x, (float) point.y});
    points.insert(points.begin() + index, p);
    checkRectAroundWithPoint(p);
}

PRL_FPoint const& PRL_FPointList :: get(unsigned int index) const
{
    return points[index];
}

unsigned int PRL_FPointList :: getPointsNumber() const
{
    return points.size();
}

unsigned int PRL_FPointList :: size() const
{
    return points.size();
}

void PRL_FPointList :: remove(int index)
{
    points.erase(points.begin() + index);
    calculateRectAround(); // update the rectangle around if needed
}

void PRL_FPointList :: invert()
{
    std::vector <PRL_FPoint> vector_copy = points;
    for (unsigned int i(0); i<points.size(); i++)
    {
        points[i] = vector_copy[points.size() - 1 - i];
    }
}

void PRL_FPointList :: translateBy(float x, float y)
{
    for (unsigned int i(0); i < points.size(); i++)
    {
        points[i].x += x;
        points[i].y += y;
    }
}

void PRL_FPointList :: checkRectAroundWithPoint(PRL_FPoint& point)
{
    // for x
    if (point.x < rect_arround_hitbox.x) rect_arround_hitbox.x = point.x;
    else if (point.x > rect_arround_hitbox.x + rect_arround_hitbox.w) rect_arround_hitbox.w = point.x - rect_arround_hitbox.x;
    // for y
    if (point.y < rect_arround_hitbox.y) rect_arround_hitbox.y = point.y;
    else if (point.y > rect_arround_hitbox.y + rect_arround_hitbox.h) rect_arround_hitbox.h = point.y - rect_arround_hitbox.y;
}

void PRL_FPointList :: calculateRectAround()
{
    if (points.size() > 0)
    {
        float minx(points[0].x), maxx(points[0].x), miny(points[0].y), maxy(points[0].y);
        for (unsigned int i(1); i < points.size(); i++)
        {
            // for x
            if (points[i].x < minx) minx = points[i].x;
            else if (points[i].x > maxx) maxx = points[i].x;
            // for y
            if (points[i].y < miny) miny = points[i].y;
            else if (points[i].y > maxy) maxy = points[i].y;
        }
        rect_arround_hitbox = {minx, miny, (int) (maxx - minx), (int) (maxy - miny)};
    }
}
PRL_FRect const& PRL_FPointList :: getRectAround() const
{
    return rect_arround_hitbox;
}


* ********************************************* *
*                  PRL_HitBox                   *
* ********************************************* *

PRL_HitBox :: PRL_HitBox() : _type(PRL_HITBOX_RECT)
{
	PRL_FRect frect(0.0, 0.0, 0, 0);
    _rect = frect;
    rect_arround_hitbox = _rect;

    PRL_FCircle circle(0.0, 0.0, 0.0);
    _circle = circle;
}

PRL_HitBox :: ~PRL_HitBox()
{
    ;
}

void PRL_HitBox :: set(PRL_FRect& rect)
{
    _rect = rect;
    rect_arround_hitbox = rect;
    _type = PRL_HITBOX_RECT;
}

void PRL_HitBox :: set(PRL_FCircle& circle)
{
    _circle = circle;
    rect_arround_hitbox = {circle.x - circle.r, circle.y - circle.r, (int) circle.r, (int) circle.r};
    _type = PRL_HITBOX_CIRCLE;
}

void PRL_HitBox :: set(PRL_FPointList& fpointList)
{
    _fpointList = fpointList;
    rect_arround_hitbox = fpointList.getRectAround();
    _type = PRL_HITBOX_POLYGON;
}

PRL_FRect const& PRL_HitBox :: getRect() const
{
    return _rect;
}

PRL_FCircle const& PRL_HitBox :: getCircle() const
{
    return _circle;
}

PRL_FPointList const& PRL_HitBox :: getPolygon() const
{
    return _fpointList;
}

PRL_HitBoxType PRL_HitBox :: getType() const
{
    return _type;
}

PRL_FRect const& PRL_HitBox :: getRectAround() const
{
    return rect_arround_hitbox;
}

void PRL_HitBox :: translateBy(float x, float y)
{
    if (_type == PRL_HITBOX_RECT)
    {
        _rect.x += x;
        _rect.y += y;
    }
    else if (_type == PRL_HITBOX_CIRCLE)
    {
        _circle.x += x;
        _circle.y += y;
    }
    else
    {
        _fpointList.translateBy(x, y);
    }
}


* ********************************************* *
*               PRL_FPointCluster               *
* ********************************************* *

PRL_FPointCluster :: PRL_FPointCluster()
{
    ;
}

PRL_FPointCluster :: ~PRL_FPointCluster()
{
    ;
}

int PRL_FPointCluster :: getFramesNumber() const
{
    return anchorPoint.size();
}

int PRL_FPointCluster :: getActionPointsNumber() const
{
    return actionPoint[0].size();
}

int PRL_FPointCluster :: getHitBoxesNumber() const
{
    return hitBoxe[0].size();
}

PRL_HitBox const& PRL_FPointCluster :: getHitBox(int frame, int hitBoxNumber) const
{
    return (PRL_HitBox const&) hitBoxe[frame][hitBoxNumber];
}

PRL_FPoint const& PRL_FPointCluster :: getActionPoint(int frame, int actionPointNumber) const
{
    return (PRL_FPoint const&) actionPoint[frame][actionPointNumber];
}

PRL_FPoint const& PRL_FPointCluster :: getFixedPoint(int frame) const
{
    return (PRL_FPoint const&) anchorPoint[frame];
}

void PRL_FPointCluster :: setHitBox(int frame, int hitBoxNumber, PRL_HitBox const& hitbox)
{
    hitBoxe[frame][hitBoxNumber] = hitbox;
}

void PRL_FPointCluster :: setActionPoint(int frame, int actionPointNumber, PRL_FPoint const& point)
{
    actionPoint[frame][actionPointNumber] = point;
}

void PRL_FPointCluster :: setAnchorPoint(int frame, PRL_FPoint const& point)
{
    anchorPoint[frame] = point;
}*/


/// ---------------------------------------------------------------------------------------------------------------

// Get non empty line from file
void getline_ne(basic_istream<char>& f, string& line)
{
	getline(f, line);

	while(line == "")
		getline(f, line);
}

/* ********************************************* */
/*               	PRL_Image	                 */
/* ********************************************* */

int PRL_Image :: imageCount = 0;

int PRL_Image :: getCount() noexcept
{
	return imageCount;
}

PRL_Image :: PRL_Image(const std::string& path, SDL_Renderer *_renderer)
{
	// Loading of anim file.
	filePath = path;
	display.renderer = _renderer;
	display.imgSurface.resize(1);
	display.imgTexture.resize(1);
	display.imgTrueSize.resize(1);
	display.imgScaledSize.resize(1);

	if (load_CPU() != 0)
	{
		clear_CPU();
		throw (std::runtime_error(PRL_GetError()));
	}
	if (load_GPU() != 0)
	{
		clear_CPU();
		clear_GPU();
		throw (std::runtime_error(PRL_GetError()));
	}

	imageCount++;
}

PRL_Image :: PRL_Image(const char* path, SDL_Renderer *_renderer)
{
	PRL_Image(std::string(path), _renderer);
}

PRL_Image :: ~PRL_Image()
{
	clear_CPU();
	clear_GPU();
	imageCount--;
}

int PRL_Image :: load_CPU()
{
	std::ifstream file(filePath);

	if (file.is_open())
	{
		string parentDir;
		string line;

		getline(file, line); // First line: [display]
		getline_ne(file, line); // Reference renderer
		PRL_Config :: extractPoint(line, display.refRenderSize);

		if (display.refRenderSize.x == 0 || display.refRenderSize.y == 0)
		{
			PRL_SetError("Invalid reference renderer size: can not be 0");
			return PRL_ERROR;
		}

		getline_ne(file, line); // Main texture name
		PRL_GetParentDir(filePath, parentDir);
		line = parentDir + string("/") + line;
        display.imgSurface[0] = IMG_Load(line.c_str());

        if (display.imgSurface[0] == nullptr)
		{
			PRL_SetError(string("Unable to load surface: ") + string(SDL_GetError()));
			return PRL_ERROR;
		}

		display.scalingRatio.x = 1.0f * handler.config.getRenderResolution().x / display.refRenderSize.x;
		display.scalingRatio.y = 1.0f * handler.config.getRenderResolution().y / display.refRenderSize.y;
		display.imgTrueSize[0].set(display.imgSurface[0]->w, display.imgSurface[0]->h);
		display.imgScaledSize[0].set((int)(display.imgSurface[0]->w * display.scalingRatio.x), (int)(display.imgSurface[0]->h * display.scalingRatio.y));
		cout << "Ok so far\n";
		// Load masks
		PRL_Point sz;
		int current_mask(0);
		SDL_Surface* surface(nullptr);

		getline_ne(file, line); // [mask]

		string maskPath;
		while (false && line != "[hitbox-rect]")
		{
			if (line != string(""))
			{
				PRL_GetPath(filePath, parentDir, maskPath, maskPath); // mask path not recovering any useful information
				maskPath = parentDir + "/" + line;
				surface = IMG_Load(maskPath.c_str());

				if (surface == nullptr)
				{
					PRL_SetError(string("Unable to load mask ") + to_string(current_mask) + string(": ") + string(SDL_GetError()));
					clear_CPU();
					return PRL_ERROR;

				}
				display.imgSurface.push_back(surface);

				// Update true and scaled sizes
				sz.set(surface->w, surface->h);
				display.imgTrueSize.push_back(sz);
				sz.set((int)(surface->w * display.scalingRatio.x), (int)(surface->h * display.scalingRatio.y));
				display.imgScaledSize.push_back(sz);

				// Load local pos!

				surface = nullptr;
				current_mask++;
			}
			getline(file, line);
		}

		// resize all the vectors

		if (display.imgTrueSize.size() != 0)
		{
			display.imgSurface.resize(display.imgTrueSize.size());
			display.imgTexture.resize(display.imgTrueSize.size());
			display.imgScaledSize.resize(display.imgTrueSize.size());
			display.imgLocalPos.resize(display.imgTrueSize.size() - 1);
		}

		// Points: still to do

		file.close();
	}
	else
	{
		PRL_SetError("File could not be opened");
		return PRL_ERROR;
	}
	return 0;
}

int PRL_Image :: load_GPU()
{
	display.imgTexture[0] = SDL_CreateTextureFromSurface(display.renderer, display.imgSurface[0]);
	//SDL_Texture *mask(nullptr);

	if (display.imgTexture[0] == nullptr)
	{
		PRL_SetError(string("Unable to load main texture: ") + string(SDL_GetError()));
		return PRL_ERROR;
	}

	for (size_t i(0); i < display.imgTexture.size(); ++i)
	{
        ;
	}

	return 0;
}

void PRL_Image :: clear_CPU()
{
	for (size_t i(0); i < display.imgSurface.size(); ++i)
	{
		SDL_FreeSurface(display.imgSurface[i]);
		display.imgSurface[i] = nullptr;
	}
	display.imgSurface.clear();
	display.imgTrueSize.clear();
	display.imgScaledSize.clear();
}

void PRL_Image :: clear_GPU()
{
	for (size_t i(0); i < display.imgTexture.size(); ++i)
	{
		SDL_DestroyTexture(display.imgTexture[i]);
		display.imgTexture[i] = nullptr;
	}
	display.imgTexture.clear();
}

SDL_Texture* PRL_Image :: _display :: getTexture() const
{
	return imgTexture[0];
}

const PRL_Point& PRL_Image :: _display :: getSize() const
{
	return imgScaledSize[0];
}

const PRL_Point& PRL_Image :: _display :: getTrueSize() const
{
	return imgTrueSize[0];
}

const PRL_Point& PRL_Image :: _display :: getRefRenderSize() const
{
	return refRenderSize;
}

void PRL_Image :: addTargeting()
{
	targetingCount++;
}

void PRL_Image :: removeTargeting()
{
	targetingCount--;
}

int PRL_Image :: _display :: getMaskNumber() const
{
	return (int) imgTrueSize.size() - 1;
}

SDL_Renderer* PRL_Image :: _display :: getRenderer() const
{
	return renderer;
}

void PRL_Image :: _display :: get(std::vector<SDL_Texture*>& imgTex,
								std::vector<PRL_FRect>& imgDst) const
{
	imgTex = imgTexture;

	for (size_t i(0); i < imgScaledSize.size(); ++i)
	{
		imgDst[i].w = imgScaledSize[i].x;
		imgDst[i].h = imgScaledSize[i].y;
	}
}

/* ********************************************* */
/*            _PRL_ImageAccessor 	             */
/* ********************************************* */

_PRL_ImageAccessor :: _PRL_ImageAccessor()
{
	;
}

_PRL_ImageAccessor :: ~_PRL_ImageAccessor()
{
	;
}

void _PRL_ImageAccessor :: addTargeting(PRL_Image* img) const
{
	img->addTargeting();
}

void _PRL_ImageAccessor :: removeTargeting(PRL_Image* img) const
{
	img->removeTargeting();
}

int PRL_Sprite :: spriteCount = 0;

int PRL_Sprite :: getCount() noexcept
{
	return spriteCount;
}

PRL_Sprite :: PRL_Sprite() : targetImage(nullptr)
{
	spriteCount++;
}


PRL_Sprite :: PRL_Sprite(PRL_Image* image)
{
	if (setImage(image) != 0)
	{
		throw (std::runtime_error(PRL_GetError()));
	}
	spriteCount++;
}

PRL_Sprite :: ~PRL_Sprite()
{
	spriteCount--;
}


_PRL_ImageAccessor image_accessor;

int PRL_Sprite :: setImage(PRL_Image* image)
{
	if (image == nullptr)
	{
		PRL_SetError("Invalid image");
		return PRL_ERROR;
	}

	if (targetImage != nullptr)
	{
		image_accessor.removeTargeting(image); // Remove previous targeting
	}

	image_accessor.addTargeting(image); // Add targeting
	targetImage = image;
	image->display.get(dspTexture, dspDst);

	// Temporary
	colHitbox.clear();
	PRL_FRect frect(0.0f, 0.0f, image->display.getSize().x, image->display.getSize().y);
    PRL_HBRect* hitbox = new PRL_HBRect(frect);
	colHitbox.push_back(hitbox);

	return 0;
}



/* ********************************************* */
/*               PRL_Animation                   */
/* ********************************************* */

int PRL_Animation :: animationCount = 0;

int PRL_Animation :: getCount()
{
	return animationCount;
}

PRL_Animation :: PRL_Animation(const std::string& path, SDL_Renderer *_renderer)
{
	// Loading of anim file.
	filePath = path;
	display.renderer = _renderer;

	if (load_CPU() != 0)
		throw (PRL_GetError());
	if (load_GPU() != 0)
		throw (PRL_GetError());

	animationCount++;
}

PRL_Animation :: PRL_Animation(const char* path, SDL_Renderer *_renderer)
{
	PRL_Animation(std::string(path), _renderer);
}

PRL_Animation :: ~PRL_Animation()
{
	clear_CPU();
	clear_GPU();
	animationCount--;

	for (size_t i(0); i < collision.mainHitbox.size(); ++i)
	{
        delete collision.mainHitbox[i];

        for (size_t j(0); j < collision.mainHitbox.size(); ++j)
		{
			delete collision.maskHitbox[i][j];
		}
	}
}

int PRL_Animation :: load_CPU()
{
	std::ifstream file(filePath);

	if (file.is_open())
	{
		PRL_Point temp;
		std::string parentDir;
		std::string line, dump;
		SDL_Surface* surface;
		getline(file, line); // first line: [display]
		getline(file, line);
		display.frameRate = stof(line); // FPS
		getline(file, line);
		PRL_Config :: extractPoint(line, display.refRenderSize); // Reference renderer

		getline(file, line);
		while (line != "[mask]")
		{
			if (line != std::string(""))
			{
				PRL_GetParentDir(filePath, parentDir);
				dump = parentDir + "/" + line;
				surface = IMG_Load(dump.c_str());

				if (surface == nullptr)
				{
					PRL_SetError(std::string("Unable to load main surface: ") + std::string(SDL_GetError()));
					clear_CPU();
					return PRL_ERROR;
				}
				display.mainSurface.push_back(surface);
				temp.x = surface->w;
				temp.y = surface->h;
				display.mainTrueTextureSize.push_back(temp);

				surface = nullptr;
			}
			getline(file, line);
		}

		// resize all the vectors
		size_t framesNb(display.mainSurface.size());
		display.mainTexture.resize(framesNb);
		display.mainScaledTextureSize.resize(framesNb);

		if (display.maskPerTexture != 0)
		{
			display.maskSurface.resize(framesNb);
			display.maskTexture.resize(framesNb);
			display.maskScaledTextureSize.resize(framesNb);
			display.maskLocalPos.resize(framesNb);
		}

		getline(file, line);
		display.maskPerTexture = (size_t) stoi(line);

		// Load masks
		if (display.maskPerTexture != 0)
		{
			getline(file, line);

			for (size_t i(0); i < framesNb; ++i)
			{
				display.maskSurface[i].resize(display.maskPerTexture);
				for (size_t j(0); j < display.maskPerTexture; ++j)
				{
					PRL_GetPath(filePath, parentDir, dump, dump);
					dump = parentDir + "/" + line;
					display.maskSurface[i][j] = IMG_Load(dump.c_str());
					if (display.maskSurface[i][j] == nullptr)
					{
						PRL_SetError(std::string("Unable to load mask surface: ") + std::string(SDL_GetError()));
						clear_CPU();
						return PRL_ERROR;
					}
					temp.x = display.maskSurface[i][j]->w;
					temp.y = display.maskSurface[i][j]->h;
					display.maskTrueTextureSize[i][j] = temp;
				}
				getline(file, line);
			}
		}

		while (line != "[collision]")
		{
			getline(file, line);
		}
        getline(file, line);

        if (stoi(line) == -1) // no use of collision, default hit box is entire texture
		{
			PRL_FRect local_rect; PRL_FPoint local_p(0.0f, 0.0f);
			for (size_t i(0); false && i < framesNb; ++i)
			{
				// Main hit boxes
				local_rect.set(local_p, display.mainScaledTextureSize[i]);
				PRL_HBRect* hb_r = new PRL_HBRect(local_rect);
				if (hb_r == nullptr)
				{
					PRL_SetError("Unable to create main hit box", false);
					return PRL_ERROR;
				}
				collision.mainHitbox.push_back(hb_r);
				collision.maskHitbox.resize(framesNb);

				// Mask hit boxes
				for (size_t j(0); j < display.maskSurface[i].size(); ++j)
				{
					local_rect.set(local_p, display.mainScaledTextureSize[i]);
					PRL_HBRect* hb_r_m = new PRL_HBRect(local_rect);
					if (hb_r_m == nullptr)
					{
						PRL_SetError("Unable to create mask hit box", false);
						return PRL_ERROR;
					}
					collision.maskHitbox[i].push_back(hb_r_m);
				}
			}
		}
		else
		{
			PRL_SetError(string("-- Pearl Beta -- Collision should be set to -1 in file: ") + filePath, true);
		}
		// points

		// TEMP: anchor file
		display.anchorPoint.clear();
		for (size_t i(0); i < framesNb; ++i)
		{
			display.anchorPoint.push_back(PRL_Point(0, 0));
		}

		file.close();
	}
	else
	{
		PRL_SetError("File could not be opened");
		return PRL_ERROR;
	}
	return 0;
}

int PRL_Animation :: load_GPU()
{
	PRL_Point renderSize = handler.config.getRenderResolution();
	display.scalingRatio = {1.0f * renderSize.x / display.refRenderSize.x, 1.0f * renderSize.y / display.refRenderSize.y};

	SDL_Texture *texture(nullptr), *targetTexture(nullptr);
	PRL_Point scaledSize;

    for (size_t i(0); i < display.mainTexture.size(); ++i)
	{
		texture = SDL_CreateTextureFromSurface(display.renderer, display.mainSurface[i]);

		if (display.scalingRatio.x * display.mainTrueTextureSize[i].x + display.scalingRatio.y * display.mainTrueTextureSize[i].y <
			display.mainTrueTextureSize[i].x * display.mainTrueTextureSize[i].y) // if less pixels could be stored
		{
			scaledSize = {(int) display.scalingRatio.x * display.mainTrueTextureSize[i].x, (int) display.scalingRatio.y * display.mainTrueTextureSize[i].y};
			targetTexture = SDL_CreateTexture(display.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, scaledSize.x, scaledSize.y);
		}

		else
		{
			scaledSize = display.mainTrueTextureSize[i];
			targetTexture = SDL_CreateTexture(display.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, scaledSize.x, scaledSize.y);
		}

        if (targetTexture == nullptr || texture == nullptr)
		{
			PRL_SetError(std::string("Unable to create main texture: ") + std::string(SDL_GetError()));
			clear_CPU();
			clear_GPU();
			return PRL_ERROR;
		}

		display.mainScaledTextureSize[i] = scaledSize;
		SDL_SetRenderTarget(display.renderer, targetTexture);
        SDL_RenderCopy(display.renderer, texture, nullptr, nullptr);
		SDL_SetRenderTarget(display.renderer, nullptr);
		display.mainTexture[i] = targetTexture;

		if (display.maskPerTexture != 0)
		{
			display.maskTexture[i].resize(display.maskPerTexture);
			for (size_t j(0); j < display.maskPerTexture; ++j)
			{
				display.maskTexture[i][j] = SDL_CreateTextureFromSurface(display.renderer, display.maskSurface[i][j]);
				if (display.maskTexture[i][j] == nullptr)
				{
					PRL_SetError(std::string("Unable to create mask texture: ") + std::string(SDL_GetError()));
					clear_CPU();
					clear_GPU();
					return PRL_ERROR;
				}
			}
		}
		texture = nullptr;
		targetTexture = nullptr;
	}
	clear_CPU();
	return 0;
}

void PRL_Animation :: clear_CPU()
{
	for (size_t i(0); i < display.mainSurface.size(); ++i)
	{
		SDL_FreeSurface(display.mainSurface[i]);
		display.mainSurface[i] = nullptr;

        if (display.maskPerTexture != 0)
		{
			for (size_t j(0); j < display.maskPerTexture; ++j)
			{
				SDL_FreeSurface(display.maskSurface[i][j]);
				display.maskSurface[i][j] = nullptr;
			}
		}
	}
}

void PRL_Animation :: clear_GPU()
{
	for (size_t i(0); i < display.mainTexture.size(); ++i)
	{
		SDL_DestroyTexture(display.mainTexture[i]);

        if (display.maskPerTexture != 0)
		{
			for (size_t j(0); j < display.maskPerTexture; ++j)
			{
				SDL_DestroyTexture(display.maskTexture[i][j]);
			}
		}
	}
}

SDL_Texture* PRL_Animation :: _display :: getTexture(size_t which) const
{
	return mainTexture[which];
}

const PRL_Point& PRL_Animation :: _display :: getSize(size_t which) const
{
	return mainScaledTextureSize[which];
}

const PRL_Point& PRL_Animation :: _display :: getRefRenderSize() const
{
	return refRenderSize;
}

int PRL_Animation :: _display :: getFramesNumber() const
{
	return (int) mainTexture.size();
}

float PRL_Animation :: _display :: getFPS() const
{
	return frameRate;
}

void PRL_Animation :: addTarget()
{
	targetCount++;
}

void PRL_Animation :: removeTarget()
{
	targetCount--;
}

int PRL_Animation :: _display :: getMaskNumber() const
{
	return (int) maskPerTexture;
}

SDL_Renderer* PRL_Animation :: _display :: getRenderer() const
{
	return renderer;
}

PRL_Point const& PRL_Animation :: _display :: getAnchor(size_t which) const noexcept
{
	return anchorPoint[which];
}

/* ********************************************* */
/*            _PRL_AnimationAccessor             */
/* ********************************************* */

_PRL_AnimationAccessor :: _PRL_AnimationAccessor()
{
	;
}

_PRL_AnimationAccessor :: ~_PRL_AnimationAccessor()
{
	;
}

void _PRL_AnimationAccessor :: addTarget(PRL_Animation* anim) const
{
	anim->addTarget();
}

void _PRL_AnimationAccessor :: removeTarget(PRL_Animation* anim) const
{
	anim->removeTarget();
}

/* ********************************************* */
/*            		PRL_Animated                 */
/* ********************************************* */

int PRL_Animated :: animatedCount = 0;

int PRL_Animated :: getCount() noexcept
{
	return animatedCount;
}

_PRL_AnimationAccessor accessor_animated;

PRL_Animated :: PRL_Animated() : targetAnimation(nullptr), timeCurrent(0), timePrevUpdate(0),
currentFrame(0), repeatCount(0), started(false)
{
	animatedCount++;
}

PRL_Animated :: PRL_Animated(PRL_Animation* anim) : targetAnimation(nullptr), timeCurrent(0),
timePrevUpdate(0), currentFrame(0), repeatCount(0), started(false)
{
	setAnim(anim);
	animatedCount++;
}

PRL_Animated :: ~PRL_Animated()
{
	animatedCount--;
}

void PRL_Animated :: setAnim(PRL_Animation* anim)
{
	if (anim == nullptr)
	{
		PRL_SetError("Invalid animation (nullptr)", true);
	}

	accessor_animated.addTarget(anim);
	if (targetAnimation != nullptr) // if previous target animation existed
		accessor_animated.removeTarget(targetAnimation);

	targetAnimation = anim;
	repeatCount = 0;
	currentFrame = 0;
	started = false;

	// Displayable part
	dspRenderer = anim->display.getRenderer();
	updateDisplayable();

	// Temporary
	colHitbox.clear();
	PRL_FRect frect(0.0f, 0.0f, anim->display.getSize(0).x, anim->display.getSize(0).y);
	PRL_HBRect* hitbox = new PRL_HBRect(frect);
	colHitbox.push_back(hitbox);

}

void PRL_Animated :: start() noexcept
{
	started = true;
	timePrevUpdate = handler.time.getTimeUpdated();
}

void PRL_Animated :: restart() noexcept
{
	started = true;
	timePrevUpdate = handler.time.getTimeUpdated();
    currentFrame = 0;
}

void PRL_Animated :: stop() noexcept
{
	started = false;
}

bool PRL_Animated :: isStarted() const noexcept
{
	return started;
}

int PRL_Animated :: getCurrentFrame() const noexcept
{
	return currentFrame;
}

int PRL_Animated :: getRepeatCount() const noexcept
{
	return repeatCount;
}

void PRL_Animated :: update()
{
	if (handler.time.timeUpdated - timePrevUpdate >= (long long) (1000000 / targetAnimation->display.getFPS()))
	{
		timePrevUpdate = handler.time.timeUpdated;

		if (currentFrame < targetAnimation->display.getFramesNumber() - 1)
		{
			currentFrame++;
		}
		else
		{
			currentFrame = 0;
			repeatCount++;
		}
		updateDisplayable();
	}
}

PRL_Point temp_anchor;
void PRL_Animated :: updateDisplayable()
{
	temp_anchor = targetAnimation->display.getAnchor(currentFrame);
	dspTexture[0] = targetAnimation->display.getTexture(currentFrame);
	dspDst[0].x = dspDesiredPos.x - temp_anchor.x;
	dspDst[0].y = dspDesiredPos.y - temp_anchor.y;
	dspDst[0].w = targetAnimation->display.getSize(currentFrame).x;
	dspDst[0].h = targetAnimation->display.getSize(currentFrame).y;
}

