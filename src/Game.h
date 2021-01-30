#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
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

	static sf::Vector2f resolution;
	static sf::Vector2f getResolution()
	{
		return resolution;
	}

	static void setResolution(float x, float y)
	{
		resolution.x = x;
		resolution.y = y;
	}

private:

	sf::Text m_guiText;
};
