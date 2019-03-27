#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "PRL_defines.h"
#include "PRL_global_variables.h"
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


/* ********************************************* */
/*                  PRL_HitBox                   */
/* ********************************************* */

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


/* ********************************************* */
/*               PRL_FPointCluster               */
/* ********************************************* */

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
}

/* ********************************************* */
/*               PRL_Animation                   */
/* ********************************************* */

int PRL_Animation :: animationsCount = 0;

int PRL_Animation :: getCount()
{
	return animationsCount;
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

	animationsCount++;
}

PRL_Animation :: PRL_Animation(const char* path, SDL_Renderer *_renderer)
{
	PRL_Animation(std::string(path), _renderer);
}

PRL_Animation :: ~PRL_Animation()
{
	clear_CPU();
	clear_GPU();
	animationsCount--;
}

int PRL_Animation :: load_CPU()
{
	std::ifstream file(filePath);

	if (file.is_open())
	{
		PRL_Point temp;
		std::string parentFolder;
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
				PRL_GetPath(filePath, parentFolder, dump, dump);
				dump = parentFolder + "/" + line;
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
					PRL_GetPath(filePath, parentFolder, dump, dump);
					dump = parentFolder + "/" + line;
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

int PRL_Animation :: load_GPU()
{
	PRL_Point renderSize = handler.config.getRenderResolution();
	display.scalingRatio = {1.0 * renderSize.x / display.refRenderSize.x, 1.0 * renderSize.y / display.refRenderSize.y};

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

_PRL_AnimationAccessor accessor_animated;

PRL_Animated :: PRL_Animated() : targetAnimation(nullptr), timePrevUpdate(0), timeCurrent(0),
currentFrame(0), repeatCount(0), started(false)
{
	;
}

PRL_Animated :: ~PRL_Animated()
{
	;
}

int PRL_Animated :: setAnim(PRL_Animation* anim)
{
	if (anim == nullptr)
	{
		SDL_SetError("Invalid animation");
		return PRL_ERROR;
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

	return 0;
}

void PRL_Animated :: start()
{
	started = true;
	timePrevUpdate = handler.time.getTimeUpdated();
}

void PRL_Animated :: restart()
{
	started = true;
	timePrevUpdate = handler.time.getTimeUpdated();
    currentFrame = 0;
}

void PRL_Animated :: stop()
{
	started = false;
}

bool PRL_Animated :: isStarted() const
{
	return started;
}

int PRL_Animated :: getCurrentFrame() const
{
	return currentFrame;
}

int PRL_Animated :: getRepeatCount() const
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

void PRL_Animated :: updateDisplayable()
{
	dspTexture = targetAnimation->display.getTexture(currentFrame);
	dspSrc.w = targetAnimation->display.getSize(currentFrame).x;
	dspSrc.h = targetAnimation->display.getSize(currentFrame).y;
	dspDst.w = dspSrc.w;
	dspDst.h = dspSrc.h;
}


// Old implementation

/*


// Parameters for char strings names
const int FILENAME_LENGTH(400), FORMAT_LENGTH(20);

PRL_AnimationSimple :: PRL_AnimationSimple() : framesNumber(0), frameRate(0), loaded(false), rect({0}),
srcRect({0}), reference_renderer({0}), renderer(renderer_GLOBAL[0])
{
    texture.clear();
    dstSizes.clear();
    srcSizes.clear();
}

PRL_AnimationSimple :: ~PRL_AnimationSimple()
{
    clear();
}

void PRL_AnimationSimple :: clear()
{
    size_t sz = texture.size();

    for (size_t i(0); i < sz; ++i)
        if (texture[i] != nullptr)
            SDL_DestroyTexture(texture[i]);

    texture.clear();
    dstSizes.clear();
    srcSizes.clear();
}

int PRL_AnimationSimple :: load(const char file_path[])
{
    string str = file_path;
    return load(str);
}

int PRL_AnimationSimple :: load(string& file) /// To DO: interpolation + downscale
{
    if (loaded) // if already loaded, free the memory
    {
        for (int i(0); i<framesNumber; i++)
        {
            SDL_DestroyTexture(texture[i]);
        }
    }

    int firstFrameNumber(0), lastFrameNumber(0), ret(0);
    string  fileName(""), fileFormat(""), filePath("");
    string loadingName("");
    SDL_Surface *surface(nullptr);

    FILE* filep = fopen(file.c_str(), "r");

    PRL_GetPath(file, filePath, fileName, fileFormat);

    if (fileName.size() == 0 || fileFormat.size() == 0)
    {
        fclose(filep);
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_AnimationSimple, the provided file has no format or name: " << file << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Provided file incorrect: ") + file);
        return PRL_ERROR;
    }

    if (filep == nullptr)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_AnimationSimple, could not open " << file << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Could not open ") + file);
        return PRL_ERROR;
    }

    char c(0);
    int cpt(0);
    fileName = "";
    fileFormat = "";

    do
    {
        c = fgetc(filep);
        if (c == ':')
            cpt++;
        else if (cpt == 0)
            fileName += c;
        else
            fileFormat += c;
    }while(cpt < 2);

    fscanf(filep, "%d:%d:%f:", &firstFrameNumber, &lastFrameNumber, &frameRate);
    fscanf(filep, "%d:%d", &reference_renderer.x, &reference_renderer.y);

    framesNumber = lastFrameNumber - firstFrameNumber + 1;

    if (framesNumber > PRL_MAX_FRAMES_ANIM_SIMPLE)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __CERR_REF__ << "In PRL_AnimationSimple, tried to create an animation of " << framesNumber << " frames but the maximum is " << PRL_MAX_FRAMES_ANIM_SIMPLE << ": limiting the number of frames to the maximum" << endl;
        #endif // PRL_USE_WARNINGS
        framesNumber = PRL_MAX_FRAMES_ANIM_SIMPLE; // malloc depends directly on this variable
    }

    fclose(filep); // close the file

    float xratio(1.0*config_GLOBAL.renderResolution.x / reference_renderer.x); // prepares the ratio to change the rect size for correct display
    float yratio(1.0*config_GLOBAL.renderResolution.y / reference_renderer.y);
    texture.resize(framesNumber);

    if (reference_renderer.x * reference_renderer.y <= config_GLOBAL.renderResolution.x * config_GLOBAL.renderResolution.y) // reference renderer <= current renderer
    {
        cout << "Normal memory usage" << endl;
        for (int i(0); i < framesNumber; i++)
        {
            loadingName = filePath + "/" + fileName + to_string((int)(i+firstFrameNumber)) + "." + fileFormat;
            surface = IMG_Load(loadingName.c_str());

            if (surface == nullptr)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Surface not loaded: ") + string(SDL_GetError()));
                clear();
                return PRL_ERROR;
            }

            dstSizes.push_back((PRL_Point) {(int) floor(xratio*surface->w), (int) floor(yratio*surface->h)}); // display sizes
            srcSizes.push_back((PRL_Point) {surface->w, surface->h}); // display size

            texture[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (texture[i] == nullptr)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, could not create the texture: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture creation problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }
            else if (SDL_SetTextureBlendMode(texture[i], SDL_BLENDMODE_BLEND) != 0)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture blend mode problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }
        }
    }
    else // reference renderer > current renderer
    {
        cout << "Reducing memory usage" << endl;
        SDL_Texture *temp_texture(nullptr);
        SDL_Rect temp_dst({0});

        for (int i(0); i < framesNumber; i++)
        {
            loadingName = filePath + string("/") + fileName + to_string((int)(i+firstFrameNumber)) + string(".") + fileFormat;
            surface = IMG_Load(loadingName.c_str());

            if (surface == nullptr)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Surface not loaded: ") + string(SDL_GetError()));
                clear();
                return PRL_ERROR;
            }

            dstSizes.push_back((PRL_Point) {(int) floor(xratio*surface->w), (int) floor(yratio*surface->h)}); // display sizes
            srcSizes.push_back(dstSizes[i]);//((PRL_Point) {surface->w, surface->h}); // display size

            temp_dst = {0, 0, dstSizes[i].x, dstSizes[i].y};

            temp_texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (temp_texture == nullptr)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, could not create the texture: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture creation problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }

            texture[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, temp_dst.w, temp_dst.h);

            if (texture[i] == nullptr)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, could not create the texture: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture creation problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }

            if (SDL_SetRenderTarget(renderer, texture[i]) != 0)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, could not set render target: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture render problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }
            if (SDL_RenderCopy(renderer, temp_texture, NULL, &temp_dst) != 0)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, could not copy texture: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture copy problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }
            SDL_DestroyTexture(temp_texture);

            if (SDL_SetRenderTarget(renderer, NULL) != 0)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, could not set render to default: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture render problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }

            if (SDL_SetTextureBlendMode(texture[i], SDL_BLENDMODE_BLEND) != 0)
            {
                #if PRL_AUTO_WRITE_ERRORS == 1
                cerr << __CERR_REF__ << "In PRL_AnimationSimple, a problem occurred when changing texture blend mode: " << SDL_GetError() << endl;
                #endif // PRL_AUTO_WRITE_ERRORS
                PRL_SetError(string("Texture blend mode problem: ") + string(SDL_GetError()));
                return PRL_ERROR;
            }
        }
    }

    loaded = true;
    return ret;
}

SDL_Texture* PRL_AnimationSimple :: getTexture(size_t which) const
{
    if (which > size_t(framesNumber - 1))
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationSimple, requested the frame " << which << " and maximum is " << framesNumber-1 << " (counting from 0 with " << framesNumber << " frames): returning the first one" << endl;
        #endif // PRL_USE_WARNINGS
        return nullptr;//texture[0]; // null?
    }
    return texture[which];
}

PRL_Point PRL_AnimationSimple :: getSize(size_t which) const
{
    return dstSizes[which];
}

PRL_Point PRL_AnimationSimple :: getSrcSize(size_t which) const
{
    return srcSizes[which];
}

PRL_Point PRL_AnimationSimple :: getRefRendSize() const
{
    return reference_renderer;
}

int PRL_AnimationSimple :: getFramesNumber() const
{
    return framesNumber;
}

void PRL_AnimationSimple :: setRenderer(const SDL_Renderer *newRenderer)
{
    if (!loaded)
    {
        renderer = (SDL_Renderer*) newRenderer;
    }
    else
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationSimple, the renderer can only be changed before the loading occurs" << endl;
        #endif // PRL_USE_WARNINGS
}

float PRL_AnimationSimple :: getFrameRate() const
{
    return frameRate;
}

bool PRL_AnimationSimple :: isLoaded() const
{
    return loaded;
}

PRL_FPointCluster* PRL_AnimationSimple :: getPointCluster() const
{
    return (PRL_FPointCluster*) &pointCluster;
}



PRL_AnimatedSimple :: PRL_AnimatedSimple() : started(false), paused(false), current_anim_repeat_count(0), current_anim_index(0), currentTimeAnim(0), previousTimeAnim(0)
{
    ;
}

PRL_AnimatedSimple :: ~PRL_AnimatedSimple()
{
    targetAnimation.clear();
    currentFrame.clear();
}

bool PRL_AnimatedSimple :: isAdded(const PRL_AnimationSimple *target) const
{
    size_t sz(targetAnimation.size());
    for (size_t i(0); i < sz; ++i)
    {
        if (target == targetAnimation[i]) return true;
    }
    return false;
}

bool PRL_AnimatedSimple :: isAdded(const PRL_AnimationSimple *target, size_t& index) const
{
    size_t sz(targetAnimation.size());
    for (size_t i(0); i < sz; i++)
    {
        if (target == targetAnimation[i])
        {
            index = i;
            return true;
        }
    }
    index = -1;
    return false;
}

int PRL_AnimatedSimple :: makeAnimActive(size_t which)
{
    current_anim_index = which;
}

int PRL_AnimatedSimple :: add(const PRL_AnimationSimple *target)
{
    if (targetAnimation.size() <= PRL_ANIMATION_MAX_TARGETS)
    {
        if (target == nullptr)
        {
            #if PRL_AUTO_WRITE_ERRORS == 1
            cerr << __CERR_REF__ << "In PRL_AnimatedSimple, the target sent to be added is invalid (nullptr)" << endl;
            #endif // PRL_AUTO_WRITE_ERRORS
            PRL_SetError("Invalid pointer on target animation");
            return PRL_ERROR;
        }
        else if (isAdded(target))
        {
            #if PRL_USE_WARNINGS == 1
            cout << __CERR_REF__ << "In PRL_AnimatedSimple, the target sent to be added is already added" << endl;
            #endif // PRL_USE_WARNINGS
            return PRL_WARNING;
        }
        else
        {
            targetAnimation.push_back((PRL_AnimationSimple*) target);
            currentFrame.push_back(0);
            current_anim_index = (size_t) (targetAnimation.size() - 1);
            return 0;
        }
    }
    else
    {
        #if PRL_USE_WARNINGS == 1
        cout << __CERR_REF__ << "In PRL_AnimatedSimple, tried to add more than " << PRL_ANIMATION_MAX_TARGETS << " animation targets: it will be ignored" << endl;
        #endif // PRL_USE_WARNINGS
        return PRL_WARNING;
    }
} /// re read stopped here!!!!!!!!!!!!!!

void PRL_AnimatedSimple :: startAnim()
{
    paused = false;
    started = true;

    PRL_Point dst(targetAnimation[current_anim_index]->getSize(currentFrame[current_anim_index]));
    PRL_Point src(targetAnimation[current_anim_index]->getSrcSize(currentFrame[current_anim_index]));
    dspDst.w = dst.x;
    dspDst.h = dst.y;
    dspSrc.w = src.x;
    dspSrc.h = src.y;
}

void PRL_AnimatedSimple :: restartAnim()
{
    currentFrame[current_anim_index] = 0; // reset frame number
    current_anim_repeat_count = 0;

    paused = false;
    started = true;

    PRL_Point dst(targetAnimation[current_anim_index]->getSize(currentFrame[current_anim_index]));
    PRL_Point src(targetAnimation[current_anim_index]->getSrcSize(currentFrame[current_anim_index]));
    dspDst.w = dst.x;
    dspDst.h = dst.y;
    dspSrc.w = src.x;
    dspSrc.h = src.y;
}

void PRL_AnimatedSimple :: pauseAnim()
{
    paused = true;
}

void PRL_AnimatedSimple :: stopAnim()
{
    started = paused = false;
    currentFrame[current_anim_index] = 0;
    current_anim_repeat_count = 0;
}

bool PRL_AnimatedSimple :: isAnimStarted() const
{
    return started;
}

bool PRL_AnimatedSimple :: isAnimPaused() const
{
    return paused;
}

bool PRL_AnimatedSimple :: isAnimStopped() const // delete?
{
    return !started;
}

int PRL_AnimatedSimple :: getCurrentFrame() const
{
    return currentFrame[current_anim_index];
}

int PRL_AnimatedSimple :: getFramesNumber() const
{
    return targetAnimation[current_anim_index]->getFramesNumber();
}

void PRL_AnimatedSimple :: updateAnim()
{
    currentTimeAnim = timer_GLOBAL.us();

    size_t i(0);

    #if PRL_ANIMATION_CHECK_TIME_AT_UPDATE == 1
    if (started && currentTimeAnim - previousTimeAnim >= PRL_ANIMATION_FPS_CORRECTION_FACTOR/targetAnimation[i]->getFrameRate())
    #else
    if (started)
    #endif // PRL_ANIMATION_CHECK_TIME_AT_UPDATE
    {
        #if PRL_ANIMATION_CHECK_TIME_AT_UPDATE == 1
        previousTimeAnim = currentTimeAnim; // Update he time, independent of the animation being paused or not
        #endif // PRL_ANIMATION_CHECK_TIME_AT_UPDATE

        if (!paused)
        {
            if (currentFrame[i] == targetAnimation.at(i)->getFramesNumber() - 1)
            {
                currentFrame[i] = 0;
                ++current_anim_repeat_count;
            }
            else
            {
                ++currentFrame[i];
            }

            dspTexture = targetAnimation[i]->getTexture(currentFrame[i]);
        }
    }

}






PRL_AnimationVideo :: PRL_AnimationVideo(int buff) : bufferSz(buff), framesFilledInBufferAtUpdate(1)
{
    texture = (SDL_Texture**) malloc(PRL_ANIM_VIDEO_BUFFER*sizeof(SDL_Texture*));
    rect = (SDL_Rect*) malloc(PRL_ANIM_VIDEO_BUFFER*sizeof(SDL_Rect));
    if (texture == NULL || rect == NULL)
        cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, unable to prepare the buffer of " << PRL_ANIM_VIDEO_BUFFER << " frames" << endl;
    else
        for (int i(0); i<bufferSz; i++)
        {
            texture[i] = NULL;
            rect[i] = {0};
        }

    path = (char*) malloc (FILENAME_LENGTH * sizeof(char));
    filename = (char*) malloc (FILENAME_LENGTH * sizeof(char));
    format = (char*) malloc (FORMAT_LENGTH * sizeof(char));
    strcpy(path, "");
    strcpy(filename, "");
    strcpy(format, "");
    reference_renderer = {0};
    referenceFramePosition = {0};
    framesNumber = 0;
    frameRate = 0;
    framesReady = currentFrame = 0;
    loaded = started = paused = fillBuffAtUpdate = false;
    firstFrameNumber = lastFrameNumber = 0;
    previousTimeAnim = 0;
    dump_texture = NULL;
}

PRL_AnimationVideo :: ~PRL_AnimationVideo()
{
    for (int i(0); i < bufferSz; i++)
        if (texture[i] != NULL) SDL_DestroyTexture(texture[i]);

    if (texture != NULL) free(texture);
    if (rect != NULL) free(rect);

    if (path != NULL) free(path);
    if (filename != NULL) free(filename);
    if (format != NULL) free(format);
    if (dump_texture != NULL) SDL_DestroyTexture(dump_texture);
}

SDL_Rect PRL_AnimationVideo :: getSize(int bufferedFrameNb) const
{
    if (bufferedFrameNb >= 0 && bufferedFrameNb < bufferSz)
        return rect[bufferedFrameNb];
    else
    {
        return {0, 0, 0, 0};
    }
}

SDL_Rect PRL_AnimationVideo :: getReferenceRendererSize() const
{
    return reference_renderer;
}

int PRL_AnimationVideo :: getFramesNumber() const
{
    return framesNumber;
}

void PRL_AnimationVideo :: setBufferSize(const int buff)
{
    if (loaded) // this condition restricts the usage of setBuffer to the case of a still unloaded animation
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, tried to change the buffer after having called load()" << endl;
        #endif // PRL_USE_WARNINGS
    }
    else if (buff < 1)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, tried to set a negative or null buffer value (" << buff << ")" << endl;
        #endif // PRL_USE_WARNINGS
    }
    else
    {
        SDL_Texture **p = (SDL_Texture**) malloc(buff * sizeof(SDL_Texture*));
        SDL_Rect *p2 = (SDL_Rect*) malloc(buff * sizeof(SDL_Rect));
        if (p == NULL || p2 == NULL)
        {
            #if PRL_USE_WARNINGS == 1
            cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, unable to reallocate memory for the new buffer of " << buff << ". Ancient buffer of " << bufferSz << " is kept" << endl;
            #endif // PRL_USE_WARNINGS
        }
        else
        {
            int i(0);
            int minbuff(bufferSz);
            if (bufferSz > buff) minbuff = buff;
            for (i = 0; i < minbuff; i++)
            {
                p[i] = texture[i];
                p2[i] = rect[i];
            }

            if (buff < bufferSz)
            {
                for (; i < bufferSz; i++)
                {
                    if (texture[i] != NULL) SDL_DestroyTexture(texture[i]);
                    rect[i] = {0};
                }
            }
            free(texture);
            free(rect);
            texture = p;
            rect = p2;
            bufferSz = buff;
        }
    }
}

int PRL_AnimationVideo :: getBufferSize() const
{
    return bufferSz;
}

int PRL_AnimationVideo :: getBufferReady() const
{
    return framesReady;
}

int PRL_AnimationVideo :: load(const char file[])
// Retrieve path, name, format, frames number, fps, ...
{
    if (loaded) // if already loaded, free the memory
    {
        for (int i(0); i<framesNumber; i++)
        {
            if (texture[i] != NULL)
            {
                SDL_DestroyTexture(texture[i]);
                texture[i] = NULL;
                rect[i] = {0};
            }
        }
    }
    FILE* filep = fopen(file, "r");
    if (filep == NULL)
    {
        cerr << "Error: could not open " << file << ": the file may be missing, locked or used" << endl;
        return PRL_ERROR;
    }

    // initialized in the constructor
    strcpy(path, "");
    strcpy(filename, "");
    strcpy(format, "");

    char temp[FILENAME_LENGTH] = "", c = -1;
    int filename_count(0), format_count(0), pathlength(0);
    bool reading(true);

    #if PRL_ALLOW_FLOAT_FPS == 1
    fscanf(filep, "%d:%d:%f:", &firstFrameNumber, &lastFrameNumber, &frameRate);
    #else
    fscanf(filep, "%d:%d:%d:", &firstFrameNumber, &lastFrameNumber, &frameRate);
    #endif // PRL_ALLOW_FLOAT_FPS

    framesNumber = lastFrameNumber-firstFrameNumber+1;

    if (framesNumber > PRL_MAX_FRAMES_ANIM_SIMPLE)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, tried to create an animation of " << framesNumber << " frames but the maximum is " << PRL_MAX_FRAMES_ANIM_SIMPLE << ": limiting the number of frames to the maximum" << endl;
        #endif // PRL_USE_WARNINGS
        framesNumber = PRL_MAX_FRAMES_ANIM_SIMPLE; // malloc depends directly on this variable
    }

    while (reading) // reading filename
    {
        c = fgetc(filep);
        if (c == EOF)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, the .anim file " << file << " is badly defined" << endl;
            return PRL_ERROR;
        }
        else if (c == PRL_FILE_SEPARATION_CHARACTER) // end of the current char variable
        {
            reading = false;
        }
        else
        {
            if (filename_count == FILENAME_LENGTH)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, the .anim file " << file << " contains an animation file name of more than " << FILENAME_LENGTH << " characters" << endl;
                return PRL_ERROR;
            }
            sprintf(temp, "%c", c);
            strcat(filename, temp);
            filename_count++;
        }
    }

    reading = true; int var_to_check_format_point(0);
    while (reading) // reading format
    {
        c = fgetc(filep); var_to_check_format_point++;
        if (c == EOF)
        {
            cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, the .anim file " << file << " is badly defined" << endl;
            return PRL_ERROR;
        }
        else if (c == '.' && var_to_check_format_point == 0) // do not read the first format point if written (to be added later!)
        {
            ;
        }
        else if (c == PRL_FILE_SEPARATION_CHARACTER) // end of the current char variable
        {
            reading = false;
        }
        else
        {
            if (format_count == FORMAT_LENGTH)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, the .anim file " << file << " contains an animation file format of more than " << FORMAT_LENGTH << " characters" << endl;
                return PRL_ERROR;
            }
            sprintf(temp, "%c", c);
            strcat(format, temp);
            format_count++;
        }
        var_to_check_format_point++;
    }
    fscanf(filep, "%d/%d", &reference_renderer.x, &reference_renderer.y);
    reference_renderer.w = reference_renderer.x;
    reference_renderer.h = reference_renderer.y;

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
            //strcat(temp, filename); // adds the root filename to the path: not needed here
            strcpy(path, temp);
        }
    }
    fclose(filep); // close the file

    fillBuffer(bufferSz);

    loaded = true;
    return 0;
}

int PRL_AnimationVideo :: fillBuffer(const int frames)
{
    bool timeHasArrived(false);
    char loadingName[FORMAT_LENGTH+FILENAME_LENGTH+10] = ""; // + 1 for the point and + 9 for the number string length (000001)
    char temp[FILENAME_LENGTH] = "";
    SDL_Surface *surface = NULL;

    float xratio = 1.0 * config_GLOBAL.renderResolution.x / reference_renderer.x; // prepares the ratio to change the rect size for correct display
    float yratio = 1.0 * config_GLOBAL.renderResolution.y / reference_renderer.y;

    if (frames > 0) timeHasArrived = true;

    int cpt(0);
    for (int i(0); (cpt<frames || !timeHasArrived) && i<bufferSz; i++)
    {
        if (texture[i] == NULL)
        {
            strcpy(loadingName, path); // add the path
            strcat(loadingName, filename); // add the filename
            sprintf(temp, "%d.%s", currentFrame+framesReady+firstFrameNumber, format); // prepare number . format
            strcat(loadingName, temp); // add the number . format
            surface = IMG_Load(loadingName);

            if (surface == NULL)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo: " << SDL_GetError() << endl;
                return PRL_ERROR;
            }
            rect[i].x = rect[i].w = xratio*surface->w; // set the size of every frame
            rect[i].y = rect[i].h = xratio*surface->h;

            texture[i] = SDL_CreateTextureFromSurface(dspRenderer, surface);
            SDL_FreeSurface(surface); surface = NULL;

            if (texture[i] == NULL)
            {
                cerr << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, could not create the texture: " << SDL_GetError() << endl;
                return PRL_ERROR;
            }
            if (SDL_SetTextureBlendMode(texture[i], SDL_BLENDMODE_BLEND) != 0)
            {
                #if PRL_USE_WARNINGS == 1
                cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, a problem occurred when changing texture blend mode: " << SDL_GetError() << ". Blend mode not changed" << endl;
                #endif // PRL_USE_WARNINGS
            }

            framesReady++;

            if (frames <= 0) // if loading continuously
            {
                if (timer_GLOBAL.us() - previousTimeAnim >= 0.95*PRL_ANIMATION_FPS_CORRECTION_FACTOR/frameRate) // 0.95 is an additional safety factor
                    timeHasArrived = true;
            }
            cpt++;
        }
    }
    return 0;
}

void PRL_AnimationVideo :: startAnim()
{
    started = true;
    paused = false;
}

void PRL_AnimationVideo :: restartAnim()
{
    started = true;
    paused = false;
    freeBuffer();
    fillBuffer(1);
}

void PRL_AnimationVideo :: pauseAnim()
{
    paused = true;
}

void PRL_AnimationVideo :: stopAnim()
{
    started = paused = false;
    freeBuffer();
}

bool PRL_AnimationVideo :: isStarted() const
{
    return started;
}

bool PRL_AnimationVideo :: isPaused() const
{
    return paused;
}

bool PRL_AnimationVideo :: isStopped() const
{
    return !started;
}

int PRL_AnimationVideo :: getCurrentFrame() const
{
    return currentFrame;
}

void PRL_AnimationVideo :: updateAnim()
{
    #if PRL_ANIMATION_CHECK_TIME_AT_UPDATE == 1
    long long currentTime = timer_GLOBAL.us();
    #endif // PRL_ANIMATION_CHECK_TIME_AT_UPDATE
    if (!started) dspTexture = NULL;
    else
    {
        #if PRL_ANIMATION_CHECK_TIME_AT_UPDATE == 1
        if (!paused && currentTime - previousTimeAnim >= PRL_ANIMATION_FPS_CORRECTION_FACTOR/frameRate)
        #else
        if (!paused)
        #endif // PRL_ANIMATION_CHECK_TIME_AT_UPDATE
        {
            #if PRL_ANIMATION_CHECK_TIME_AT_UPDATE == 1
            previousTimeAnim = currentTime;
            #endif // PRL_ANIMATION_CHECK_TIME_AT_UPDATE
            if (currentFrame == framesNumber-1)
            {
                stopAnim();
            }
            else
            {
                if (dump_texture != NULL)
                    SDL_DestroyTexture(dump_texture);
                dspTexture = texture[0];
                dspDst = {referenceFramePosition.x + (referenceFramePosition.w-rect[0].w)/2, referenceFramePosition.y + (referenceFramePosition.h-rect[0].h)/2, rect[0].w, rect[0].h};
                dspSrc = {0, 0, rect[0].w, rect[0].h};

                dump_texture = texture[0];

                for (int i(0); i < bufferSz-1; i++) // reorganize the buffer
                {
                    texture[i] = texture[i+1];
                    rect[i] = rect[i+1];
                }
                texture[bufferSz-1] = NULL;
                rect[bufferSz-1] = {0};
                framesReady--;

                if (fillBuffAtUpdate) fillBuffer(framesFilledInBufferAtUpdate);

                currentFrame++;
                if (currentFrame == framesNumber-1)
                {
                    stopAnim();
                }
            }
        }
    }
}

#if PRL_ALLOW_FLOAT_FPS == 1
float PRL_AnimationVideo :: getFrameRate() const
#else
int PRL_AnimationVideo :: getFrameRate() const
#endif // PRL_ALLOW_FLOAT_FPS
{
    return frameRate;
}

bool PRL_AnimationVideo :: isLoaded() const
{
    return loaded;
}

void PRL_AnimationVideo :: fillBufferAtUpdate(bool fillAtUpdate, const int frames)
{
    fillBuffAtUpdate = fillAtUpdate;
    framesFilledInBufferAtUpdate = frames;
}

void PRL_AnimationVideo :: fillBufferAtUpdate(bool fillAtUpdate)
{
    fillBuffAtUpdate = fillAtUpdate;
    framesFilledInBufferAtUpdate = 1;
}

void PRL_AnimationVideo :: freeBuffer()
{
    for (int i(0); i<bufferSz; i++)
    {
        if (texture[i] != NULL)
        {
            SDL_DestroyTexture(texture[i]);
            texture[i]=NULL;
        }
        rect[i] = {0};
    }
    framesReady = 0;
    currentFrame = 0;
}

void PRL_AnimationVideo :: setPosition(int x, int y)
{
    if (!loaded)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_AnimationVideo, the position cannot be set before the loading occurs since setPosition() demands at least 1 frame loaded" << endl;
        #endif // PRL_USE_WARNINGS
    }
    else
    {
        referenceFramePosition.x = x;
        referenceFramePosition.y = y;
        referenceFramePosition.w = rect[0].w;
        referenceFramePosition.h = rect[0].h;
        dspDst = {(float) x, (float) y, rect[0].w, rect[0].h};
        dspSrc = {0.0, 0.0, rect[0].w, rect[0].h};
    }
}*/

