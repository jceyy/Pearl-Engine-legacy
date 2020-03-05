#ifndef PRL_TYPES_H_INCLUDED
#define PRL_TYPES_H_INCLUDED

#include <SDL2/SDL.h>
#include <type_traits>
#include <typeinfo>
#include <vector>

class PRL_FPoint;
class PRL_DPoint;

//! Class to store a 2D point made out of integer coordinates.
class PRL_Point
{
public:
	PRL_Point();
	PRL_Point(int x_y);
	PRL_Point(int x_, int y_);
	PRL_Point(PRL_Point const& point);
	PRL_Point(PRL_FPoint const& point);
	PRL_Point(PRL_DPoint const& point);
	~PRL_Point();

	void set(int x_, int y_) noexcept;
	int norm2() const noexcept;
	double norm() const noexcept;

    int x, y;
};

//! Class to store a 2D point made out of floating point coordinates.
class PRL_FPoint
{
public:
	PRL_FPoint();
	PRL_FPoint(float x_y);
	PRL_FPoint(float x_, float y_);
	PRL_FPoint(PRL_Point const& point);
	PRL_FPoint(PRL_FPoint const& point);
	PRL_FPoint(PRL_DPoint const& point);
	~PRL_FPoint();

	void set(float x_, float y_) noexcept;
	float norm2() const noexcept;
	double norm() const noexcept;
	void normalize() noexcept;

    float x, y;
};

//! Class to store a 2D point made out of double floating point coordinates.
class PRL_DPoint
{
public:
	PRL_DPoint();
	PRL_DPoint(double x_y);
	PRL_DPoint(double x_, double y_);
	PRL_DPoint(PRL_Point const& point);
	PRL_DPoint(PRL_FPoint const& point);
	PRL_DPoint(PRL_DPoint const& point);
	~PRL_DPoint();

	void set(double x_, double y_) noexcept;
	double norm2() const noexcept;
	double norm() const noexcept;
	void normalize() noexcept;

    double x, y;
};


template<typename T, typename S>
S operator*(T q, S const& p) noexcept
{
	static_assert(std::is_same<S, PRL_Point>::value || std::is_same<S, PRL_FPoint>::value || std::is_same<S, PRL_DPoint>::value, "At least one PRL point type is required");
	static_assert(std::is_arithmetic<T>::value, "Arithmetic type is required");
	return S(p.x * q, p.y * q);
}


PRL_Point const& operator+(PRL_Point const& p, PRL_Point const& q) noexcept;
PRL_Point const& operator+(PRL_Point const& p, PRL_Point const& q) noexcept;
PRL_Point const& operator-(PRL_Point const& p) noexcept;

PRL_FPoint const& operator+(PRL_FPoint const& p, PRL_FPoint const& q) noexcept;
PRL_FPoint const& operator-(PRL_FPoint const& p, PRL_FPoint const& q) noexcept;
PRL_FPoint const& operator-(PRL_FPoint const& p) noexcept;

PRL_DPoint const& operator+(PRL_DPoint const& p, PRL_DPoint const& q) noexcept;
PRL_DPoint const& operator-(PRL_DPoint const& p, PRL_DPoint const& q) noexcept;
PRL_DPoint const& operator-(PRL_DPoint const& p) noexcept;

class PRL_FRect;

//! Class used to store rectangles with integer coordinates.
class PRL_Rect
{
public:
	PRL_Rect();
	PRL_Rect(int x, int y, int w, int h);
	PRL_Rect(PRL_Point const& point, int w, int h);
	PRL_Rect(PRL_Rect const& rect);
	PRL_Rect(PRL_FRect const& rect);
	~PRL_Rect();

	void set(int x, int y) noexcept;
	void set(int x, int y, int w, int h) noexcept;
	void set(PRL_Point const& x_y, PRL_Point const& w_h) noexcept;

    int x, y; //!< Coordinates.
    int w, h; //!< Width and height of the rectangle.
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
	PRL_FRect(PRL_FRect const& rect);
	PRL_FRect(PRL_Rect const& rect);
	~PRL_FRect();

	void set(float x, float y) noexcept;
	void set(float x, float y, int w, int h) noexcept;
	void set(PRL_FPoint const& x_y, PRL_Point const& w_h) noexcept;

	//! Coordinates.
    /*!
    Floating points track much finer coordinates variations than integers and results in smoother motion. Moreover, non-integer coordinates
    can be used in drawing operations: it results in pixel interpolation.
    */
    float x, y;
    //! Width and height of the rectangle.
    /*!
    The variables w and h are kept as integers because in drawing operations they represents an area in pixels on the source or destination "image",
    which can not be parts of a pixel.
    */
    int w, h;
};


//! Class to store a circle made out of integer coordinates and radius.
class PRL_Circle
{
public:
	PRL_Circle(PRL_Point const& center, int r);
	PRL_Circle(int x_, int y_, int r_);
	PRL_Circle();
	~PRL_Circle();

	int x, y; //!< Center of the circle.
	int r; //!< Radius.

	//! Get the center of the circle.
	/*!
	\return Returns the center as a PRL_Point.
	*/
	PRL_Point getCenter() const noexcept;
	PRL_Rect getRect() const noexcept;
	void setCenter(PRL_Point const& center) noexcept;

	static int getCount() noexcept;

private:
	static int cirCount;
};


//! Class to store a circle made out of floating point coordinates and radius.
class PRL_FCircle
{
public:
	PRL_FCircle(PRL_FPoint const& center, float r);
	PRL_FCircle(float x_, float y_, float r_);
	PRL_FCircle();
	~PRL_FCircle();

	float x, y; //!< Center of the circle.
	float r; //!< Radius.

	//! Get the center of the circle.
	/*!
	\return Returns the center as a PRL_FPoint.
	*/
	PRL_FPoint getCenter() const noexcept;
	PRL_FRect getRect() const noexcept;
	void setCenter(PRL_FPoint const& center) noexcept;

	static int getCount() noexcept;

private:
	static int cirCount;
};


//! @brief Class used to store and work with polygons.
//! @details The minimal vertices count is 3.
class PRL_Polygon
{
public:
    //! @brief Constructor.
    //! @details Setting less than 3 vertices will result in an exception throw.
	PRL_Polygon(std::vector <PRL_FPoint> const& vertices_);
	~PRL_Polygon();

    //! @brief Get how many vertices there is in the polygon.
	size_t vertexCount() const noexcept;
    //! @brief Get a specific vertex (no bound check!).
	PRL_FPoint getVertex(size_t which) const;
	//! @brief Get a specific edge (no bound check!).
	//! @details The edge 0 is defined as the edge from vertex 0 to vertex 1.
	PRL_FPoint getEdge(size_t which) const;
	//! @brief Get a specific normal (no bound check!).
	//! @details The normal n is the normalized outside-pointing vector normal to the edge n.
	PRL_FPoint getNormal(size_t which) const;
	//! @brief Get the center of mass of the polygon.
	PRL_FPoint getCenterOfMass() const noexcept;
    //! @brief Get the smallest rectangle in which the whole polygon fits.
	PRL_FRect getRectAround() const noexcept;
    //! @brief Tells if the polygon is convex.
	bool isConvex() const noexcept;
	//! @brief Tells if the polygon is clockwise-oriented.
	bool isClockwise() const noexcept;
    //! @brief Change the vertices.
    //! @details Setting less than 3 vertices will result in an exception throw.
	void setVertices(std::vector <PRL_FPoint> const& vertices_);

    //! @brief Convert a rectangle to a clockwise-oriented polygon.
	static PRL_Polygon rect2poly(PRL_FRect const& rect) noexcept;
	//! @brief Convert a rectangle to a clockwise-oriented polygon.
	static PRL_Polygon rect2poly(PRL_Rect const& rect) noexcept;
	//! @brief Get how many polygons are currently in use.
	static int getCount() noexcept;

private:
	std::vector <PRL_FPoint> vertices;
	std::vector <PRL_FPoint> edges;
	std::vector <PRL_FPoint> normals;
	PRL_FRect rectAround;
	PRL_FPoint centerOfMass;
	bool clockwise;
	bool convex;

	void computeNormals() noexcept;
	void computeEdges() noexcept;
	void computeRect() noexcept;
	void computeCenterOfMass() noexcept;
	void checkClockwise() noexcept;
	void checkConvex() noexcept;
	void computeAll() noexcept;

	static int polyCount;
};


inline int PRL_ScalarProduct(PRL_Point const& p, PRL_Point const& q) noexcept;
inline float PRL_ScalarProduct(PRL_FPoint const& p, PRL_FPoint const& q) noexcept;
inline double PRL_ScalarProduct(PRL_DPoint const& p, PRL_DPoint const& q) noexcept;

//! @brief Get the angle between two unnormalized vectors in a right-handed coordinate system.
//! @details In numerical problems where the vertical axis is flipped, the angle has the opposite sign.
inline double PRL_Angle(PRL_FPoint const& from, PRL_FPoint const& to) noexcept;

//! @brief Get the angle between two normalized vectors in a right-handed coordinate system.
//! @details In numerical problems where the vertical axis is flipped, the angle has the opposite sign.
inline double PRL_AngleNorm(PRL_FPoint const& from, PRL_FPoint const& to) noexcept;

#endif // PRL_TYPES_H_INCLUDED
