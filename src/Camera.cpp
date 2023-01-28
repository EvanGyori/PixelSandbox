#include "Camera.h"

#include <cmath>

Camera::Camera()
{
    x = 0.0f;
    y = 0.0f;
    cellToPixels = 5;

    panSpeed = 2000.0f;
    zoomSpeed = 1;
}

Camera::~Camera()
{
}

int Camera::getScale()
{
    return cellToPixels;
}

float Camera::getX()
{
    return x;
}

float Camera::getY()
{
    return y;
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

void Camera::convertToWorld(int& posX, int& posY, int sw, int sh)
{
    posX = (posX - sw/2) / cellToPixels + x;
    posY = (posY - sh/2) / cellToPixels + y;
}

void Camera::move(float dx, float dy)
{
    x += dx * panSpeed * pow(0.9f, cellToPixels);
    y += dy * panSpeed * pow(0.9f, cellToPixels);
}



