#include "Camera.h"

sf::Vector2f g_cameraPos;
sf::Vector2f Camera::worldToScreenPos(sf::Vector2f worldPos)
{
	return (worldPos - g_cameraPos) / g_screenHeightInWorldCoordinates;
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
