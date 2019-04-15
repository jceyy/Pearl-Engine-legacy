#ifndef PRL_TYPES_H_INCLUDED
#define PRL_TYPES_H_INCLUDED

#include <SDL2/SDL.h>



//! Class to store a 2D point made out of integer coordinates.
class PRL_Point
{
public:
	PRL_Point();
	PRL_Point(int x_, int y_);
	PRL_Point(PRL_Point const& point);
	~PRL_Point();

	void set(int x_, int y_);

    int x, y;
};

//! Class to store a 2D point made out of floating point coordinates.
class PRL_FPoint
{
public:
	PRL_FPoint();
	PRL_FPoint(float x_, float y_);
	PRL_FPoint(PRL_FPoint const& fpoint);
	~PRL_FPoint();

	void set(float x_, float y_);

    float x, y;
};

//! Class to store a 2D point made out of double floating point coordinates.
class PRL_DPoint
{
public:
	PRL_DPoint();
	PRL_DPoint(double x_, double y_);
	PRL_DPoint(PRL_DPoint const& dpoint);
	~PRL_DPoint();

	void set(double x_, double y_);

    double x, y;
};


//! Class used to store rectangles with integer coordinates.
class PRL_Rect
{
public:
	PRL_Rect();
	PRL_Rect(int x_, int y_, int w_, int h_);
	PRL_Rect(PRL_Point const& point, int w_, int h_);
	PRL_Rect(PRL_Rect const& rect);
	~PRL_Rect();

    int x, y; //!< Coordinates.
    int w, h; //!< Weight and height of the rectangle.
};


//! Class used to store rectangles with floating point coordinates.
/*!
Use this class with the Pearl Engine whenever you want to use coordinates as with SDL_Rect in native SDL: it is used in the same way
with the difference that coordinates are not integers but floating point number.
*/
class PRL_FRect
{
public:
	PRL_FRect();
	PRL_FRect(float x_, float y_, int w_, int h_);
	PRL_FRect(PRL_FPoint const& fpoint, int w_, int h_);
	PRL_FRect(PRL_FRect const& frect);
	~PRL_FRect();

	//! Coordinates.
    /*!
    Floating points track much finer coordinates variations than integers and results in smoother motion. Moreover, non-integer coordinates
    can be used in drawing operations: it results in pixel interpolation.
    */
    float x, y;
    //! Weight and height of the rectangle.
    /*!
    The variables w and h are kept as integers because in drawing operations they represents an area in pixels on the source or destination "image",
    which can not be parts of a pixel.
    */
    int w, h;
};


//! Class to store a circle made out of integer coordinates and radius.
/*!
\note In Pearl Engine it is mainly used for rendering purposes, so the center and radius are mostly expressed in pixels.
*/
class PRL_Circle
{
public:
	PRL_Circle();
	PRL_Circle(int x_, int y_, int r_);
	PRL_Circle(PRL_Point const& center, int r_);
	PRL_Circle(PRL_Circle const& circle);
	~PRL_Circle();

	//! Get the center of the circle.
	/*!
	\return Returns the center as a PRL_Point.
	*/
	PRL_Point getCenter() const;

    int x, y; //!< Center of the circle.
    int r; //!< Radius.
};

//! Class to store a circle made out of floating point coordinates and radius.
/*!
\note In Pearl Engine it is mainly used for rendering purposes, so the center and radius are mostly expressed in pixels.
*/
class PRL_FCircle
{
public:
	PRL_FCircle();
	PRL_FCircle(float x_, float y_, float r_);
	PRL_FCircle(PRL_FPoint const& center, float r_);
	PRL_FCircle(PRL_FCircle const& fcircle);
	~PRL_FCircle();

	//! Get the center of the circle.
	/*!
	\return Returns the center as a PRL_Point.
	*/
	PRL_FPoint getCenter() const;

    float x, y; //!< Center of the circle.
    float r; //!< Radius.
};

inline int PRL_ScalarProduct(PRL_Point p, PRL_Point q) noexcept;
inline float PRL_ScalarProduct(PRL_FPoint p, PRL_FPoint q) noexcept;
inline double PRL_ScalarProduct(PRL_DPoint p, PRL_DPoint q) noexcept;

#endif // PRL_TYPES_H_INCLUDED
