#pragma once

#include <SFML/System/Vector2.hpp>
#include "Map.h"

extern float g_screenHeightInWorldCoordinates;
extern sf::Vector2f g_cameraPos;

struct Camera
{
	static void setCameraPos(sf::Vector2f p);
	static sf::Vector2f getCameraPos() { return g_cameraPos; }

	static sf::Vector2f worldToScreenPos(sf::Vector2f worldPos);
	static sf::Vector2f worldToScreenSize(sf::Vector2f worldSize);

	static sf::Vector2f screenToWorldPos(sf::Vector2f screenPos);
	static sf::Vector2f screenToWorldSize(sf::Vector2f screenSize);
    
    static sf::Vector2f worldToScreenPos(float x, float y);
};
