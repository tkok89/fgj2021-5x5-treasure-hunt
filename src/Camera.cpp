#include "Camera.h"

#include <assert.h>

float g_screenHeightInWorldCoordinates = 10;
sf::Vector2f g_cameraPos;
void Camera::setCameraPos(sf::Vector2f p)
{
    assert(p == p);
    g_cameraPos = p;
}

sf::Vector2f Camera::worldToScreenPos(sf::Vector2f worldPos)
{
    return (worldPos - g_cameraPos) / g_screenHeightInWorldCoordinates;
}

sf::Vector2f Camera::worldToScreenPos(float x, float y)
{
    return (sf::Vector2f(x,y) - g_cameraPos) / g_screenHeightInWorldCoordinates;
}

sf::Vector2f Camera::worldToScreenSize(sf::Vector2f worldSize)
{
    return worldSize / g_screenHeightInWorldCoordinates;
}

sf::Vector2f Camera::screenToWorldPos(sf::Vector2f screenPos)
{
	return screenPos * g_screenHeightInWorldCoordinates + g_cameraPos;
}

sf::Vector2f Camera::screenToWorldSize(sf::Vector2f screenSize)
{
	return screenSize * g_screenHeightInWorldCoordinates;
}
