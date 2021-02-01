#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include "Map.h"

extern sf::Vector2f g_resolution;
extern std::string g_currentTextInput;
extern sf::Clock timeFromStart;
class Game
{
public:
	Game();
	~Game() = default;

	void update(const sf::Time elapsedTime);
	void draw(sf::RenderWindow& window);
	void gui(sf::RenderWindow& window);

	static void textInput(sf::Uint32 unicode);
	


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
    
    static bool showDebugText;
private:

	sf::Text m_guiText;
	Map map;
    float cameraLerpPerSecond = 5;
    std::string debugText;
    float cameraMinSize = 10;
    float lastMove = 0;
    float maxSize = 40;
    float zoomDuration = 30;
};
