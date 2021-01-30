#pragma once

#include <SFML/Graphics/Texture.hpp>

const sf::Vector2f mapSize(10, 10);

struct Map
{
	Map();
	void draw(sf::Vector2f cameraPos);

	sf::Texture image;
};

