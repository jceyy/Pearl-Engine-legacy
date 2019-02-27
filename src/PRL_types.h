#ifndef PRL_TYPES_H_INCLUDED
#define PRL_TYPES_H_INCLUDED

#include <SDL2/SDL.h>

#define PRL_Rect SDL_Rect

typedef struct PRL_FRect PRL_FRect;
struct PRL_FRect
{
    float x, y;
    int w, h;
};

typedef struct PRL_Point PRL_Point;
struct PRL_Point
{
    int x, y; // point
};

typedef struct PRL_FPoint PRL_FPoint;
struct PRL_FPoint
{
    float x, y;
};

typedef struct PRL_Circle PRL_Circle;
struct PRL_Circle
{
    int x, y; // center of the circle
    int radius;// pixels
};

typedef struct PRL_FCircle PRL_FCircle;
struct PRL_FCircle
{
    float x, y; // center of the circle
    float radius;// pixels

    PRL_FPoint getCenter() const
    {
        PRL_FPoint p({x, y});
        return p;
    }
};

typedef struct PRL_Vector PRL_Vector;
struct PRL_Vector
{
    double x, y;
};

#endif // PRL_TYPES_H_INCLUDED
