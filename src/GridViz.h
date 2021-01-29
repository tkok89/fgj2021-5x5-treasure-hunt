#pragma once

#include <SFML/System/Vector2.hpp>

struct Grid;

struct GridViz
{
	static void render(Grid &grid, sf::Vector2f pos, sf::Vector2f size, sf::Vector2u moveThisRow, float moveAmount);
};
