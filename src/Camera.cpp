#include "Camera.h"

Camera::Camera()
{
    x = 0.0f;
    y = 0.0f;
    cellToPixels = 5;

    panSpeed = 5.0f;
    zoomSpeed = 1;
}

Camera::~Camera()
{
}

int Camera::getScale()
{
    return cellToPixels;
}

void Camera::zoom(int increment)
{
    cellToPixels += increment * zoomSpeed;
    if (cellToPixels < 1) { cellToPixels = 1; }
}

void Camera::convertToScreen(int& posX, int& posY, int sw, int sh)
{
    posX = cellToPixels * (posX - x) + sw / 2;
    posY = cellToPixels * (posY - y) + sh / 2;
}

void Camera::move(float dx, float dy)
{
    x += dx * panSpeed;
    y += dy * panSpeed;
}