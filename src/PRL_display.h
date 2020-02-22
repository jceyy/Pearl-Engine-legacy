#ifndef PRL_CLASS_DISPLAYABLE_H_INCLUDED
#define PRL_CLASS_DISPLAYABLE_H_INCLUDED

/*!
 @file PRL_display.h
 @author Jean-Clément Ringenbach
*/

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "PRL_defines.h"
#include "PRL_types.h"
#include "PRL_time.h"
#include "PRL_camera.h"
#include "PRL_collision.h"

/*! @internal
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PRL_ Displayable %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 @endinternal
*/

//! @brief This class contains methods for image display, motion and collision.
class PRL_Displayable
{
    friend class PRL_Displayer;

	//! @internal Display related part @endinternal
public:
	PRL_Displayable();
    PRL_Displayable(SDL_Texture *texture, SDL_Renderer *renderer);
    ~PRL_Displayable();

    //! @brief Set a texture alongside with its renderer.
    int set(SDL_Texture *texture, SDL_Renderer *renderer) noexcept;
    //! @brief Get the main texture to be displayed. Method to be removed?
    SDL_Texture* getTexture() const noexcept;
    //! @brief Make the PRL_Displayable being displayed on the screen.
    void enableDisplay() noexcept;
    //! @brief Stop the PRL_Displayable from being displayed on the screen.
    void disableDisplay() noexcept;
    //! @brief Tell whether the PRL_Displayable object is displayed on screen or not.
    bool isDisplayEnabled() const noexcept;
    //! @brief Set the display angle.
    void setRotAngle(double angle) noexcept;
    //! @brief Get the display angle.
    double getRotAngle() const noexcept;
    //! @brief Set the position (upper left corner).
    virtual void setPos(float x, float y) noexcept;
    //! @brief Set the position (upper left corner).
    virtual void setPos(PRL_FPoint const& pos) noexcept;
    //! @brief Set the position of the center.
    virtual void setCenterPos(float x, float y) noexcept;
    //! @brief Set the position of the center.
    virtual void setCenterPos(PRL_FPoint const& pos) noexcept;
    //! @brief Get destination rectangle used for display.
    PRL_FRect const& getDstRect() const noexcept;

    void setAlpha(float alpha);
    void setAlpha(float alpha, long long us);

    void setScale(float scale);
    void setScale(float scale, long long us);

    void setAngle(double angle);
    void setAngle(double angle, long long us);

    void setColorMode(SDL_Color const& color);
    void setColorMode(SDL_Color const& color, long long us);

protected:
    PRL_FRect dspMainDst; //!< @brief Main texture's destination rectangle.
    SDL_Texture* dspMainTexture; //!< @brief Main texture.
    std::vector <PRL_FRect> dspMaskDst; //!< @brief Masks' destination rectangles.
    std::vector <SDL_Texture*> dspMaskTexture; //!< @brief Masks' textures.
    std::vector <SDL_Texture*> dspTexture; //!< @brief Masks' textures.
    std::vector <PRL_FRect> dstDst;
    SDL_Renderer* dspRenderer; //!< @brief Renderer used for the current set of textures.

    bool dspEnabled = true; //!< Tells whether the PRL_Displayable is currently displayed or not.
    double dspRotAngle; //!< @brief Rotation angle in degrees.
    PRL_FPoint dspDesiredPos;

private:
    //! @brief Used only by PRL_Displayer: address of the PRL_Displayable.
    size_t dspDisplayerAddress;
    //! @brief Tell whether the PRL_Displayable has been added to a PRL_Displayer or not.
    bool dspDisplayerAdded;


	//! @internal Motion related part @endinternal
public:
	//! @brief Enable motion.
	void enableMotion() noexcept;
	//! @brief Disable motion but keep velocity value.
	void disableMotion() noexcept;
	//! @brief Tell whether motion is enabled or not.
	bool isMotionEnabled() const noexcept;
	//! @brief Set velocity of the PRL_Displayable in pixels per second.
    void setVelocity(PRL_FPoint const& velocity) noexcept;
    //! @brief Get velocity of the PRL_Displayable in pixels per second.
    PRL_FPoint const& getVelocity() const noexcept;

protected:
	PRL_FPoint dspVelocity; //!< @brief Velocity in pixels per second.
	bool dspMotionEnabled = false; //!< @brief Tell whether motion is enabled or not.


	//! @internal General related part @endinternal
public:
	//! @brief Get how many PRL_Displayable are currently in use.
    static int getCount() noexcept;

private:
    static int dspCount; //!< @brief Number of currently declared PRL_Displayable.
};


/*! @internal
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PRL_ Displayer %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
@endinternal
*/

//! @brief This class is meant to display PRL_Displayable objects.
/*! @details A PRL_Displayer has no idea about the PRL_Displayable's renderer: only
the displayed object knows to which renderer it belongs. Thus a PRL_Displayer can be
filled with objects having different renderers, though it is not recommended for clarity
reasons.
*/
class PRL_Displayer
{
public:
    PRL_Displayer();
    ~PRL_Displayer();

    bool isAdded(PRL_Displayable* dsp) const noexcept;
    //! @brief Add a PRL_Displayable.
    /*!
    @details This function checks whether this particular PRL_Displayable has already been
    added.
    @param[in] dsp The PRL_Displayable object to add.
    @return 0 on success or a negative error code on failure, call PRL_GetError() for more information.
    */
	int add(PRL_Displayable* dsp);
	//! @brief Remove a PRL_Displayable.
    /*!
    @details This function checks whether this particular PRL_Displayable has already been
    added before removing it.
    @param[in] dsp The PRL_Displayable object to remove.
    @return 0 on success or a negative error code on failure, call PRL_GetError() for more information.
    */
	int remove(PRL_Displayable* dsp);
    //! @brief Get how many PRL_Displayable have already been added.
    /*!
    @return Number of added objects.
    */
	int getAddedNumber() const noexcept;
	//! @brief Display all the objects of the PRL_Displayer.
	/*!
	@return 0 on success or a negative error code on failure, call PRL_GetError() for more information.
	*/
	int display() const;

	//! @brief NOT USED YET: Display using the current camera.
	int displayWithCamera() const;
	//! @brief NOT USED YET: Set the used camera.
    void setCamera(PRL_Camera* cam);

    //! @brief Get how many PRL_Displayer are currently in use.
    //! @return PRL_Displayer count.
	static int getDsprCount() noexcept;

private:
	std::vector <PRL_Displayable*> displayable; //!< Added objects.
	PRL_Camera *camera; //!< NOT USED YET: camera.

	static int dsprCount; //!< Count of PRL_Displayer currently in use.
};

//! @brief Update an integer rectangle from a floating point one.
/*!
 @details SDL_Rect is the structure currently in use by SDL2 to specify the drawing area filled
 by a texture on the renderer. Pearl Engine uses floating point coordinates (PRL_FRect) which have
 to be casted to integers so that SDL use them. In the future SDL2 may natively use floating point
 coordinates.
 @param[in] frect floating point rectangle.
 @param[out] integer rectangle.
*/
inline void rect2FRect(PRL_FRect const& frect, SDL_Rect& rect) noexcept; // from float to int - to be depreciated as long as SDL 2.x uses float as coordinates

#endif // PRL_CLASS_DISPLAYABLE_H_INCLUDED
