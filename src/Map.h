#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <memory>

const sf::Vector2f mapSize(100, 100);

struct Map
{
	Map();
	void draw();
	sf::Vector2f nearestCollision(sf::Vector2f pos);
	sf::Color getColor(sf::Vector2f pos);

	sf::Image image;
	sf::Texture texture;
	std::unique_ptr<sf::Shader> mapVisShader;
};

