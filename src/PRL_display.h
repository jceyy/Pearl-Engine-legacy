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
    friend class PRL_Collider;

	//! @internal Display related part @endinternal
public:
	PRL_Displayable();
    PRL_Displayable(SDL_Texture *texture, SDL_Renderer *renderer);
    ~PRL_Displayable();

    //! @brief Set a texture alongside with its renderer.
    int set(SDL_Texture *texture, SDL_Renderer *renderer) noexcept;
    //! @brief Get the main texture to be displayed. Method to be removed?
    SDL_Texture* getTexture() const noexcept; // add parameter int index = 0?
    //! @brief Make the PRL_Displayable being displayed on the screen.
    void show() noexcept;
    //! @brief Stop the PRL_Displayable from being displayed on the screen.
    void hide() noexcept;
    //! @brief Toggle the visibility of the PRL_Displayable.
    void toggleDisplay() noexcept;
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

    //! @brief Get how many PRL_Displayable are currently in use.
    static int getCount() noexcept;

    void setAlpha(float alpha);
    void setAlpha(float alpha, long long us);

    void setScale(float scale);
    void setScale(float scale, long long us);

    void setAngle(double angle);
    void setAngle(double angle, long long us);

    void setColorMode(SDL_Color const& color);
    void setColorMode(SDL_Color const& color, long long us);


    void showBox() noexcept;
    void hideBox() noexcept;
    void toggleBoxDisplay() noexcept;
    bool isBoxShown() const noexcept;
    int setBoxColor(SDL_Color const& color1, SDL_Color const& color2);
    void setBoxWidth(int width, PRL_Point refRender) noexcept;

    //! @brief Enable collision.
	void enableCol() noexcept;
	//! @brief Disable collision.
	void disableCol() noexcept;
	//! @brief Tell whether collision is enabled or not.
	bool isColEnabled() const noexcept;
	//! @brief Tell if a collision is happening.
	bool isColliding() const noexcept;
	//! @brief Set the collision type.
	void setColType(PRL_ColType type) noexcept;
	//! @brief Set the collision group the collidable belongs to.
	void setColGroup(PRL_ColGroup group) noexcept;
	//! @brief Set collision priority.
	void setColPriority(int priority) noexcept;

	void tempSetIndex(size_t index); //!< TO BE REMOVED

	PRL_ColInfo colInfo; //!< Contains information on collisions concerning this collidable.

protected:
	std::vector <SDL_Texture*> dspTexture; //!< @brief Textures composing the displayable.
    std::vector <PRL_FRect> dspDst; //!< @brief Textures' destination rectangles.
    SDL_Renderer* dspRenderer; //!< @brief Renderer used for the set of textures.

    bool dspEnabled = true; //!< Tells whether the PRL_Displayable is currently displayed or not.
    double dspRotAngle; //!< @brief Rotation angle in degrees.
    PRL_FPoint dspDesiredPos; /// USED in update displayable in animated (anchor point)

    //! @brief Hit boxes composing the object.
	//! @details Hit boxes are stored in a two dimensional array for animations.
	std::vector /*<std::vector */<PRL_HitBox*>  colHitbox;
	//! @brief Hit points composing the object.
	//! @details Hit points are stored in a two dimensional array for animations.
    std::vector /*<std::vector*/ <PRL_FPoint> colHitPoint;

	bool colEnabled = true; //!< @brief Tell whether collision is enabled or not.
	bool colIsColliding; //!< @brief  Tell if a collision is occurring.
	PRL_ColType colType; //!< @brief  Collision type of the object.
	PRL_ColGroup colGroup; //!< @brief  Collision group in which collisions are tested for this object.
	int colPriority; //!< @brief Priority in a collision.

	int addHitBox(PRL_HitBox* hb);
	void addHitPoint(PRL_FPoint const& p);

private:
    //! @brief Whether to show a box around the PRL_Displayable or not.
    bool dspShowBox;
    //! @brief Textures of the box.
    /*! @details In the current implementation, there are only two colors possible: idle and colliding.
    They can be set using the setBoxColors() function, but are set to grey and green per default.*/
    std::vector <SDL_Texture*> dspBoxTexture;
    size_t dspBoxTextureIndex; //! @brief Index of the currently displaying box.
    int dspBoxWidth; //!< @brief Width of the box.
    PRL_Point dspRefRenderRes; //!< @brief Reference render resolution.

    size_t dspDisplayerAddress; //!< @brief Used only by PRL_Displayer, address of the PRL_Displayable.
    bool dspDisplayerAdded; //!< @brief Tell whether the PRL_Displayable has been added to a PRL_Displayer or not.

    int colColliderAddress; //!< @brief Used only by PRL_Collider, address of the PRL_Displayable.
    bool colColliderAdded; //!< @brief Tell if the PRL_Displayable has been added to a PRL_Collider.

    static int dspCount; //!< @brief Number of currently declared PRL_Displayable.


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
	int display(bool enableBoxes = true) const;

	//! @brief NOT USED YET: Display using the current camera.
	int displayWithCamera() const;
	//! @brief NOT USED YET: Set the used camera.
    void setCamera(PRL_Camera* cam);

    //! @brief Get how many PRL_Displayer are currently in use.
    //! @return PRL_Displayer count.
	static int getCount() noexcept;

private:
    //! @brief Compute the destination rectangles for the 4 sides of the box.
    /*! @param[in] The destination rectangle for the PRL_Displayable to be displayed.
        @param[out] A table of size 4 that will be filled with the calculated destination rectangles.
        @param[in] The scaled width (in x and y directions) of the bar composing the box. */
    //! @details This function does not check if the pointers are valid or not.
    void computeBoxDest(const SDL_Rect* dst, SDL_Rect dstbox[], const PRL_Point* scaledWidth) const noexcept;

	std::vector <PRL_Displayable*> displayable; //!< @brief Added objects.
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
