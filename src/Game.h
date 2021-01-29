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
		void start(sf::Vector2u moveThisRow, float tileLength);
		float updateSlide();
		float tileLength = 0.0f;
		sf::Vector2u moveThisRow;
		sf::Clock tileClock;
		sf::Time duration = sf::seconds(1u);
		bool active = false;
	};
	sf::Text m_guiText;
	SlidingTiles slidingTiles;
};
