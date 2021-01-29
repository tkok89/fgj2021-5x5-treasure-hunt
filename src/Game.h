#pragma once

#include "Grid.h"
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game() = default;

	void update(const sf::Time elapsedTime);
	void draw(sf::RenderWindow& window);
	void gui(sf::RenderWindow& window);

	void onJumpAction();
	void setMovement(sf::Vector2i direction);

	static sf::String getName()
	{
		return "FGJ 2021";
	}

private:

	Grid grid;

	sf::Text m_guiText;
};
