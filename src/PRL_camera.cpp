#include "PRL_camera.h"
#include "PRL_defines.h"
#include <iostream>
#include <math.h>

using std::cout;
using std::cout;
using std::endl;

PRL_Camera :: PRL_Camera() : zoom(1.0), cameraNumber(0)
{
	PRL_FRect r(0.0, 0.0, 0, 0);
    cameraRectangle = r;

    PRL_FPoint p(0.0, 0.0);
    absoluteCenter = p;
}

PRL_Camera :: ~PRL_Camera()
{
    ;
}

void PRL_Camera :: setZoom(double newZoom)// NB: doesn't change the center position
{
    if (newZoom > 0.0)
    {
        cameraRectangle.w = floor(cameraRectangle.w * zoom / newZoom);
        cameraRectangle.h = floor(cameraRectangle.h * zoom / newZoom);
        zoom = newZoom;
    }
    else
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Camera, tried to set a negative or null zoom" << endl;
        #endif // PRL_USE_WARNINGS
}

void PRL_Camera :: setRectangle(float x, float y, int w, int h)
{
    zoom = 1.0;
    cameraRectangle = {x, y, w, h};

    absoluteCenter.x = cameraRectangle.x + cameraRectangle.w/2.0;
    absoluteCenter.y = cameraRectangle.y + cameraRectangle.h/2.0;
}

void PRL_Camera :: setAbsoluteCenter(float x, float y)
{
    absoluteCenter.x = x;
    absoluteCenter.y = y;

    cameraRectangle.x = x - cameraRectangle.w/2.0;
    cameraRectangle.y = y - cameraRectangle.h/2.0;
}

void PRL_Camera  :: setCameraNumber(int nb)
{
    cameraNumber = nb;
}

int PRL_Camera :: getCameraNumber()
{
    return cameraNumber;
}

double PRL_Camera :: getZoom()
{
    return zoom;
}

PRL_FRect PRL_Camera :: getRectangle()
{
    return cameraRectangle;
}

PRL_FPoint PRL_Camera :: getCenter()
{
    return absoluteCenter;
}

PRL_FPoint PRL_Camera :: getUpLeftCorner()
{
    return {cameraRectangle.x, cameraRectangle.y};
}

