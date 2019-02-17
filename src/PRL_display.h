#ifndef PRL_CLASS_DISPLAYABLE_H_INCLUDED
#define PRL_CLASS_DISPLAYABLE_H_INCLUDED

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "PRL_defines.h"
#include "PRL_types.h"
#include "PRL_timer.h"
#include "PRL_camera.h"


/* ********************************************* */
/*              PRL_Displayable                  */
/* ********************************************* */

#define PRL_DISPLAYABLE_DEFAULT_ADDRESS -1 // a negative value is important!

class PRL_Displayable
{
    friend class PRL_Displayer;

public:
    PRL_Displayable();
    ~PRL_Displayable();

    // Returns 0 if allright
    int setTexture(SDL_Texture* texture);
    /// Get the texture to be displayed.
    SDL_Texture* getTexture() const;

    int setRenderer(SDL_Renderer *renderer);
    /// Make the displayable active or not.
    void makeActive(bool b);
    /// Tell whether the displayable object is active (displayed by the displayer) or not.
    bool isActive() const;
    /// Set the display angle.
    void setAngle(double angle);
    /// Get the display angle.
    double getAngle() const;
    /// Set the position (upper left corner).
    virtual void setPos(float x, float y);
    /// Set the position (upper left corner).
    virtual void setPos(PRL_FPoint pos);
    /// Set the position of the center.
    virtual void setCenterPos(float x, float y);
    /// Set the position of the center.
    virtual void setCenterPos(PRL_FPoint pos);
    /// Get destination rectangle used for display.
    PRL_FRect const& getDstRect() const;
    /// Get source rectangle used for display.
    PRL_FRect const& getSrcRect() const;
    /// Get how many displayable objects are currently in use.
    static int getDspCount();

protected:
    PRL_FRect dspSrc;
    PRL_FRect dspDst;
    SDL_Texture *dspTexture;
    SDL_Renderer *dspRenderer;
    bool dspIsActive;
    double dspAngle;

    //! Updates the int dst and src (SDL_Rect) FULL rectangles (including w and h) from the float ones. SDL_Rect is currently used by SDL for rendering
    void updateDisplayRect(); // from float to int - to be depreciated as long as SDL 2.x uses float as coordinates

private:
    //! Variables used exclusively for displaying textures on the renderer since current version of SDL2 works with integer coordinates.
    SDL_Rect dspSrc_int, dspDst_int;

    //! Used only by displayer: address of the displayable in the displayer
    size_t dspDisplayerAddress;
    bool dspDisplayerAdded;

    static int dspCount;
};


/* ********************************************* */
/*               PRL_Displayer                   */
/* ********************************************* */

class PRL_Displayer
{
private:
	std::vector <PRL_Displayable*> object;
	PRL_Camera *camera;

	static int dsprCount;

public:
    PRL_Displayer();
    ~PRL_Displayer();

    /// Add a PRL_Displayable object to the renderer
	int add(PRL_Displayable *obj);
	/// Remove a PRL_Displayable object from the renderer
	int remove(PRL_Displayable *obj);
    /// Return how many objects are currently added
	size_t getObjectsNumber() const;
	/// Set the used camera
    void setCamera(PRL_Camera *cam);
	/// Display all the (active) objects of the renderer
	int display() const;
	/// Display using the current camera, specifying whether or not to use source rectangles
	int displayWithCamera() const;
    /// Get how many displayer objects are currently in use.
	static int getDsprCount();
};

#endif // PRL_CLASS_DISPLAYABLE_H_INCLUDED
