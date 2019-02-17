#ifndef PRL_CLASS_CAMERA_H_INCLUDED
#define PRL_CLASS_CAMERA_H_INCLUDED

#include "SDL2/SDL.h"
#include "PRL_types.h"

class PRL_Camera
{
private:
    PRL_FRect cameraRectangle; // how the camera really is (absolute)
    PRL_FPoint absoluteCenter; // absolute center of the camera
    double zoom;
    int cameraNumber;

public:
    PRL_Camera();
    ~PRL_Camera();

    /// Set the camera dimension and reset the zoom to 1.0
    void setRectangle(float x, float y, int w, int h);
    /// Set the zoom and adapt the dimensions (rectangle) in consequence
    void setZoom(double newZoom);
    /// Set the absolute center of the camera rectangle and doesn't change the zoom
    void setAbsoluteCenter(float x, float y);
    /// Set a number for this camera (simple meta data)
    void setCameraNumber(int nb);
    /// Get the camera number
    int getCameraNumber();
    /// Get the current zoom
    double getZoom();
    /// Get the whole camera rectangle
    PRL_FRect getRectangle();
    /// Get the camera absolute center position
    PRL_FPoint getCenter();
    /// Get the coordinates of the upper left corner
    PRL_FPoint getUpLeftCorner();
};

#endif // PRL_CLASS_CAMERA_H_INCLUDED
