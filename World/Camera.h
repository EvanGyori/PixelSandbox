#pragma once

#include <cmath>

class Camera
{
public:
    Camera();
    ~Camera();

    // returns cellToPixels
    int getScale();
    float getX();
    float getY();

    void zoom(int increment);

    // sw, sh - screen width and height in pixels
    // edits posX and posY to change from world to pixel coordinates
    void convertToScreen(int& posX, int& posY, int sw, int sh);

    // sw, sh - screen width and height in pixels
    // edits posX and posY to change from pixel to world coordinates
    void convertToWorld(int& posX, int& posY, int sw, int sh);

    void move(float dx, float dy);

private:
    float x, y;

    // Zoom
    int cellToPixels;

    // Settings
    float panSpeed;
    int zoomSpeed;
};