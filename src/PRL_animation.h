#ifndef PRL_ANIMATION_H_INCLUDED
#define PRL_ANIMATION_H_INCLUDED

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "PRL_display.h"
#include "PRL_defines.h"
//#include "PRL_collision.h"

//using std::vector;

//! Enumeration containing all the hit box types
/*!
The different hit box types are:
- rectangular (PRL_HITBOX_RECT), the hit box is a rectangle stored as a PRL_FRect
- circle (PRL_HITBOX_CIRCLE), the hit box is a circle, stored as a PRL_FCircle
- polygon (PRL_HITBOX_POLYGON), the hit box is an arbitrary shape, described by points arranged in a certain order, and stored as an PRL_FPointList
*/
enum PRL_HitBoxType
{
    PRL_HITBOX_RECT, PRL_HITBOX_CIRCLE, PRL_HITBOX_POLYGON
};


/* ********************************************* */
/*                PRL_PointList                  */
/* ********************************************* */

//! Class used to store points.
/*!
This class provides a convenient way to store points. Please note that the points are stored as float numbers, and any integer point added will be converted into a float one.
*/
class PRL_FPointList
{
public:
    //! Constructor.
    PRL_FPointList();
    //! Destructor.
    ~PRL_FPointList();

    //! Add a point to the list.
    void add(PRL_FPoint& point);
    //! Add a point to the list.
    /*!
    The provided point with int coordinates will be converted into float coordinates.
    */
    void add(PRL_Point& point);
    //! Insert a point to the list at a certain index.
    void insert(PRL_FPoint& point, unsigned int index);
    //! Insert a point (int) to the list at a certain index.
    /*!
    The provided point with int coordinates will be converted into float coordinates.
    */
    void insert(PRL_Point& point, unsigned int index);
    //! Get point at index.
    PRL_FPoint const& get(unsigned int index) const;
    //! Get the total number of points.
    unsigned int getPointsNumber() const;
    //! Get the total number of points.
    unsigned int size() const;
    //! Remove the point at the specified index.
    void remove(int index);
    //! Invert the order of the whole list.
    void invert();
    //! Translate all the points.
    /*!
    All the points will undergo the following translation:
    p' = p + (x, y)
    where p' are the transformed points, p the original ones and (x, y) the translation vector.
    */
    void translateBy(float x, float y);
    //! Get the rectangle around the FPoint list.
    /*!
    This method gives the smallest rectangle in which all the points fit. The rectangle is aligned with the principal axes x and y.
    This can be used to get from a complex form a simple geometric object on which a first collision test is done in order to determinate whether or not to proceed to further tests.
    Be careful: this method doesn't update the rectangle (see calculateRectAround and checkRectAroundWithPoint).
    */
    PRL_FRect const& getRectAround() const;

private:
    std::vector <PRL_FPoint> points; //!< The vector containing all the points.
    PRL_FRect rect_arround_hitbox; //!< This rectangle stores the last calculated rectangle around the points.

    //! Calculate the rectangle around the point list.
    /*!
    This method computes the smallest rectangle in which all the points fit. The rectangle is aligned with the principal axes x and y.
    */
    void calculateRectAround();
    //! Update the rectangle around based on a point.
    /*!
    This method is automatically used when a point is added to the point list. It computes the new rectangle using the previous one and checking whether the new point lies inside it or not.
    */
    void checkRectAroundWithPoint(PRL_FPoint& point);
};



/* ********************************************* */
/*                  PRL_HitBox                   */
/* ********************************************* */

//! A class defining hit boxes.
/*!
This class provides methods to manipulate hit boxes. Please note that the points are stored as float numbers.
*/
class PRL_HitBox
{
private:
    PRL_HitBoxType _type; //!< Type of the hit box.
    PRL_FRect _rect; //!< If the hit box type is a rectangle (PRL_HITBOX_RECT), this member will store the corresponding points.
    PRL_FCircle _circle; //!< If the hit box type is a circle (PRL_HITBOX_CIRCLE), this member will store the corresponding points.
    PRL_FPointList _fpointList; //!< If the hit box type is a polygon (PRL_HITBOX_POLYGON), this member will store the corresponding points.
    PRL_FRect rect_arround_hitbox; //!< This member store the smallest rectangle around the hit box.

public:
    //! Constructor.
    PRL_HitBox();
    //! Destructor.
    ~PRL_HitBox();

    //! Set a rectangle hit box.
    /*!
    Doing so, the hit box type becomes PRL_HITBOX_RECT.
    If the hit box previously was of another type, the existing data is replaced.
    */
    void set(PRL_FRect& rect);
    //! Set a circle hit box.
    /*!
    Doing so, the hit box type becomes PRL_HITBOX_CIRCLE.
    If the hit box previously was of another type, the existing data is replaced.
    */
    void set(PRL_FCircle& circle);
    //! Set a polygon hit box.
    /*!
    Doing so, the hit box type becomes PRL_HITBOX_POLYGON.
    If the hit box previously was of another type, the existing data is replaced.
    */
    void set(PRL_FPointList& fpointList);
    //! Get the rectangle if type is rectangle.
    /*!
    If the hit box type is not PRL_HITBOX_RECT, you will get a null rectangle: {x, y, w, h} = {0, 0, 0, 0}
    where x and y are the upper left corner coordinates, w the width and h the height.
    */
    PRL_FRect const& getRect() const;
    //! Get the circle if type is circle.
    /*!
    If the hit box type is not PRL_HITBOX_CIRCLE, you will get a null circle: {x, y, r} = {0, 0, 0}
    where x and y are the upper left corner coordinates and r the radius.
    */
    PRL_FCircle const& getCircle() const;
    //! Get the polygon if type is polygon.
    /*!
    If the hit box type is not PRL_HITBOX_POLYGON, you will get an empty point list.
    */
    PRL_FPointList const& getPolygon() const;
    //! Get the hit box type.
    PRL_HitBoxType getType() const;
    //! Get the rectangle around the hit box.
    /*!
    This method gives the smallest rectangle in which the hit box fit. The rectangle is aligned with the principal axes x and y.
    If the hit box type is already PRL_HITBOX_RECT, this rectangle will actually be the hit box itself.
    */
    PRL_FRect const& getRectAround() const;
    //! Translate the whole hit box.
    /*!
    All the points composing the hit box will undergo the following translation:
    p' = p + (x, y)
    where p' are the transformed points, p the original ones and (x, y) the translation vector.
    */
    void translateBy(float x, float y);
};

/* ********************************************* */
/*               PRL_PointCluster                */
/* ********************************************* */

//! A class containing points and hit boxes.
/*!
This class contains 2 types of points: action points and anchor points.
Action points are points used to specify location on an animation associated with specific actions (i.e. the foot of a player in 2D combat game).
Fixed points are points used when an animation has a changing size. It defines the static reference point in the animation.
So, if the animation is not translating on the screen, the anchor point will remain at the same place.
*/
class PRL_FPointCluster
{
    friend class PRL_AnimationSimple;

private:
    //! Vector containing the hit boxes.
    /*!
    The first dimension is the corresponding frames and the second one the hit box number.
    Each hit box has the same number during the whole animation: the hit box number is constant frame to frame.
    */
    std::vector <std::vector <PRL_HitBox>> hitBoxe;
    //! Vector containing the action points.
    /*!
    The first dimension is the corresponding frames and the second one the action point number.
    Each action point has the same number during the whole animation: the action point number is constant frame to frame.
    */
    std::vector <std::vector <PRL_FPoint>> actionPoint;
    //! Vector containing the anchor points.
    /*!
    There is only 1 anchor point per frame, so the vector is simple.
    */
    std::vector <PRL_FPoint> anchorPoint;

public:
    //! Constructor.
    PRL_FPointCluster();
    //! Destructor.
    ~PRL_FPointCluster();

    //! Get how many frames are loaded.
    /*!
    This should give the same than the number of anchor points (1 per frame).
    */
    int getFramesNumber() const; // HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERE DOXY_STOP
    //! Get the number of action points at each frame
    int getActionPointsNumber() const;
    //! Get the number of hit boxes at each frame
    int getHitBoxesNumber() const;
    //! Get the desired hitbox at the desired frame
    PRL_HitBox const& getHitBox(int frame, int hitBoxNumber) const;
    //! Get the desired action point at the desired frame
    PRL_FPoint const& getActionPoint(int frame, int actionPointNumber) const;
    //! Get the anchor point at the desired frame
    PRL_FPoint const& getFixedPoint(int frame) const;
    //! Set the desired hitbox at the desired frame
    void setHitBox(int frame, int hitBoxNumber, PRL_HitBox const& hitbox);
    //! Set the desired action point at the desired frame
    void setActionPoint(int frame, int actionPointNumber, PRL_FPoint const& point);
    //! Set the anchor point at the desired frame
    void setAnchorPoint(int frame, PRL_FPoint const& point);
};


/* ********************************************* */
/*                 PRL_Animation                 */
/* ********************************************* */


class PRL_Animation
{
public:
	/// Need the path of the animation's .anim file.
	PRL_Animation(const char* path, SDL_Renderer *renderer);
	/// Need the path of the animation's .anim file.
	PRL_Animation(const std::string& path, SDL_Renderer *renderer);
	~PRL_Animation();

	struct _display
	{
		SDL_Texture* getTexture(size_t which) const;
		/// Return the size {width, height, width, height}.
		const PRL_Point& getSize(size_t which) const; // no bound checking, in term of current render resolution
		/// Return the source size (real one without scaling) {width, height, width, height}.
		const PRL_Point& getSrcSize(size_t which) const;
		/// Return {width, height, width, height}.
		const PRL_Point& getTargetResolution() const;
		/// Return the total number of frames of the animation.
		int getFramesNumber() const;
		/// Return the frame rate.
		float getFrameRate() const;
		/// Get the point cluster.
	};
	_display display;

	struct _points
	{
	public:
        std::vector <PRL_FPointList> action;
        PRL_FPointList anchor;
        std::vector <PRL_HitBox> hitbox;
	private:
		//PRL_FPointCluster *getPointCluster() const; // change to reference?
	};
	_points points;

	/// Get the number of PRL_Animation currently declared.
	static int getCount();

private:
	static int animationsCount;

	std::string filePath;
	std::vector <SDL_Texture*> mainTexture;
	std::vector <SDL_Surface*> mainSurface;
	std::vector <std::vector <SDL_Texture*> > maskTexture;
	std::vector <std::vector <SDL_Surface*> > maskSurface;
	std::vector <std::vector <PRL_FRect> > maskPos;
	std::vector <PRL_Point> dstSize;
	std::vector <PRL_Point> srcSize;
	PRL_Point refRenderer; // size under the following form: reference_renderer = {w, h, w, h};

	SDL_Renderer *renderer; // the used renderer

	PRL_FPointCluster pointCluster; // contains all the points loaded from the .anim file
	size_t maskPerTexture;

	float frameRate; // FPS



	void load();
	int load_CPU();
	int load_GPU();
	void clear_CPU();
};



/* ********************************************* */
/*            PRL_AnimationSimple                */
/* ********************************************* */

#define PRL_MAX_FRAMES_ANIM_SIMPLE 18000 // 18000 frames gives 5 minutes at 60 FPS

class PRL_AnimationSimple /// A simple animation has frames of constant size. See PRL_MAX_FRAMES_ANIM_SIMPLE to get the maximum frames number
{
public:
    PRL_AnimationSimple();
    ~PRL_AnimationSimple();

    /// Need the path of the animation's .anim file
    int load(const char file_path[]);
    /// Need the path of the animation's .anim file
    int load(std::string& file_path);
    /// Number between 0 and framesNumber-1
    SDL_Texture* getTexture(size_t which) const;
    //const SDL_Texture* operator[](size_t index) const;
    //const SDL_Texture* at(size_t index) const;

    /// Return the size {width, height, width, height}
    PRL_Point getSize(size_t which) const; // no bound checking, in term of current render resolution
    /// Return the source size (real one without scaling) {width, height, width, height}
    PRL_Point getSrcSize(size_t which) const;
    /// Return {width, height, width, height}
    PRL_Point getRefRendSize() const;
    /// Return the total number of frames of the animation
    int getFramesNumber() const;
    /// Set the renderer to be used. By default renderer_GLOBAL[0]. Use this function before load(const char file_path[])
    void setRenderer(const SDL_Renderer *newRenderer); // in constructor!
    /// Return the frame rate
    float getFrameRate() const;

    /// Tell whether the animation has already been loaded or not
    bool isLoaded() const;
    /// Get the point cluster
    PRL_FPointCluster *getPointCluster() const; // change to reference?

    void clear();
    // add define to remove checking

private:
    static int animations_count;
    std::vector <SDL_Texture*> texture; // pointer of pointer in order to use malloc() ---- change to vector!
    std::vector <PRL_Point> dstSizes; // pointer of pointer in order to use malloc() ---- change to vector!
    std::vector <PRL_Point> srcSizes; // pointer of pointer in order to use malloc() ---- change to vector!
    PRL_Point rect, srcRect; // actual size of the frames under the following form: rect = {w, h, w, h};
    PRL_Point reference_renderer; // size under the following form: reference_renderer = {w, h, w, h};
    int framesNumber; // total number of frames
    SDL_Renderer *renderer; // the used renderer
    PRL_FPointCluster pointCluster; // contains all the points loaded from the .anim file


    float frameRate; // FPS

    bool loaded; // whether it is loaded or not
};



/* ********************************************* */
/*             PRL_AnimatedSimple                */
/* ********************************************* */

#define PRL_ANIMATION_MAX_TARGETS 128
#define PRL_ANIMATION_CHECK_TIME_AT_UPDATE 1 // make the update time-dependent (synchronous)
#define PRL_ANIMATION_FPS_CORRECTION_FACTOR 950000 // 0.95 * 1 000 000, used in the updateAnim method to correct FPS asynchronous behavior


class PRL_AnimatedSimple : public PRL_Displayable
{
public:
    PRL_AnimatedSimple();
    ~PRL_AnimatedSimple();
    /// Add a new animation target
    int add(const PRL_AnimationSimple *target);
    ///Add a new animation target and gives the corresponding animation index
    int add(const PRL_AnimationSimple *target, size_t& index);
    /// Check whether the animation target is already added or not
    bool isAdded(const PRL_AnimationSimple *target) const;
    /// Check whether the animation target is already added or not and gives the corresponding animation index
    bool isAdded(const PRL_AnimationSimple *target, size_t& index) const;
    /// Start or resume an animation and pause all the others
    void startAnim();
    /// Reset and start an animation
    void restartAnim();
    /// Pause an animation
    void pauseAnim();
    /// Stop an animation and set the current frame to 0
    void stopAnim();
    /// Check whether the animation is started
    bool isAnimStarted() const;
    /// Check whether the animation is paused
    bool isAnimPaused() const;
    /// Check whether the animation is stopped
    bool isAnimStopped() const;
    /// Return the current frame of the currently used animation
    int getCurrentFrame() const;
    /// Return how many times the currently active animation has repeated without stopping
    int getRepeatCount() const;
    /// Make the specified animation active and start it. Doing so, the previous active animation is stopped and becomes inactive
    int makeAnimActive(size_t which);
    /// Return the number of frames of the currently active animation
    int getFramesNumber() const;
    /// Update the animation (only the graphical part, no positioning)
    void updateAnim();
    /// Note: there is no remove method since this object isn't meant to be reshaped after the initialization

protected:
    std::vector <PRL_AnimationSimple*> targetAnimation;
    std::vector <int> currentFrame;

    bool started, paused;
    int current_anim_repeat_count;
    size_t current_anim_index;
    long long currentTimeAnim, previousTimeAnim; /// microseconds

    // Used when an animation is started to pause all the others.
    //void pauseAllExcept(const int which); // Keep private!
};

class PRL_Animated : public PRL_AnimatedSimple
{

};

/// ////////////////////////// LATER: add sound directly built-in
/* ********************************************* */
/*             PRL_AnimationVideo                */
/* ********************************************* */

#define PRL_ANIM_VIDEO_BUFFER 20 // 20 frames prepared in advance

class PRL_AnimationVideo : public PRL_Displayable /// A simple animation has frames of constant size. See PRL_MAX_FRAMES_ANIM_SIMPLE to get the maximum frames number
{
public:
    PRL_AnimationVideo(int buff = PRL_ANIM_VIDEO_BUFFER);
    ~PRL_AnimationVideo();

    /// Need the path of the animation's .anim file
    int load(const char file_path[]);
    /// Return {width, height, width, height}. Needs a frame number within the buffer size
    SDL_Rect getSize(int bufferedFrameNb) const;
    /// Return {width, height, width, height}
    SDL_Rect getReferenceRendererSize() const;
    /// Return the total number of frames of the animation
    int getFramesNumber() const;
    /// Set the buffer, before calling load() (note that it is automatically set to PRL_ANIM_VIDEO_BUFFER)
    void setBufferSize(int buff);
    /// Return the current buffer size
    int getBufferSize() const;
    /// Return how many frames are ready to be displayed
    int getBufferReady() const;
    /// Start the animation
    void startAnim();
    /// Reset and start an animation
    void restartAnim();
    /// Pause an animation
    void pauseAnim();
    /// Stop an animation and set the current frame to 0
    void stopAnim();
    /// Check whether the animation is started
    bool isStarted() const;
    /// Check whether the animation is paused
    bool isPaused() const;
    /// Check whether the animation is stopped
    bool isStopped() const;
    /// Return the current frame of the animation
    int getCurrentFrame() const;
    /// Update the animation (only the graphical part, no positioning)
    void updateAnim();
    /// Return the frame rate
    #if PRL_ALLOW_FLOAT_FPS == 1
    float getFrameRate() const;
    #else
    int getFrameRate() const;
    #endif // PRL_ALLOW_FLOAT_FPS
    /// Tell whether the animation has already been loaded or not
    bool isLoaded() const;
    /// Fill the buffer with the specified frames number, 0 to fill continuously until calling stopFilingBuffer();
    int fillBuffer(const int frames);
    /// State whether the buffer fills up at updateAnim()
    void fillBufferAtUpdate(bool fillAtUpdate, const int frames);
    /// State whether the buffer fills up at updateAnim() By default 1 frame
    void fillBufferAtUpdate(bool fillAtUpdate);
    /// Set position
    void setPosition(int x, int y);

private:
    SDL_Texture** texture; // pointer of pointer in order to use malloc()
    SDL_Rect* rect; // actual size of the frames under the following form: rect = {w, h, w, h};
    SDL_Rect reference_renderer; // size under the following form: reference_renderer = {w, h, w, h};
    SDL_Rect referenceFramePosition;
    int framesNumber; // total number of frames
    SDL_Texture* dump_texture; // texture to be destroyed on next loop (rendered and out of the buffer)
    #if PRL_ALLOW_FLOAT_FPS == 1
    float frameRate; // FPS (float)
    #else
    int frameRate; // FPS
    #endif // PRL_ALLOW_FLOAT_FPS
    int bufferSz, framesReady;
    int currentFrame; // the current displayed frame

    bool loaded; // whether it is loaded or not
    bool started, paused, fillBuffAtUpdate;
    int framesFilledInBufferAtUpdate;

    char *path, *filename, *format; // file reading specific
    int firstFrameNumber, lastFrameNumber; // file reading specific
    long long previousTimeAnim;

    void freeBuffer();
};

#endif // PRL_ANIMATION_H_INCLUDED

