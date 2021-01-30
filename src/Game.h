#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "Map.h"

extern sf::Vector2f g_resolution;

class Game
{
public:
	Game();
	~Game() = default;

	void update(const sf::Time elapsedTime);
	void draw(sf::RenderWindow& window);
	void gui(sf::RenderWindow& window);

	void setMovement(sf::Vector2i direction);

	static sf::String getName()
	{
		return "FGJ 2021";
	}

	static sf::Vector2f getResolution()
	{
		return g_resolution;
	}

	static void setResolution(float x, float y)
	{
		g_resolution.x = x;
		g_resolution.y = y;
	}
    
    bool showDebugText = false;
private:

	sf::Text m_guiText;
	Map map;
    float cameraLerpPerSecond = 5;
    std::string debugText;
};
