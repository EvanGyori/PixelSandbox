#pragma once

class Camera
{
public:
    Camera();
    ~Camera();

    // returns cellToPixels
    int getScale();

    void zoom(int increment);

    // sw, sh - screen width and height in pixels
    // edits posX and posY to correlate into pixel coordinates
    void convertToScreen(int& posX, int& posY, int sw, int sh);

    void move(float dx, float dy);

private:
    float x, y;

    // Zoom
    int cellToPixels;

    // Settings
    float panSpeed;
    int zoomSpeed;
};