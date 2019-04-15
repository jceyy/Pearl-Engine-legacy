#include <iostream>
#include <typeinfo>
#include <thread>
#include <vector>
#include <math.h>

#include "PRL_collision.h"
#include "PRL_global.h"

using std::cout;
using std::cerr;
using std::endl;


PRL_HitBox :: PRL_HitBox()
{
	;
}

PRL_HitBox :: ~PRL_HitBox()
{
	;
}

PRL_FRect _frect_temp;
PRL_FRect const& PRL_HitBox :: getIncludingRect() const noexcept
{
	return _frect_temp;
}

PRL_FPoint _fpoint_temp;
PRL_FPoint const& PRL_HitBox :: getCenterOfMass() const noexcept
{
	return _fpoint_temp;
}

void PRL_HitBox :: computeCOM() noexcept
{
    ;
}


PRL_HitBoxRect :: PRL_HitBoxRect(PRL_FRect const& rect)
{
	includingFRect = rect;
}

PRL_HitBoxRect :: ~PRL_HitBoxRect()
{
	;
}

PRL_FRect const& PRL_HitBoxRect :: getIncludingRect() const noexcept
{
	return includingFRect;
}

void PRL_HitBoxRect :: set(PRL_FRect const& rect) noexcept
{
	includingFRect = rect;
}

PRL_FRect const& PRL_HitBoxRect :: get() const noexcept
{
	return includingFRect;
}

PRL_FPoint const& PRL_HitBoxRect :: getCenterOfMass() const noexcept
{
	return centerOfMass;
}

void PRL_HitBoxRect :: computeCOM() noexcept
{
	centerOfMass.x = (includingFRect.x + (float)includingFRect.w)/2.0f;
	centerOfMass.y = (includingFRect.y + (float)includingFRect.h)/2.0f;
}


typedef struct PRL_Vector PRL_Vector; // to be depreciated
struct PRL_Vector
{
    double x, y;
};

/// GEOMETRIC FUNCTIONS ///

PRL_Rect GetBoxAroundCircle(const PRL_Circle C)/// Get the AABB box around a circle
{
    PRL_Rect box;
    box.x=C.x-C.r;
    box.y=C.y-C.r;
    box.w=box.h=2*C.r;
    return box;
}

PRL_Point GetPointFromCoord(const int x, const int y)/// Returns a PRL_Point from 2 coordinates
{
    PRL_Point p;
    p.x=x;
    p.y=y;
    return p;
}

PRL_Point GetProjectedPointI(const PRL_Point A, const PRL_Point B, const PRL_Point C)/// Orthogonal projection AI of AC on AB
{
      PRL_Vector u,AC;
      u.x = B.x - A.x;
      u.y = B.y - A.y;
      AC.x = C.x - A.x;
      AC.y = C.y - A.y;
      double ti = (u.x*AC.x + u.y*AC.y)/(u.x*u.x + u.y*u.y);
      PRL_Point I;
      I.x = A.x + ti*u.x;
      I.y = A.y + ti*u.y;
      return I;
}

PRL_Vector GetNormalFromSegment(const PRL_Point A, const PRL_Point B, const PRL_Point C)/// Returns the normal in the direction of the point C
{
      PRL_Vector AC,u,N;
      u.x = B.x - A.x;
      u.y = B.y - A.y;
      AC.x = C.x - A.x;
      AC.y = C.y - A.y;
      double det = u.x*AC.y-u.y*AC.x;
      N.x = -u.y*(det);
      N.y = u.x*(det);
      double norm = sqrt(N.x*N.x + N.y*N.y);
      N.x/=norm;
      N.y/=norm;
      return N;
}

PRL_Vector GetReboundVector(const PRL_Vector v, const PRL_Vector N)/// Returns a PRL_Vector giving the new movement's tangent after a collision.
{
  PRL_Vector v2;
  double scalp = (v.x*N.x +  v.y*N.y);
  v2.x = v.x -2*scalp*N.x;
  v2.y = v.y -2*scalp*N.y;
  return v2;
}

bool ProjectionOnSegment(const PRL_Point A, const PRL_Point B, const PRL_Point C) /// Is the orthogonal projection of C on AB between A and B?
{
    int ACx = C.x-A.x;
    int ACy = C.y-A.y;
    int ABx = B.x-A.x;
    int ABy = B.y-A.y;
    int BCx = C.x-B.x;
    int BCy = C.y-B.y;
    int s1 = (ACx*ABx) + (ACy*ABy);
    int s2 = (BCx*ABx) + (BCy*ABy);
    if (s1*s2>0)
        return false;
    return true;
}

/// COLLISION FUNCTIONS ///

// There is 4 overloads of this function: PRL_Coll_PointRect

bool PRL_Coll_PointRect(PRL_FPoint const& p, PRL_FRect const& box)
{
    if (p.x >= box.x && p.x < box.x + (float) box.w && p.y >= box.y && p.y < box.y + (float) box.h)
        return true;
    else
        return false;
}

bool PRL_Coll_PointRect(PRL_FPoint const& p, PRL_Rect const& box)
{
    if (p.x >= (float) box.x && p.x < (float) box.x + (float) box.w && p.y >= (float) box.y && p.y < (float) box.y + (float) box.h)
        return true;
    else
        return false;
}

bool PRL_Coll_PointRect(PRL_Point const& p, PRL_FRect const& box)
{
    if ((float) p.x >= box.x && (float) p.x < box.x + (float) box.w && (float) p.y >= box.y && (float) p.y < box.y + (float) box.h)
        return true;
    else
        return false;
}

bool PRL_Coll_PointRect(PRL_Point const& p, PRL_Rect const& box)
{
    if (p.x >= box.x && p.x < box.x + box.w && p.y >= box.y && p.y < box.y + box.h)
        return true;
    else
        return false;
}

// There is 4 overloads of this function: PRL_Coll_2Rects

bool PRL_Coll_2Rects(PRL_Rect const& box1, PRL_Rect const& box2)
{
    if ((box2.x >= box1.x + box1.w) || (box2.x + box2.w <= box1.x) || (box2.y >= box1.y + box1.h) || (box2.y + box2.h <= box1.y))
        return false;
    else
        return true;
}

bool PRL_Coll_2Rects(PRL_FRect const& box1, PRL_FRect const& box2)
{
    if ((box2.x >= box1.x + (float) box1.w) || (box2.x + (float) box2.w <= box1.x) || (box2.y >= box1.y + (float) box1.h) || (box2.y + (float) box2.h <= box1.y))
        return false;
    else
        return true;
}

bool PRL_Coll_2Rects(PRL_FRect const& box1, PRL_Rect const& box2)
{
    if (((float) box2.x >= box1.x + (float) box1.w) || ((float) box2.x + (float) box2.w <= box1.x) || ((float) box2.y >= box1.y + (float) box1.h) || ((float) box2.y + (float) box2.h <= box1.y))
        return false;
    else
        return true;
}

bool PRL_Coll_2Rects(PRL_Rect const& box1, PRL_FRect const& box2)
{
    if ((box2.x >= (float) box1.x + (float) box1.w) || (box2.x + (float) box2.w <= (float) box1.x) || (box2.y >= (float) box1.y + (float) box1.h) || (box2.y + (float) box2.h <= (float) box1.y))
        return false;
    else
        return true;
}

bool PRL_Coll_2Circles(PRL_FCircle const& C1, PRL_FCircle const& C2)
{
    float a(C1.x - C2.x), b(C1.y - C2.y), c(C1.r + C2.r); // a*a + b*b is the distance between the 2 centers
    if (a*a + b*b > c*c)
        return false;
    else
        return true;
}

int PRL_Coll_2Segments(PRL_FPoint const& A, PRL_FPoint const& B, PRL_FPoint const& I, PRL_FPoint const& P)/// [AB] and [IP] segments
{
    PRL_Vector D,E;
    D.x = B.x - A.x;
    D.y = B.y - A.y;
    E.x = P.x - I.x;
    E.y = P.y - I.y;
    double denominator = D.x*E.y - D.y*E.x;
    if (denominator==0)
        return -1;   // error, limit case
    int t = - (A.x*E.y-I.x*E.y-E.x*A.y+E.x*I.y) / denominator;
    if (t<0 || t>=1)
        return 0;
    int u = - (-D.x*A.y+D.x*I.y+D.y*A.x-D.y*I.x) / denominator;
    if (u<0 || u>=1)
        return 0;
    return 1;
}

/*bool PRL_Coll_PointPolygon(PRL_FPoint const& p, PRL_FPointList const& polygon)//General function, for all polygons
{   // If the segment between a point p in the polygon and an other random point outside
    // the polygon cuts edges an odd number of times, then p is in the polygon
    size_t i;
    int iseg, intersections_count = 0;
    PRL_FPoint I, A, B;
    I.x = 1000000.0 + (float) (rand()%1000);   // 1 000 000 + a random integer between 0 and 999
    I.y = 1000000.0 + (float) (rand()%1000);

    for(i = 0; i < polygon.size(); i++)
    {
        A = polygon.get(i);
        if (i == polygon.size() - 1)  // if it is the last point, we bind it to the first one
            B = polygon.get(0);
        else           // else we bind it to the next one
            B = polygon.get(i + 1);

        iseg = PRL_Coll_2Segments(A, B, I, p);
        if (iseg == -1)
            return PRL_Coll_PointPolygon(p, polygon);  // limit case, we recursively relaunch the function

        intersections_count += iseg;
    }
    if (intersections_count % 2 == 1)  // Is intersections_count an odd integer?
        return true;
    else
        return false;
}

bool PRL_Coll_RectCircle(PRL_FRect const& rect, PRL_FCircle const& circle)
{
    if (PRL_Coll_PointRect(circle.getCenter(), rect))
        return true;
        //check collisions of segments and circle
    return false;
}

bool PRL_Coll_RectPolygon(PRL_FRect const& rect, PRL_FPointList const& circle)
{
    return false;
}

bool PRL_Coll_CirclePolygon(PRL_FCircle const& rect, PRL_FPointList const& circle)
{
    return false;
}

bool PRL_Coll_2Polygons(PRL_FPointList const& poly1, PRL_FPointList const& poly2)
{
    return false;
}

bool PRL_Coll_2HitBoxes(PRL_HitBox const& hitbox1, PRL_HitBox const& hitbox2)
{
    if (PRL_Coll_2Rects(hitbox1.getRectAround(), hitbox2.getRectAround()))
    {
        if (hitbox1.getType() == PRL_HITBOX_RECT && hitbox2.getType() == PRL_HITBOX_RECT)
            return true;
        else if (hitbox1.getType() == PRL_HITBOX_RECT && hitbox2.getType() == PRL_HITBOX_CIRCLE)
            return PRL_Coll_RectCircle(hitbox1.getRect(), hitbox2.getCircle());
        else if (hitbox1.getType() == PRL_HITBOX_RECT && hitbox2.getType() == PRL_HITBOX_POLYGON)
            return PRL_Coll_RectPolygon(hitbox1.getRect(), hitbox2.getPolygon());
        else if (hitbox1.getType() == PRL_HITBOX_CIRCLE && hitbox2.getType() == PRL_HITBOX_CIRCLE)
            return PRL_Coll_2Circles(hitbox1.getCircle(), hitbox2.getCircle());
        else if (hitbox1.getType() == PRL_HITBOX_CIRCLE && hitbox2.getType() == PRL_HITBOX_POLYGON)
            return PRL_Coll_CirclePolygon(hitbox1.getCircle(), hitbox2.getPolygon());
        else if (hitbox1.getType() == PRL_HITBOX_POLYGON && hitbox2.getType() == PRL_HITBOX_POLYGON)
            return PRL_Coll_2Polygons(hitbox1.getPolygon(), hitbox2.getPolygon());
    }
    return false;
}

bool PRL_Coll_2HitBoxes(const PRL_HitBox* hitbox1, const PRL_HitBox* hitbox2)
{
    if (PRL_Coll_2Rects(hitbox1->getRectAround(), hitbox2->getRectAround()))
    {
        if (hitbox1->getType() == PRL_HITBOX_RECT && hitbox2->getType() == PRL_HITBOX_RECT)
            return true;
        else if (hitbox1->getType() == PRL_HITBOX_RECT && hitbox2->getType() == PRL_HITBOX_CIRCLE)
            return PRL_Coll_RectCircle(hitbox1->getRect(), hitbox2->getCircle());
        else if (hitbox1->getType() == PRL_HITBOX_RECT && hitbox2->getType() == PRL_HITBOX_POLYGON)
            return PRL_Coll_RectPolygon(hitbox1->getRect(), hitbox2->getPolygon());
        else if (hitbox1->getType() == PRL_HITBOX_CIRCLE && hitbox2->getType() == PRL_HITBOX_CIRCLE)
            return PRL_Coll_2Circles(hitbox1->getCircle(), hitbox2->getCircle());
        else if (hitbox1->getType() == PRL_HITBOX_CIRCLE && hitbox2->getType() == PRL_HITBOX_POLYGON)
            return PRL_Coll_CirclePolygon(hitbox1->getCircle(), hitbox2->getPolygon());
        else if (hitbox1->getType() == PRL_HITBOX_POLYGON && hitbox2->getType() == PRL_HITBOX_POLYGON)
            return PRL_Coll_2Polygons(hitbox1->getPolygon(), hitbox2->getPolygon());
    }
    return false;
}

bool PRL_Coll_HitBoxPoint(PRL_HitBox const& hitbox, PRL_FPoint const& point)
{
    return false;
}
*/



bool Coll_2Points(const PRL_Point p1, const PRL_Point p2)
{
    if(p1.x==p2.x&&p1.y==p2.y)
        return true;
    else
        return false;
}

bool Coll_PointCircle(const PRL_Point p, const PRL_Circle C) // may be faster if there were some 'radius2' variable included in the crcl structure
{
    int d2=(p.x-C.x)*(p.x-C.x)+(p.y-C.y)*(p.y-C.y);
    if (d2>C.r*C.r)
        return false;
    else
        return true;
}

bool Coll_CirclePoint(const PRL_Circle C, const PRL_Point p) // may be faster if there were some 'radius2' variable included in the crcl structure
{
    int d2=(p.x-C.x)*(p.x-C.x)+(p.y-C.y)*(p.y-C.y);
    if (d2>C.r*C.r)
        return false;
    else
        return true;
}

bool Coll_PointStraightLine(const PRL_Point A, const PRL_Point O, const PRL_Point P)///A point, (OP) straight line
{
    if ( ((A.x==O.x)&&(A.y==O.y)) || ((A.x==P.x)&&(A.y==P.y)) )
        return true;
    PRL_Vector AO, AP;
    AO.x=O.x-A.x;
    AO.y=O.y-A.y;
    AP.x=P.x-A.x;
    AP.y=P.y-A.y;
    if (1.00*AO.x/AP.x==1.00*AO.y/AP.y)
        return true;
    return false;
}

bool Coll_StraightLinePoint(const PRL_Point B, const PRL_Point C, const PRL_Point A)///A point, (OP) straight line
{
    if ( ((A.x==B.x)&&(A.y==B.y)) || ((A.x==C.x)&&(A.y==C.y)) )
        return true;
    PRL_Vector AB, AC;
    AB.x=B.x-A.x;
    AB.y=B.y-A.y;
    AC.x=C.x-A.x;
    AC.y=C.y-A.y;
    if (1.00*AB.x/AC.x==1.00*AB.y/AC.y) /// ICICICICICI
        return true;
    return false;
}

bool Comm_2StraightLines(const PRL_Point A, const PRL_Point B, const PRL_Point O, const PRL_Point P)
{
    PRL_Vector AB, OP;
    AB.x=B.x-A.x;
    AB.y=B.y-A.y;
    OP.x=P.x-O.x;
    OP.y=P.y-O.y;
    if (1.00*AB.x/OP.x==1.00*AB.y/OP.y)
    {
        if (Coll_PointStraightLine(A, O, P) && Coll_PointStraightLine(B, O, P))
            return true;
        return false;
    }
    return true;
}

bool Coll_PointSegment(const PRL_Point A, const PRL_Point B, const PRL_Point C)///C point, [AB] segment
{
    if (Coll_PointStraightLine(A, B, C)==false)
        return false;
    PRL_Vector AB, AC, BC; // second calculation, loss of time!
    AB.x=B.x-A.x;
    AB.y=B.y-A.y;
    AC.x=C.x-A.x;
    AC.y=C.y-A.y;
    BC.x=C.x-B.x;
    BC.y=C.y-B.y;
    if( (AB.x*AC.x+AB.y*AC.y)*(BC.x*AB.x+BC.y*AB.y)<0)//(AB scalar AC) * (BC scalar AB) <0 ?
        return true;
    return false;
}

bool Coll_PointPolygonConvex(const PRL_Point P, const PRL_Point p_tab[], const int p_nb)
{
    int i, d; // A point is in the polygon if when covered in the trigonometric way it remains on the left
    PRL_Point A, B;
    PRL_Vector D,T;
    for(i=0;i<p_nb;i++)
    {
        A = p_tab[i];
        if (i==p_nb-1)  // if last point, we bind it to the first one
            B = p_tab[0];
        else           // else we bind it to the next one
            B = p_tab[i+1];
        D.x = B.x - A.x;
        D.y = B.y - A.y;
        T.x = P.x - A.x;
        T.y = P.y - A.y;
        d = D.x*T.y - D.y*T.x;
        if (d<0) // if y up-oriented d<0
            return true;  // if a point is "on the right".
    }
    return false;
}

bool Coll_PolygonConvexPoint(const PRL_Point p_tab[], const int p_nb, const PRL_Point P)
{
    int i, d; // A point is in the polygon if when covered in the trigonometric way it remains on the left
    PRL_Point A, B;
    PRL_Vector D,T;
    for(i=0;i<p_nb;i++)
    {
        A = p_tab[i];
        if (i==p_nb-1)  // if last point, we bind it to the first one
            B = p_tab[0];
        else           // else we bind it to the next one
            B = p_tab[i+1];
        D.x = B.x - A.x;
        D.y = B.y - A.y;
        T.x = P.x - A.x;
        T.y = P.y - A.y;
        d = D.x*T.y - D.y*T.x;
        if (d<0) // if y up-oriented d<0
            return true;  // if a point is "on the right".
    }
    return false;
}

bool Coll_StraightLineCircle(const PRL_Point A, const PRL_Point B, const PRL_Circle C)
{
    PRL_Vector u, AC;
    u.x = B.x - A.x;
    u.y = B.y - A.y;
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    double numerator = u.x*AC.y - u.y*AC.x;   // vector magnitude of v
    if (numerator <0)
      numerator = -numerator ;   // absolute value
    double denominator = sqrt(u.x*u.x + u.y*u.y);  // vector magnitude of u
    double CI = numerator / denominator;
    if (CI<C.r)
        return true;
    else
        return false;
}

bool Coll_CircleStraightLine(const PRL_Circle C, const PRL_Point A, const PRL_Point B)
{
    PRL_Vector u, AC;
    u.x = B.x - A.x;
    u.y = B.y - A.y;
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    double numerator = u.x*AC.y - u.y*AC.x;   // vector magnitude of v
    if (numerator <0)
      numerator = -numerator ;   // absolute value
    double denominator = sqrt(u.x*u.x + u.y*u.y);  // vector magnitude of u
    double CI = numerator / denominator;
    if (CI<C.r)
        return true;
    else
        return false;
}

bool Coll_SegmentCircle(const PRL_Point A, const PRL_Point B, const PRL_Circle C)
{
    if (Coll_StraightLineCircle(A,B,C) == false)
        return false;  // if we are not touching the straight line, neither do we for the segment
    PRL_Vector AB,AC,BC;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    BC.x = C.x - B.x;
    BC.y = C.y - B.y;
    double scalp1 = AB.x*AC.x + AB.y*AC.y;  // scalar product
    double scalp2 = (-AB.x)*BC.x + (-AB.y)*BC.y;  // scalar product
    if (scalp1>=0 && scalp2>=0)
        return true;   // I between A and B, okay
        // last possibility, A or B is in the circle
    if (Coll_PointCircle(A,C))
        return true;
    if (Coll_PointCircle(B,C))
        return true;
    return false;
}

bool Coll_CircleSegment(const PRL_Circle C, const PRL_Point A, const PRL_Point B)
{
    if (Coll_StraightLineCircle(A,B,C) == false)
        return false;  // if we are not touching the straight line, neither do we for the segment
    PRL_Vector AB,AC,BC;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    BC.x = C.x - B.x;
    BC.y = C.y - B.y;
    double scalp1 = AB.x*AC.x + AB.y*AC.y;  // scalar product
    double scalp2 = (-AB.x)*BC.x + (-AB.y)*BC.y;  // scalar product
    if (scalp1>=0 && scalp2>=0)
        return true;   // I between A and B, okay
        // last possibility, A or B is in the circle
    if (Coll_PointCircle(A,C))
        return true;
    if (Coll_PointCircle(B,C))
        return true;
    return false;
}

bool Coll_SegmentStraightLine(const PRL_Point A, const PRL_Point B, const PRL_Point O, const PRL_Point P)
{
  PRL_Vector AO,AP,AB;
  AB.x = B.x - A.x;
  AB.y = B.y - A.y;
  AP.x = P.x - A.x;
  AP.y = P.y - A.y;
  AO.x = O.x - A.x;
  AO.y = O.y - A.y;
  if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x)<0)
     return true;
  else
     return false;
}

bool Coll_StraightLineSegment(const PRL_Point O, const PRL_Point P, const PRL_Point A, const PRL_Point B)
{
  PRL_Vector AO,AP,AB;
  AB.x = B.x - A.x;
  AB.y = B.y - A.y;
  AP.x = P.x - A.x;
  AP.y = P.y - A.y;
  AO.x = O.x - A.x;
  AO.y = O.y - A.y;
  if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x)<0)
     return true;
  else
     return false;
}

/*bool Coll_CircleAABB(const PRL_Circle C, const PRL_Rect box)
{
   PRL_Rect boxCircle = GetBoxAroundCircle(C);  // retourner la bounding box de l'image porteuse, ou calculer la bounding box.
   if (PRL_Coll_2AABBs(box, boxCircle)==false)
      return false;   // first test
   if (Coll_PointCircle(GetPointFromCoord(box.x,box.y), C) == true
    || Coll_PointCircle(GetPointFromCoord(box.x,box.y+box.h), C) == true
    || Coll_PointCircle(GetPointFromCoord(box.x+box.w,box.y), C) == true
    || Coll_PointCircle(GetPointFromCoord(box.x+box.w,box.y+box.h), C) == true)
      return true;   // second test
   if (PRL_Coll_PointAABB(GetPointFromCoord(C.x,C.y), box) == 1)
      return true;   // third test
   if (ProjectionOnSegment(GetPointFromCoord(C.x, C.y), GetPointFromCoord(box.x,box.y), GetPointFromCoord(box.x,box.y+box.h))==true
   || ProjectionOnSegment(GetPointFromCoord(C.x, C.y), GetPointFromCoord(box.x,box.y), GetPointFromCoord(box.x+box.w,box.y))==true)
      return true;
   return false;
}




* ********************************************* *
*                PRL_CollInfos                  *
* ********************************************* *


PRL_CollInfos :: PRL_CollInfos() : srcNumber(0), hitBox_target(0), collidable_target(nullptr)
{
    ;
}

PRL_CollInfos :: ~PRL_CollInfos()
{
    ;
}



* ********************************************* *
*                PRL_Collidable                 *
* ********************************************* *

PRL_Collidable :: PRL_Collidable(PRL_FRect *display_dst_rect) :
    collider_address(-1), coll_isDynamic_(true), collidableType(PRL_COLLTYPE_0)
{
    ;
}

PRL_Collidable :: ~PRL_Collidable()
{
    ;
}

void PRL_Collidable :: makeCollStatic()
{
    coll_isDynamic_ = false;
}

void PRL_Collidable :: makeCollDynamic()
{
    coll_isDynamic_ = true;
}

void PRL_Collidable :: makeCollDynamic(bool b)
{
    coll_isDynamic_ = b;
}

bool PRL_Collidable :: isCollDynamic() const
{
    return coll_isDynamic_;
}

void PRL_Collidable :: setCollidableType(PRL_CollType type)
{
    collidableType = type;
}

void PRL_Collidable :: updateAbsolutePointCluster() ///////////////////////////// PROBLEM WITH currentFrame[0] (temporary solution)
{
    absoluteCluster.setAnchorPoint(currentFrame[0],
                                   {dspDst.x + relativeCluster->getFixedPoint(currentFrame[0]).x,
                                   dspDst.y + relativeCluster->getFixedPoint(currentFrame[0]).y});
    int i(0), points_number(relativeCluster->getActionPointsNumber());
    for (; i < points_number; i++)
    {
        absoluteCluster.setActionPoint(currentFrame[0], i,
                                   {dspDst.x + relativeCluster->getActionPoint(currentFrame[0], i).x,
                                   dspDst.y + relativeCluster->getActionPoint(currentFrame[0], i).y});
    }

    int hitBox_number = relativeCluster->getHitBoxesNumber();
    PRL_HitBox hitBox;

    for (i = 0; i < hitBox_number ; i++)
    {
        hitBox = relativeCluster->getHitBox(currentFrame[0], i);
        hitBox.translateBy(dspDst.x, dspDst.y);
        absoluteCluster.setHitBox(currentFrame[0], i, hitBox);
    }
}

void PRL_Collidable :: setCurrentAnimation(const PRL_AnimationSimple* animation)
{
    relativeCluster = animation->getPointCluster();
}

void PRL_Collidable :: setCurrentFramePointer(const int* current_frame)
{
    current_frame = (int*) current_frame;
}

int PRL_Collidable :: getActionPointsNumber() const
{
    return relativeCluster->getActionPointsNumber();
}

int PRL_Collidable :: getHitBoxesNumber() const
{
    return relativeCluster->getHitBoxesNumber();
}


* ********************************************* *
*                PRL_Collider                   *
* ********************************************* *

PRL_Collider :: PRL_Collider()
{
    ;
}

PRL_Collider :: ~PRL_Collider()
{
    ;
}

bool PRL_Collider :: isAdded(PRL_Collidable *collidable) const
{
    for (unsigned int i(0); i<collidables.size(); i++)
    {
        if (collidables[i] == collidable)
            return true;
    }
    return false;
}

bool PRL_Collider :: isAdded(PRL_CollGroup collGroup) const
{
    for (unsigned int i(0); i<collidables.size(); i++)
    {
        if (collisionGroups[i] == collGroup)
            return true;
    }
    return false;
}

void PRL_Collider :: add(PRL_Collidable *collidable)
{
    if (!isAdded(collidable))
    {
        collidables.push_back(collidable);
        collidable->collider_address = collidables.size() - 1;
    }
    else
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Collider, the collidable to be added is already added to the collider" << endl;
        #endif // PRL_USE_WARNINGS
    }

    #if PRL_MULTITHREADING == 1
    threads.resize(collidables.size() * (collidables.size() + 1) / 2 - 1); // Number of operations n*(n+1)/2 - 1
    #endif // PRL_MULTITHREADING
}

void PRL_Collider :: remove(PRL_Collidable* collidable)
{
    if (isAdded(collidable))
    {
        collidables.push_back(collidable);

        for (unsigned int i(collidable->collider_address+1); i<collidables.size(); i++) // update addresses greater than the removed collidable
        {
            collidables[i]->collider_address--;
        }

        collidables.erase(collidables.begin() + collidable->collider_address);
        collidable->collider_address = -1;
    }
    else
    {
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Collider, the collidable to be removed is not added to the collider" << endl;
        #endif // PRL_USE_WARNINGS
    }

    #if PRL_MULTITHREADING == 1
    threads.resize(collidables.size() * (collidables.size() + 1) / 2 - 1); // Number of operations n*(n+1)/2 - 1
    #endif // PRL_MULTITHREADING
}

void PRL_Collider :: testCollisions()
{
    unsigned int i(0), j(0);

    #if PRL_MULTITHREADING == 1
    int k(0);
    #endif // PRL_MULTITHREADING

    for (; i<collidables.size(); i++)
    {
        if (collidables[i]->isCollDynamic())
        {
            for (j = i + 1; j<collidables.size(); j++) // j = i + 1 because i == j is not possible, and the collision test tests all possibilities between object i and j
            {
                #if PRL_MULTITHREADING == 1
                threads[k] = std::thread(&testCollisionsBetween, this, i, j);
                k++;
                #else
                // test collision between objects i and j
                testCollisionsBetween(i, j);
                #endif // PRL_MULTITHREADING
            }
        }
    }

    #if PRL_MULTITHREADING == 1
    for (k = 0; k < threads.size(); k++) // join all the threads
        if (threads[k].joinable()) threads[k].join();
    #endif // PRL_MULTITHREADING
}

void PRL_Collider :: testCollisionsBetween(int i, int j) // without any concern about i == j or the isDynamic boolean
{
    // try to find a proper collision group
    for (unsigned int k(0); i < collisionGroups.size() - 1; i++) // no need to test the very last one
    {
        if ((collidables[i]->collidableType & collisionGroups[k]) && (collidables[j]->collidableType & collisionGroups[k]) )
        {
            // test whether display rects are colliding or not
            if (PRL_Coll_2Rects(collidables[i]->dspDst, collidables[j]->dspDst))
            {
                // update the absolute cluster in preparation for all the collision tests
                collidables[i]->updateAbsolutePointCluster();
                collidables[j]->updateAbsolutePointCluster();

                PRL_CollInfos collInfos;

                int HB1_nb(collidables[i]->getHitBoxesNumber()), HB2_nb(collidables[j]->getHitBoxesNumber());
                int AP1_nb(collidables[i]->getActionPointsNumber()), AP2_nb(collidables[j]->getActionPointsNumber()); // HB for hit box, AP for action point, 1 or 2 in reference to the object 1 or 2
                int i_HB1(0), j_HB2(0);

                if (AP1_nb < HB1_nb) // split the problem into 2 cases
                {
                    if (AP2_nb < HB2_nb) // split again into 2 cases
                    {
                        for (i_HB1 = 0; i_HB1 < AP1_nb; i_HB1++)
                        {
                            for (j_HB2 = 0; j_HB2 < AP2_nb; j_HB2++)
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }

                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < HB2_nb; j_HB2++) // no AP2 left since AP2_nb < HB2_nb
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }
                            }
                        }

                        for (; i_HB1 < HB1_nb; i_HB1++) // no AP1 left since AP1_nb < HB1_nb
                        {
                            for (j_HB2 = 0; j_HB2 < AP2_nb; j_HB2++)
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < HB2_nb; j_HB2++) // no AP2 left since AP2_nb < HB2_nb
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                        }
                    }
                    else // here: if AP2_nb >= HB2_nb : invert the bounds on j_HB2
                    {
                        for (i_HB1 = 0; i_HB1 < AP1_nb; i_HB1++)
                        {
                            for (j_HB2 = 0; j_HB2 < HB2_nb; j_HB2++)
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }

                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < AP2_nb; j_HB2++) // no HB2 left since HB2_nb < AP2_nb
                            {
                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                        }

                        for (; i_HB1 < HB1_nb; i_HB1++) // no AP1 left since AP1_nb < HB1_nb
                        {
                            for (j_HB2 = 0; j_HB2 < HB2_nb; j_HB2++)
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < AP2_nb; j_HB2++) // no HB2 left since HB2_nb < AP2_nb
                            {
                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                        }
                    }
                }
                else // here: if AP1_nb >= HB1_nb : invert the bounds on i_HB1
                {
                    if (AP2_nb < HB2_nb) // split again into 2 cases
                    {
                        for (i_HB1 = 0; i_HB1 < HB1_nb; i_HB1++) // here: AP1_nb >= HB1_nb
                        {
                            for (j_HB2 = 0; j_HB2 < AP2_nb; j_HB2++)
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }

                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < HB2_nb; j_HB2++) // no AP2 left since AP2_nb < HB2_nb
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }
                            }
                        }

                        for (; i_HB1 < AP1_nb; i_HB1++) // no HB1 left since  HB1_nb < AP1_nb
                        {
                            for (j_HB2 = 0; j_HB2 < AP2_nb; j_HB2++)
                            {
                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < HB2_nb; j_HB2++) // no AP2 left since AP2_nb < HB2_nb
                            {
                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }
                            }
                        }
                    }
                    else // here: if AP2_nb >= HB2_nb : invert the bounds on j_HB2
                    {
                        for (i_HB1 = 0; i_HB1 < HB1_nb; i_HB1++)
                        {
                            for (j_HB2 = 0; j_HB2 < HB2_nb; j_HB2++)
                            {
                                // Collision test between HB1 and HB2
                                if (PRL_Coll_2HitBoxes(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->hitBoxCollisions[i_HB1].push_back(collInfos);

                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->hitBoxCollisions[j_HB2].push_back(collInfos);
                                }

                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }

                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < AP2_nb; j_HB2++) // no HB2 left since HB2_nb < AP2_nb
                            {
                                // Collision test between HB1 and AP2
                                if (PRL_Coll_HitBoxPoint(collidables[i]->absoluteCluster.getHitBox(collidables[i]->getCurrentFrame(), i_HB1),
                                                       collidables[j]->absoluteCluster.getActionPoint(collidables[j]->getCurrentFrame(), j_HB2)))
                                {
                                    collInfos.collidable_target = collidables[i];
                                    collInfos.hitBox_target = i_HB1;
                                    collInfos.srcNumber = j_HB2;
                                    collidables[j]->pointCollisions[j_HB2].push_back(collInfos);
                                }
                            }
                        }

                        for (; i_HB1 < AP1_nb; i_HB1++) // no HB1 left since HB1_nb < AP1_nb
                        {
                            for (j_HB2 = 0; j_HB2 < HB2_nb; j_HB2++)
                            {
                                // Collision test between AP1 and HB2
                                if (PRL_Coll_HitBoxPoint(collidables[j]->absoluteCluster.getHitBox(collidables[j]->getCurrentFrame(), j_HB2),
                                                       collidables[i]->absoluteCluster.getActionPoint(collidables[i]->getCurrentFrame(), i_HB1)))
                                {
                                    collInfos.collidable_target = collidables[j];
                                    collInfos.hitBox_target = j_HB2;
                                    collInfos.srcNumber = i_HB1;
                                    collidables[i]->pointCollisions[i_HB1].push_back(collInfos);
                                }
                            }
                            for (; j_HB2 < AP2_nb; j_HB2++) // no HB2 left since HB2_nb < AP2_nb
                            {
                                ;
                            }
                        }
                    }
                }
            }
            break; // quit the for loop
        }
    }
}


* ********************************************* *
*                PRL_Movable                    *
* ********************************************* *

float PRL_Movable :: time_coeff = 0.0;
int PRL_Movable :: movable_count = 0;

PRL_Movable :: PRL_Movable(PRL_FRect* display_dst_rect) : previous_time(0), current_time(0)
{
	PRL_FPoint p(0.0, 0.0);
	pxlsPerSec_ = p;
	previousDisplacement_ = p;
    display_dst_copy = display_dst_rect;
    movable_count++;
}

PRL_Movable :: ~PRL_Movable()
{
    movable_count--;
}

void PRL_Movable :: setVelocityVector(PRL_FPoint const& pxlsPerSec)
{
    pxlsPerSec_ = pxlsPerSec;
}

PRL_FPoint PRL_Movable :: getVelocityVector() const
{
    return pxlsPerSec_;
}

void PRL_Movable :: applyDisplacement()
{
    current_time = handler.time.getTimeUpdated();
    time_coeff = 1.0 * (current_time - previous_time) / 1000000;
    previousDisplacement_ = {pxlsPerSec_.x * time_coeff, pxlsPerSec_.y * time_coeff};

    display_dst_copy->x += previousDisplacement_.x;
    display_dst_copy->y += previousDisplacement_.y;

    previous_time = current_time;
}

int PRL_Movable :: getMvbCount()
{
    return movable_count;
}
*/


