#include "PRL_types.h"
#include "PRL_configuration.h"
#include <math.h>


// Declaration of PRL_Point

PRL_Point :: PRL_Point() : x(0), y(0)
{
	;
}

PRL_Point :: PRL_Point(int x_y) : x(x_y), y(x_y)
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

PRL_Point :: PRL_Point(PRL_FPoint const& point) : x(static_cast<int>(point.x)), y(static_cast<int>(point.y))
{
	;
}

PRL_Point :: PRL_Point(PRL_DPoint const& point) : x(static_cast<int>(point.x)), y(static_cast<int>(point.y))
{
	;
}

PRL_Point :: ~PRL_Point()
{
	;
}

void PRL_Point :: set(int x_, int y_) noexcept
{
	x = x_;
	y = y_;
}

int PRL_Point :: norm2() const noexcept
{
    return x * x + y * y;
}

double PRL_Point :: norm() const noexcept
{
    return sqrt((double) (x * x + y * y));
}

// Declaration of PRL_FPoint

PRL_FPoint :: PRL_FPoint() : x(0.0), y(0.0)
{
	;
}

PRL_FPoint :: PRL_FPoint(float x_y) : x(x_y), y(x_y)
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

PRL_FPoint :: PRL_FPoint(PRL_Point const& point) : x(static_cast<float>(point.x)), y(static_cast<float>(point.y))
{
	;
}

PRL_FPoint :: PRL_FPoint(PRL_DPoint const& point) : x(static_cast<float>(point.x)), y(static_cast<float>(point.y))
{
	;
}

PRL_FPoint :: ~PRL_FPoint()
{
	;
}

void PRL_FPoint :: set(float x_, float y_) noexcept
{
	x = x_;
	y = y_;
}

float PRL_FPoint :: norm2() const noexcept
{
    return x * x + y * y;
}

double PRL_FPoint :: norm() const noexcept
{
    return sqrt((double) (x * x + y * y));
}

void PRL_FPoint :: normalize() noexcept
{
    double n(norm());
    if (n != 0.0)
    {
        x = x/(float)n;
        y = y/(float)n;
    }
}

// Declaration of PRL_DPoint

PRL_DPoint :: PRL_DPoint() : x(0.0), y(0.0)
{
	;
}

PRL_DPoint :: PRL_DPoint(double x_y) : x(x_y), y(x_y)
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

PRL_DPoint :: PRL_DPoint(PRL_Point const& point) : x(static_cast<double>(point.x)), y(static_cast<double>(point.y))
{
	;
}

PRL_DPoint :: PRL_DPoint(PRL_FPoint const& point) : x(static_cast<double>(point.x)), y(static_cast<double>(point.y))
{
	;
}

PRL_DPoint :: ~PRL_DPoint()
{
	;
}

void PRL_DPoint :: set(double x_, double y_) noexcept
{
	x = x_;
	y = y_;
}

double PRL_DPoint :: norm2() const noexcept
{
    return x * x + y * y;
}

double PRL_DPoint :: norm() const noexcept
{
    return sqrt(x * x + y * y);
}

void PRL_DPoint :: normalize() noexcept
{
    double n(norm());
    if (n != 0.0)
    {
        x = x/n;
        y = y/n;
    }
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

PRL_Rect :: PRL_Rect(PRL_FRect const& rect) : x(static_cast<int>(rect.x)), y(static_cast<int>(rect.y)),
w(rect.w), h(rect.h)
{
	;
}

void PRL_Rect :: set(int x_, int y_) noexcept
{
	x = x_;
	y = y_;
}

void PRL_Rect :: set(int x_, int y_, int w_, int h_) noexcept
{
	x = x_;
	y = y_;
	w = w_;
	h = h_;
}

void PRL_Rect :: set(PRL_Point const& x_y, PRL_Point const& w_h) noexcept
{
    x = x_y.x;
    y = x_y.y;
    w = w_h.x;
    h = w_h.y;
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

PRL_FRect :: PRL_FRect(PRL_FPoint const& point, int w_, int h_) : x(point.x), y(point.y), w(w_), h(h_)
{
	;
}

PRL_FRect :: PRL_FRect(PRL_FRect const& rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h)
{
	;
}

PRL_FRect :: PRL_FRect(PRL_Rect const& rect) : x(static_cast<float>(rect.x)), y(static_cast<float>(rect.y)),
w(rect.w), h(rect.h)
{
	;
}

void PRL_FRect :: set(float x_, float y_) noexcept
{
	x = x_;
	y = y_;
}

void PRL_FRect :: set(float x_, float y_, int w_, int h_) noexcept
{
	x = x_;
	y = y_;
	w = w_;
	h = h_;
}

void PRL_FRect :: set(PRL_FPoint const& x_y, PRL_Point const& w_h) noexcept
{
    x = x_y.x;
    y = x_y.y;
    w = w_h.x;
    h = w_h.y;
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

PRL_Circle :: ~PRL_Circle()
{
	;
}

PRL_Point PRL_Circle :: getCenter() const noexcept
{
	return PRL_Point(x, y);
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

PRL_FCircle :: ~PRL_FCircle()
{
	;
}

PRL_FPoint PRL_FCircle :: getCenter() const noexcept
{
	return PRL_FPoint(x, y);
}


int PRL_Polygon :: polyCount = 0;

int PRL_Polygon :: getCount() noexcept
{
    return polyCount;
}

PRL_Polygon PRL_Polygon :: rect2poly(PRL_FRect const& rect) noexcept
{
    std::vector <PRL_FPoint> points;
    points.push_back(PRL_FPoint(rect.x, rect.y));
    points.push_back(PRL_FPoint(rect.x + (float)rect.w, rect.y));
    points.push_back(PRL_FPoint(rect.x + (float)rect.w, rect.y + (float)rect.h));
    points.push_back(PRL_FPoint(rect.x, rect.y + (float)rect.h));

    return PRL_Polygon(points);
}

PRL_Polygon PRL_Polygon :: rect2poly(PRL_Rect const& rect) noexcept
{
    std::vector <PRL_FPoint> points;
    points.push_back(PRL_FPoint((float)rect.x, (float)rect.y));
    points.push_back(PRL_FPoint((float)(rect.x + rect.w), (float)rect.y));
    points.push_back(PRL_FPoint((float)(rect.x + rect.w), (float)(rect.y + rect.h)));
    points.push_back(PRL_FPoint((float)rect.x, (float)(rect.y + rect.h)));

    return PRL_Polygon(points);
}

PRL_Polygon :: PRL_Polygon()
{
    std::vector <PRL_FPoint> vert(3);
    vert[0] = PRL_FPoint(0.0f, 0.0f);
    vert[1] = PRL_FPoint(1.0f, 0.0f);
    vert[2] = PRL_FPoint(1.0f, 1.0f);

    computeAll();
    polyCount++;
}

PRL_Polygon :: PRL_Polygon(std::vector <PRL_FPoint> const& vertices_)
{
    if (vertices_.size() < 3)
        PRL_SetError("Polygon must have at least 3 points", true);

    vertices = vertices_;
    computeAll();
    polyCount++;
}

PRL_Polygon :: ~PRL_Polygon()
{
    vertices.clear();
    edges.clear();
    normals.clear();
    polyCount--;
}

size_t PRL_Polygon :: vertexCount() const noexcept
{
    return vertices.size();
}

PRL_FPoint PRL_Polygon :: getNormal(size_t which) const
{
    return normals[which];
}

PRL_FPoint PRL_Polygon :: getVertex(size_t which) const
{
    return vertices[which];
}

PRL_FPoint PRL_Polygon :: getCenterOfMass() const noexcept
{
    return centerOfMass;
}

PRL_FRect PRL_Polygon :: getRectAround() const noexcept
{
    return rectAround;
}

bool PRL_Polygon :: isConvex() const noexcept
{
    return convex;
}

bool PRL_Polygon :: isClockwise() const noexcept
{
    return clockwise;
}

void PRL_Polygon :: setVertices(std::vector <PRL_FPoint> const& vertices_)
{
    if (vertices_.size() < 3)
        PRL_SetError("Polygon must have at least 3 points", true);

    vertices = vertices_;
    computeAll();
}

void PRL_Polygon :: computeAll() noexcept
{
    computeCenterOfMass();
    checkClockwise(); // Clockwise important to check before computing normals
    computeRect();
    computeEdges();
    computeNormals();
    checkConvex(); // uses updated normals
}


void PRL_Polygon :: checkClockwise() noexcept
{
    if (PRL_Angle(vertices[0], vertices[1]) > 0)
        clockwise = true;
    else
        clockwise = false;
}

void PRL_Polygon :: checkConvex() noexcept
{
    double angle1(PRL_AngleNorm(normals[0], normals[1])), angle2(PRL_AngleNorm(normals[1], normals[2]));

    for (size_t i(2); i < normals.size() - 1; ++i)
    {
        angle2 = PRL_AngleNorm(normals[i], normals[i + 1]);
        if (angle1 * angle2 < 0.0)
        {
            convex = false;
            break;
        }
        angle1 = angle2;
    }

    angle2 = PRL_AngleNorm(normals[normals.size() - 1], normals[0]);
    if (angle2 * angle1 < 0)
        convex = false;
    else
        convex = true;
}


void PRL_Polygon :: computeNormals() noexcept
{
    if (clockwise)
    {
        for (size_t i(0); i < vertices.size(); ++i)
        {
            normals[i].set(edges[i].y, -edges[i].x);
            normals[i].normalize();
        }
    }
    else
    {
        for (size_t i(0); i < vertices.size(); ++i)
        {
            normals[i].set(-edges[i].y, edges[i].x);
            normals[i].normalize();
        }
    }
}

void PRL_Polygon :: computeEdges() noexcept
{
    edges.clear();
    for (size_t i(0); i < vertices.size() - 1; ++i)
    {
        edges.push_back(PRL_FPoint(vertices[i+1].x - vertices[i].x, vertices[i+1].y - vertices[i].y));
    }
    edges.push_back(PRL_FPoint(vertices[vertices.size() - 1].x - vertices[vertices.size() - 2].x,
                              vertices[vertices.size() - 1].y - vertices[vertices.size() - 2].y));
}

void PRL_Polygon :: computeRect() noexcept
{
    PRL_FPoint pmin(vertices[0]), pmax(vertices[0]);
    for (size_t i(0); i < vertices.size(); ++i)
    {
        if (vertices[i].x < pmin.x)
            pmin.x = vertices[i].x;
        else if (vertices[i].x > pmax.x)
            pmax.x = vertices[i].x;
        if (vertices[i].y < pmin.y)
            pmin.y = vertices[i].y;
        else if (vertices[i].y > pmax.y)
            pmax.y = vertices[i].y;
    }
    rectAround.set(pmin.x, pmin.y, (int)(pmax.x - pmin.x), (int)(pmax.y - pmin.y));
}

void PRL_Polygon :: computeCenterOfMass() noexcept
{
    centerOfMass.set(0.0f, 0.0f);

    for (size_t i(0); i < vertices.size(); ++i)
    {
        centerOfMass.x += vertices[i].x;
        centerOfMass.y += vertices[i].y;
    }

    centerOfMass.x /= vertices.size();
    centerOfMass.y /= vertices.size();
}



PRL_Point point_temp;
PRL_FPoint fpoint_temp;
PRL_DPoint dpoint_temp;

PRL_Point const& operator+(PRL_Point const& p, PRL_Point const& q) noexcept
{
	point_temp.set(p.x + q.x, p.y + q.y);
	return point_temp;
}

PRL_Point const& operator-(PRL_Point const& p, PRL_Point const& q) noexcept
{
	point_temp.set(p.x - q.x, p.y - q.y);
	return point_temp;
}

PRL_Point const& operator-(PRL_Point const& p) noexcept
{
	point_temp.set(-p.x, -p.y);
	return point_temp;
}

PRL_FPoint const& operator+(PRL_FPoint const& p, PRL_FPoint const& q) noexcept
{
	fpoint_temp.set(p.x + q.x, p.y + q.y);
	return fpoint_temp;
}

PRL_FPoint const& operator-(PRL_FPoint const& p, PRL_FPoint const& q) noexcept
{
	fpoint_temp.set(p.x - q.x, p.y - q.y);
	return fpoint_temp;
}

PRL_FPoint const& operator-(PRL_FPoint const& p) noexcept
{
	fpoint_temp.set(-p.x, -p.y);
	return fpoint_temp;
}

PRL_DPoint const& operator+(PRL_DPoint const& p, PRL_DPoint const& q) noexcept
{
	dpoint_temp.set(p.x + q.x, p.y + q.y);
	return dpoint_temp;
}

PRL_DPoint const& operator-(PRL_DPoint const& p, PRL_DPoint const& q) noexcept
{
	dpoint_temp.set(p.x - q.x, p.y - q.y);
	return dpoint_temp;
}

PRL_DPoint const& operator-(PRL_DPoint const& p) noexcept
{
	dpoint_temp.set(-p.x, -p.y);
	return dpoint_temp;
}

int PRL_ScalarProduct(PRL_Point const& p, PRL_Point const& q) noexcept
{
	return p.x * q.x + p.y * q.y;
}

float PRL_ScalarProduct(PRL_FPoint const& p, PRL_FPoint const& q) noexcept
{
	return p.x * q.x + p.y * q.y;
}

double PRL_ScalarProduct(PRL_DPoint const& p, PRL_DPoint const& q) noexcept
{
	return p.x * q.x + p.y * q.y;
}

double PRL_Angle(PRL_FPoint const& a_from, PRL_FPoint const& b_to) noexcept
{
    double nprod = a_from.norm() * b_to.norm();
    return asin((double)(a_from.x * b_to.y /nprod - a_from.y * b_to.x / nprod));
}

double PRL_AngleNorm(PRL_FPoint const& a_from, PRL_FPoint const& b_to) noexcept
{
    return asin((double)(a_from.x * b_to.y - a_from.y * b_to.x));
}
