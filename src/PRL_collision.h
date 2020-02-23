#ifndef PRL_COLLISIONS_FUNCTIONS_INCLUDED
#define PRL_COLLISIONS_FUNCTIONS_INCLUDED

#include <iostream>
#include <thread>
#include <vector>
#include <SDL2/SDL.h>
#include "PRL_types.h"
#include "PRL_defines.h"


// CLASSES //

//! @brief Collision types (flags).
/*! @details Types can be used in a custom way, as int32_t WALL = PRL_COLLTYPE_0,
GROUND = PRL_COLLTYPE_1, PLAYER = PRL_COLLTYPE_2, ...
*/
enum PRL_ColType : uint32_t
{
    PRL_COLTYPE_0 = 1<<0,
    PRL_COLTYPE_1 = 1<<1,
    PRL_COLTYPE_2 = 1<<2,
    PRL_COLTYPE_3 = 1<<3,
    PRL_COLTYPE_4 = 1<<4,
    PRL_COLTYPE_5 = 1<<5,
    PRL_COLTYPE_6 = 1<<6,
    PRL_COLTYPE_7 = 1<<7,
    PRL_COLTYPE_8 = 1<<8,
    PRL_COLTYPE_9 = 1<<9,
    PRL_COLTYPE_10 = 1<<10,
    PRL_COLTYPE_11 = 1<<11,
    PRL_COLTYPE_12 = 1<<12,
    PRL_COLTYPE_13 = 1<<13,
    PRL_COLTYPE_14 = 1<<14,
    PRL_COLTYPE_15 = 1<<15,
    PRL_COLTYPE_16 = 1<<16,
    PRL_COLTYPE_17 = 1<<17,
    PRL_COLTYPE_18 = 1<<18,
    PRL_COLTYPE_19 = 1<<19,
    PRL_COLTYPE_20 = 1<<20,
    PRL_COLTYPE_21 = 1<<21,
    PRL_COLTYPE_22 = 1<<22,
    PRL_COLTYPE_23 = 1<<23,
    PRL_COLTYPE_24 = 1<<24,
    PRL_COLTYPE_25 = 1<<25,
    PRL_COLTYPE_26 = 1<<26,
    PRL_COLTYPE_27 = 1<<27,
    PRL_COLTYPE_28 = 1<<28,
    PRL_COLTYPE_29 = 1<<29,
    PRL_COLTYPE_30 = 1<<30,
};

//! @brief Collision group (or rule).
//! @details You can use it as PRL_CollisionGroup group1 = PRL_COLLTYPE_0 | PRL_COLLTYPE_9 | PRL_COLLTYPE_25
typedef uint32_t PRL_ColGroup;

//! @brief Enumeration containing all the hit box types
/*! @details The different hit box types are:
- rectangular (PRL_HBTYPE_RECT), the hit box is a rectangle stored as a PRL_FRect
- circle (PRL_HITBOX_CIRCLE), the hit box is a circle, stored as a PRL_FCircle
- polygon (PRL_HITBOX_POLYGON), the hit box is an arbitrary shape, described by points
arranged in a certain order, and stored as an PRL_FPointList
*/
enum PRL_HBType
{
    PRL_HBTYPE_RECT, PRL_HBTYPE_CIRCLE, PRL_HBTYPE_POLY
};

//! @brief Abstract class describing hit boxes.
/*! @details The different types of hit boxes derive from this class and override most of its methods.
*/
class PRL_HitBox
{
	friend inline bool PRL_TestCollision(PRL_HitBox const& hitbox1, PRL_HitBox const& hitbox2) noexcept;
public:
    PRL_HitBox();
    virtual ~PRL_HitBox() = 0;

    //! @brief Get the smallest rectangle surrounding the hit box.
    virtual PRL_FRect const& getRectAround() const noexcept;
    //! @brief Get the center of mass of the hit box (in local coordinates).
    virtual PRL_FPoint const& getCenterOfMass() const noexcept;
    //! @brief Get the type of the hit box.
    PRL_HBType getType() const noexcept;
    //! @brief Get the count of currently used hit boxes (of any type).
    static int getCount() noexcept;

protected:
	PRL_FPoint centerOfMass; //!< Center of mass of the hit box.
	PRL_HBType type; //!< Actual type of the hit box.
	PRL_FRect rectAround; //!< Smaller possible rectangle in which all points fit.

private:
    //! @brief Compute the center of mass of the hit box.
    /*! @details Can be used only internally to update the center of mass when the hit box is updated,
    for instance when points are added to a polygon hit box or simply when the hit box has been changed.*/
	virtual void computeCOM() noexcept;
	static int hbCount; //!< Count of hit boxes, without distinction.
};

class PRL_HBRect : public PRL_HitBox
{
public:
	PRL_HBRect(PRL_FRect const& rect);
	~PRL_HBRect();

	//! @brief Simply return the rectangle, same function as get().
    PRL_FRect const& getRectAround() const noexcept override;
    //! @brief Set a new rectangle hit box.
    //! @details This will automatically result in a calculation of the new center of mass.
    void set(PRL_FRect const& rect) noexcept;
    //! @brief Get the rectangle.
    PRL_FRect const& get() const noexcept;
    //! @brief Get the center of mass of the rectangle (in local coordinates).
    PRL_FPoint const& getCenterOfMass() const noexcept override;

    //! @brief Get the count of currently used rectangle hit boxes.
    static int getCount() noexcept;

private:
    //! @brief Compute the center of mass of the rectangle.
    //! @details This is only called when a new hit box is set and in the constructor.
	void computeCOM() noexcept override;
	static int hbRectCount; //!< Count of rectangle hit boxes.
};


class PRL_Collidable;

//! @brief Class capable of storing informations about a specific collision.
class PRL_ColInfo
{
	friend class PRL_Collider;
public:
    PRL_ColInfo();
    ~PRL_ColInfo();

    //! @brief Vector containing all the hit boxes of a collidable that collided.
    /*! @details It can contain the same hit box several times, since every element corresponds to an element of
    the target vector.
    */
    std::vector<int> involvedHitBox;
    //! @brief Vector containing all target collidables of the collision event involving the corresponding hit box.
    std::vector<PRL_Collidable*> target;

    //! @brief Get the count of currently used PRL_ColInfo classes.
    static int getCount() noexcept;

private:
	static int colInfCount; //!< Count of collision information classes.
	//! @brief Clear the vectors of the class.
	void clear() noexcept;
};

//! @brief Abstract class introducing the collision properties of objects.
class PRL_Collidable
{
	friend class PRL_Collider;
public:
	PRL_Collidable();
	virtual ~PRL_Collidable() = 0;

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
	void setColPriority(int priority);
    //! @brief Get the count of PRL_Collidable currently in use.
	static int getCount() noexcept;

	int addHitBox(PRL_HitBox* hb);
	void addHitPoint(PRL_FPoint const& p);

	PRL_ColInfo colInfo; //!< Contains information on collisions concerning this collidable.

protected:
	//! @brief Hit boxes composing the collidable.
	//! @details Hit boxes are stored in a two dimensional array for animations.
	std::vector /*<std::vector */<PRL_HitBox*>  colHitbox;
	//! @brief Hit points composing the collidable.
	//! @details Hit points are stored in a two dimensional array for animations.
    std::vector /*<std::vector*/ <PRL_FPoint> colHitPoint;

	bool colEnabled = true; //!< Tell whether collision is enabled or not.
	bool colIsColliding; //!< Tell if a collision is occurring.
	PRL_ColType colType; //!< Collision type of the collidable.
	PRL_ColGroup colGroup; //!< Collision group in which collisions are tested for this collidable.
	int colPriority; //!< Priority in a collision.

private:
	int collider_address; //!< Address within th collider (only used by the collider).
	static int colCount; //!< Count of collidables currently in use.

	//std::vector <std::vector <PRL_HitBox*> > dspMaskHitbox; !< brief Mask textures' corresponding hit boxes.
};


class PRL_Collider
{
public:
    PRL_Collider();
    ~PRL_Collider();

    //! @brief Add a collidable to the collider.
    //! @details Does nothing if the collidable has already been added.
    void add(PRL_Collidable *collidable);
    //! @brief Remove a collidable from the collider.
    //! @details Does nothing if the collidable has not been added.
    void remove(PRL_Collidable *collidable);
    //! @brief Test the collisions between the different collidables added to the collider
    void testCollisions() const noexcept;
    //! @brief Get the count of PRL_Collider currently in use.
    static int getCount() noexcept;

private:
	//! @brief Vector containing all the added collidables.
    std::vector <PRL_Collidable*> collidable;
    //! @brief Check whether the collidable is already added to the collider.
    inline bool isAdded(PRL_Collidable *col) const noexcept;

    static int colCount; //!< Number of colliders currently in use.
};

inline bool PRL_TestCollision(PRL_HitBox const& hitbox1, PRL_HitBox const& hitbox2) noexcept;
inline bool PRL_TestCollision(PRL_FPoint const& point, PRL_FRect const& rect) noexcept;
inline bool PRL_TestCollision(PRL_FRect const& rect1, PRL_FRect const& rect2) noexcept;


/* ********************************************* */
/*                PRL_CollInfos                  */
/* ********************************************* */

//class PRL_Collidable; // simple class prototype in order to use a pointer on a PRL_Collidable object

// Contain infos about a collision caused by a point or a hit box to a single collidable
/*class PRL_CollInfos
{
public:
    PRL_CollInfos();
    ~PRL_CollInfos();

    int srcNumber; // Collision's source point or hit box number. REDUNDANT
    int hitBox_target; // Target hit box in collision
    PRL_Collidable *collidable_target; // Target collidable in collision
};*/



/* ********************************************* */
/*                PRL_Movable                    */
/* ********************************************* */
/*
class PRL_Movable
{
private:
    PRL_FPoint pxlsPerSec_;
    PRL_FPoint previousDisplacement_;
    long long previous_time;
    long long current_time;
    PRL_FRect *display_dst_copy;

    static int movable_count;

public:
    PRL_Movable(PRL_FRect *display_dst_rect);
    ~PRL_Movable();

    /// Set the displacement vector, in pixels per second
    void setVelocityVector(PRL_FPoint const& pxlsPerSec);
    /// Get the current displacement vector
    PRL_FPoint getVelocityVector() const;
    /// Apply the current displacement vector to the display destination rect. Do not forget to update the timer_GLOBAL before!
    virtual void applyDisplacement();

    int getMvbCount();

    static float time_coeff; // used in computations
};*/



/* ********************************************* */
/*               PRL_Collidable                  */
/* ********************************************* */

/// Class used to construct objects capable of being collided with each others. Default collidable type: PRL_COLLTYPE_0, dynamic: true
/*class PRL_Collidable : public PRL_AnimatedSimple
{
    friend class PRL_Collider;

private:
    int collider_address;
    //int *current_frame_display;
    //PRL_FRect *display_dst_copy_collidable; // pointer on the displayable destination rect

    std::vector <std::vector <PRL_CollInfos>> pointCollisions; // Array containing informations for collisions between source action points and other collidables's hit boxes
    std::vector <std::vector <PRL_CollInfos>> hitBoxCollisions; // Array containing informations for collisions between source hit boxes and other collidables's hit boxes
    // The first index represents the concerned source action point / hit box, the second one goes up to the number of different collidables hit by the concerned AP / HB

protected:
    bool coll_isDynamic_; // If dynamic, collisions are checked from this object. Else, collisions are checked to this object from dynamic ones.
    PRL_CollType collidableType;
    PRL_FPointCluster *relativeCluster; // Pointer on the relative point cluster (found in the animation)
    PRL_FPointCluster absoluteCluster; // Point cluster used for collisions

    /// Update absolute cluster from current display destination rectangle and the pointed relative cluster
    void updateAbsolutePointCluster();

public:
    /// Provide a pointer on the display rect and a pointer on the variable in which is stored the current frame number of the animation
    PRL_Collidable(PRL_FRect *display_dst_rect);
    ~PRL_Collidable();

    /// Make the collidable static
    void makeCollStatic();
    /// Make the collidable dynamic
    void makeCollDynamic();
    /// Make the collidable dynamic
    void makeCollDynamic(bool b);
    /// Tell whether or not the collidable is dynamic. By default: true
    bool isCollDynamic() const;
    /// Set the collidable type
    void setCollidableType(PRL_CollType type);
    /// Set a pointer on the current animation to have the right relative point cluster
    void setCurrentAnimation(const PRL_AnimationSimple* animation);
    /// Set a pointer on the variable containing the current frame displayed
    void setCurrentFramePointer(const int* current_frame);
    /// Get the number of action points at each frame
    int getActionPointsNumber() const;
    /// Get the number of hit boxes at each frame
    int getHitBoxesNumber() const;
};



* ********************************************* *
*                PRL_Collider                   *
* ********************************************* *

class PRL_Collider
{
private:
    std::vector <PRL_Collidable*> collidables;
    std::vector <PRL_CollGroup> collisionGroups;
    /// Check whether the collidable is already added to the collider
    bool isAdded(PRL_Collidable *collidable) const;
    /// Check whether the collision group is already added to the collider
    bool isAdded(PRL_CollGroup collGroup) const;
    /// Test collisions between 2 collidables (method used in testCollisions) without any concern about i == j
    void testCollisionsBetween(int i, int j);

public:
    PRL_Collider();
    ~PRL_Collider();

    /// Add a collidable to the collider
    void add(PRL_Collidable *collidable);
    /// Remove a collidable from the collider
    void remove(PRL_Collidable *collidable);
    /// Test the collisions between the different collidables added to the collider
    void testCollisions();
    /// Add a collision group
    void addCollGroup(PRL_CollGroup group);
};




// GEOMETRIC FUNCTIONS //

PRL_Rect GetBoxAroundCircle(const PRL_Circle C);
PRL_Point GetPointFromCoord(const int x, const int y);
PRL_Point GetProjectedPointI(const PRL_Point A, const PRL_Point B, const PRL_Point C);
PRL_Vector GetNormalFromSegment(const PRL_Point A, const PRL_Point B, const PRL_Point C);
PRL_Vector GetReboundVector(const PRL_Vector v, const PRL_Vector N);
bool ProjectionOnSegment(const PRL_Point A, const PRL_Point B, const PRL_Point C);

// COLLISION FUNCTIONS PROTOTPYES //

/// Test collision between a point and a rectangle (unrotated)
bool PRL_Coll_PointRect(PRL_FPoint const& p, PRL_FRect const& box);
/// Test collision between a point and a rectangle (unrotated)
bool PRL_Coll_PointRect(PRL_Point const& p, PRL_FRect const& box);
/// Test collision between a point and a rectangle (unrotated)
bool PRL_Coll_PointRect(PRL_FPoint const& p, PRL_Rect const& box);
/// Test collision between a point and a rectangle (unrotated)
bool PRL_Coll_PointRect(PRL_Point const& p, PRL_Rect const& box);

/// Test collision between 2 rectangles (unrotated)
bool PRL_Coll_2Rects(PRL_FRect const& box1, PRL_FRect const& box2);
/// Test collision between 2 rectangles (unrotated)
bool PRL_Coll_2Rects(PRL_Rect const& box1, PRL_FRect const& box2);
/// Test collision between 2 rectangles (unrotated)
bool PRL_Coll_2Rects(PRL_FRect const& box1, PRL_Rect const& box2);
/// Test collision between 2 rectangles (unrotated)
bool PRL_Coll_2Rects(PRL_Rect const& box1, PRL_Rect const& box2);

/// Collision function that test collision between 2 hit boxes
bool PRL_Coll_2HitBoxes(PRL_HitBox const& hitbox1, PRL_HitBox const& hitbox2);

/// Collision function that test collision between a point and an hit box
bool PRL_Coll_HitBoxPoint(PRL_HitBox const& hitbox, PRL_FPoint const& point);

*/




#endif // PRL_COLLISIONS_FUNCTIONS_INCLUDED
