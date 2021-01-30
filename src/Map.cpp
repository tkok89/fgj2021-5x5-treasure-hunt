#include "Map.h"

#include "Resources.h"
#include "GuiRendering.h"
#include <SFML/Window/Keyboard.hpp>

Map *g_map = nullptr;

Map::Map()
{
	image.loadFromFile(Resources::getResources().mapName);
	g_map = this;
}

void Map::draw(sf::Vector2f cameraPos)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		static sf::Vector2f hackOffset(0, 0);

		sf::Time elapsedTime = sf::seconds(1.0f / 60.0f);
		float cameraSpeed = 10.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			hackOffset.x -= elapsedTime.asSeconds() * cameraSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			hackOffset.x += elapsedTime.asSeconds() * cameraSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			hackOffset.y -= elapsedTime.asSeconds() * cameraSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			hackOffset.y += elapsedTime.asSeconds() * cameraSpeed;

		cameraPos += hackOffset;
	}

	float r = g_resolution.x / g_resolution.y;
	GuiRendering::pushClipRect(-0.5f * r, -0.5f, r, 1.0f);
	sf::Vector2f topLeft = (-cameraPos - mapSize * 0.5f);
	GuiRendering::image(&image, topLeft.x, topLeft.y, mapSize.x, mapSize.y);
	GuiRendering::popClipRect();
}

sf::Vector2f Map::nearestCollision(sf::Vector2f pos)
{
	sf::Vector2f p = (pos + mapSize * 0.5f);
	p.x = floorf(p.x / image.getSize().x);
	p.y = floorf(p.y / image.getSize().y);
	return p; // WIP
}

