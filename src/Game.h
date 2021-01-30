#pragma once

#include "Grid.h"
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

	static sf::Vector2f getResolution()
	{
		return { 1280, 720 };
	}

private:

	Grid grid;
	struct SlidingTiles
	{
		void start(sf::Vector2i moveThisRowToDirection, float tileLength);
		void updateSlide();
		sf::Vector2i getMoveRow()
		{
			return moveThisRowToDirection.x < 0.0f || moveThisRowToDirection.y < 0.0f 
				? sf::Vector2i(moveThisRowToDirection.x * -1, moveThisRowToDirection.y * -1) : moveThisRowToDirection;
		}
		float tileLength = 0.0f;
		float currentPos = 0.0f;
		sf::Vector2i moveThisRowToDirection;
		sf::Clock tileClock;
		sf::Time duration = sf::seconds(2u);
		bool active = false;
	};
	sf::Text m_guiText;
	SlidingTiles slidingTiles;
};
