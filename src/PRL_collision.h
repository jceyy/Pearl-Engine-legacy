#ifndef PRL_COLLISIONS_FUNCTIONS_INCLUDED
#define PRL_COLLISIONS_FUNCTIONS_INCLUDED

#include <iostream>
#include <thread>
#include <vector>
#include <SDL2/SDL.h>
#include "PRL_types.h"
#include "PRL_defines.h"


// CLASSES //

/// Collision types (flags). They can be customizable like int32_t WALL = PRL_COLLTYPE_0, GROUND = PRL_COLLTYPE_1, PLAYER = PRL_COLLTYPE_2, ...
enum PRL_CollType
{
    PRL_COLLTYPE_0 = 1<<0,
    PRL_COLLTYPE_1 = 1<<1,
    PRL_COLLTYPE_2 = 1<<2,
    PRL_COLLTYPE_3 = 1<<3,
    PRL_COLLTYPE_4 = 1<<4,
    PRL_COLLTYPE_5 = 1<<5,
    PRL_COLLTYPE_6 = 1<<6,
    PRL_COLLTYPE_7 = 1<<7,
    PRL_COLLTYPE_8 = 1<<8,
    PRL_COLLTYPE_9 = 1<<9,
    PRL_COLLTYPE_10 = 1<<10,
    PRL_COLLTYPE_11 = 1<<11,
    PRL_COLLTYPE_12 = 1<<12,
    PRL_COLLTYPE_13 = 1<<13,
    PRL_COLLTYPE_14 = 1<<14,
    PRL_COLLTYPE_15 = 1<<15,
    PRL_COLLTYPE_16 = 1<<16,
    PRL_COLLTYPE_17 = 1<<17,
    PRL_COLLTYPE_18 = 1<<18,
    PRL_COLLTYPE_19 = 1<<19,
    PRL_COLLTYPE_20 = 1<<20,
    PRL_COLLTYPE_21 = 1<<21,
    PRL_COLLTYPE_22 = 1<<22,
    PRL_COLLTYPE_23 = 1<<23,
    PRL_COLLTYPE_24 = 1<<24,
    PRL_COLLTYPE_25 = 1<<25,
    PRL_COLLTYPE_26 = 1<<26,
    PRL_COLLTYPE_27 = 1<<27,
    PRL_COLLTYPE_28 = 1<<28,
    PRL_COLLTYPE_29 = 1<<29,
    PRL_COLLTYPE_30 = 1<<30,
};

/// Collision group (or rule). You can use it as PRL_CollisionGroup group1 = PRL_COLLTYPE_0 | PRL_COLLTYPE_9 | PRL_COLLTYPE_25
typedef uint32_t PRL_CollGroup;

class PRL_HitBox
{
public:
    PRL_HitBox();
    virtual ~PRL_HitBox() = 0;

    virtual PRL_FRect const& getIncludingRect() const noexcept;
};

class PRL_HitBoxRect : public PRL_HitBox
{
public:
	PRL_HitBoxRect(PRL_FRect const& rect);
	~PRL_HitBoxRect();

    PRL_FRect const& getIncludingRect() const noexcept override;
    void get(PRL_FRect& frect) const noexcept;

private:
	PRL_FRect hitboxRect;
};

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

    #if PRL_MULTITHREADING == 1
    std::vector <std::thread> threads;
    #endif // PRL_MULTITHREADING

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
