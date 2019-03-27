#include "PRL_types.h"


// Declaration of PRL_Point

PRL_Point :: PRL_Point() : x(0), y(0)
{
	;
}

PRL_Point :: PRL_Point(int x_, int y_) : x(x_), y(y_)
{
	;
}

PRL_Point :: PRL_Point(PRL_Point const& point) : x(point.x), y(point.y)
{
	;
}

PRL_Point :: ~PRL_Point()
{
	;
}

// Declaration of PRL_FPoint

PRL_FPoint :: PRL_FPoint() : x(0.0), y(0.0)
{
	;
}

PRL_FPoint :: PRL_FPoint(float x_, float y_) : x(x_), y(y_)
{
	;
}

PRL_FPoint :: PRL_FPoint(PRL_FPoint const& fpoint) : x(fpoint.x), y(fpoint.y)
{
	;
}

PRL_FPoint :: ~PRL_FPoint()
{
	;
}

// Declaration of PRL_DPoint

PRL_DPoint :: PRL_DPoint() : x(0.0), y(0.0)
{
	;
}

PRL_DPoint :: PRL_DPoint(double x_, double y_) : x(x_), y(y_)
{
	;
}

PRL_DPoint :: PRL_DPoint(PRL_DPoint const& dpoint) : x(dpoint.x), y(dpoint.y)
{
	;
}

PRL_DPoint :: ~PRL_DPoint()
{
	;
}

// Declaration of PRL_Rect

PRL_Rect :: PRL_Rect() : x(0.0), y(0.0), w(0), h(0)
{
	;
}

PRL_Rect :: PRL_Rect(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_)
{
	;
}

PRL_Rect :: PRL_Rect(PRL_Point const& point, int w_, int h_) : x(point.x), y(point.y), w(w_), h(h_)
{
	;
}

PRL_Rect :: PRL_Rect(PRL_Rect const& rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h)
{
	;
}

PRL_Rect :: ~PRL_Rect()
{
	;
}

// Declaration of PRL_FRect

PRL_FRect :: PRL_FRect() : x(0.0), y(0.0), w(0), h(0)
{
	;
}

PRL_FRect :: PRL_FRect(float x_, float y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_)
{
	;
}

PRL_FRect :: PRL_FRect(PRL_FPoint const& fpoint, int w_, int h_) : x(fpoint.x), y(fpoint.y), w(w_), h(h_)
{
	;
}

PRL_FRect :: PRL_FRect(PRL_FRect const& frect) : x(frect.x), y(frect.y), w(frect.w), h(frect.h)
{
	;
}

PRL_FRect :: ~PRL_FRect()
{
	;
}

// Declaration of PRL_Circle

PRL_Circle :: PRL_Circle() : x(0), y(0), r(0)
{
	;
}

PRL_Circle :: PRL_Circle(int x_, int y_, int r_) : x(x_), y(y_), r(r_)
{
	;
}

PRL_Circle :: PRL_Circle(PRL_Point const& center, int r_) : x(center.x), y(center.y), r(r_)
{
	;
}

PRL_Circle :: PRL_Circle(PRL_Circle const& circle) : x(circle.x), y(circle.y), r(circle.r)
{
	;
}

PRL_Circle :: ~PRL_Circle()
{
	;
}

PRL_Point PRL_Circle :: getCenter() const
{
	PRL_Point p(x, y);
	return p;
}

// Declaration of PRL_FCircle

PRL_FCircle :: PRL_FCircle() : x(0.0), y(0.0), r(0.0)
{
	;
}

PRL_FCircle :: PRL_FCircle(float x_, float y_, float r_) : x(x_), y(y_), r(r_)
{
	;
}

PRL_FCircle :: PRL_FCircle(PRL_FPoint const& center, float r_) : x(center.x), y(center.y), r(r_)
{
	;
}

PRL_FCircle :: PRL_FCircle(PRL_FCircle const& fcircle) : x(fcircle.x), y(fcircle.y), r(fcircle.r)
{
	;
}

PRL_FCircle :: ~PRL_FCircle()
{
	;
}

PRL_FPoint PRL_FCircle :: getCenter() const
{
	PRL_FPoint p(x, y);
	return p;
}
