#ifndef PRL_CLASS_DISPLAYABLE_H_INCLUDED
#define PRL_CLASS_DISPLAYABLE_H_INCLUDED

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "PRL_defines.h"
#include "PRL_types.h"
#include "PRL_time.h"
#include "PRL_camera.h"
#include "PRL_collision.h"


/* ********************************************* */
/*              PRL_Displayable                  */
/* ********************************************* */


class PRL_Displayable
{
    friend class PRL_Displayer;

public:
	PRL_Displayable();
    PRL_Displayable(SDL_Texture *texture, SDL_Renderer *renderer);
    ~PRL_Displayable();

    /// Set a texture alongside with its renderer.
    int set(SDL_Texture *texture, SDL_Renderer *renderer) noexcept;
    /// Get the texture to be displayed.
    SDL_Texture* getTexture() const noexcept;

    /// Make the displayable active or not.
    void makeActive(bool b) noexcept;
    /// Make the displayable static or not (if it can move or not).
    void makeStatic(bool b) noexcept;
    /// Tell whether the displayable object is active (displayed by the displayer) or not.
    bool isActive() const noexcept;
    /// Tell whether the displayable object is static or not.
    bool isStatic() const noexcept;
    /// Set the display angle.
    void setAngle(double angle) noexcept;
    /// Get the display angle.
    double getAngle() const noexcept;
    /// Set the position (upper left corner).
    virtual void setPos(float x, float y) noexcept;
    /// Set the position (upper left corner).
    virtual void setPos(PRL_FPoint pos) noexcept;
    /// Set the position of the center.
    virtual void setCenterPos(float x, float y) noexcept;
    /// Set the position of the center.
    virtual void setCenterPos(PRL_FPoint pos) noexcept;
    /// Get destination rectangle used for display.
    PRL_FRect const& getDstRect() const noexcept;
    /// Set velocity of the displayable in pixels per second.
    void setVelocity(PRL_FPoint const& velocity) noexcept;
    /// Get velocity of the displayable in pixels per second.
    PRL_FPoint const& getVelocity() const noexcept;

    /// Get how many displayable objects are currently in use.
    static int getCount() noexcept;

    struct _collision
    {
	private:
		std::vector <PRL_HitBox> mainHitboxe;
		std::vector <std::vector <PRL_HitBox> > maskHitboxe;
    };
    _collision collision;

protected:
    PRL_FRect dspDst;
    SDL_Texture *dspTexture;
    std::vector <PRL_FRect> dspMaskDst;
    std::vector <SDL_Texture*> dspMaskTexture;
    SDL_Renderer *dspRenderer;
    PRL_FPoint dspVelocity;
    bool dspIsStatic;
    bool dspIsActive;
    double dspAngle; //!> Rotation angle in degrees.

private:
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

    //! Updates the int dst and src (SDL_Rect) FULL rectangles (including w and h) from the float ones. SDL_Rect is currently used by SDL for rendering
    void rect2FRect(PRL_FRect const& frect, SDL_Rect& rect); // from float to int - to be depreciated as long as SDL 2.x uses float as coordinates

#endif // PRL_CLASS_DISPLAYABLE_H_INCLUDED
