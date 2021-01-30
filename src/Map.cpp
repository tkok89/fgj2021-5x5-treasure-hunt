#include "Map.h"

#include "Camera.h"
#include <cmath>
#include "Resources.h"
#include "GuiRendering.h"
#include "GuiRenderInfo.h"
#include <SFML/Window/Keyboard.hpp>

Map *g_map = nullptr;

Map::Map()
{
	image.loadFromFile(Resources::getResources().mapName);
	texture.loadFromImage(image);

	mapVisShader = Resources::getResources().getShader(ShaderResourceName::mapVis);

	g_map = this;
}

sf::Vector2i worldToMapPos(sf::Vector2f worldPos)
{
	float x = (worldPos.x / mapSize.x + 0.5f) * g_map->image.getSize().x;
	float y = (worldPos.y / mapSize.y + 0.5f) * g_map->image.getSize().y;
	return sf::Vector2i(int(x), int(y));
}

sf::Vector2f mapToWorldPos(sf::Vector2i mapPos)
{
	float x = (mapPos.x / (float)g_map->image.getSize().x - 0.5f) * mapSize.x;
	float y = (mapPos.y / (float)g_map->image.getSize().y - 0.5f) * mapSize.y;
	return sf::Vector2f(x, y);
}

static void drawColor(sf::Vector2f p, sf::Color c)
{
	GuiRenderInfo guiRenderInfo;
	guiRenderInfo.type = GuiRenderInfoType::Image;
	guiRenderInfo.image = g_placeholder;
	guiRenderInfo.x = p.x - 0.01f;
	guiRenderInfo.y = p.y - 0.01f;
	guiRenderInfo.w = 0.02f;
	guiRenderInfo.h = 0.02f;
	guiRenderInfo.uvX = 0;
	guiRenderInfo.uvY = 0;
	guiRenderInfo.uvW = 1;
	guiRenderInfo.uvH = 1;
	guiRenderInfo.color = { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f };
	GuiRendering::render(guiRenderInfo);
}

void Map::draw()
{
	sf::Vector2f cameraPos = Camera::getCameraPos();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		sf::Vector2f hackOffset(0, 0);

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

		Camera::setCameraPos(cameraPos + hackOffset);
	}

	float r = g_resolution.x / g_resolution.y;
	GuiRendering::pushClipRect(-0.5f * r, -0.5f, r, 1.0f);

	sf::Vector2f topLeft = Camera::worldToScreenPos(-mapSize * 0.5f);
	sf::Vector2f screenMapSize = Camera::worldToScreenSize(mapSize);
	
	mapVisShader->setUniform("mapTex", texture);
	GuiRendering::imageShaded(&texture, topLeft.x, topLeft.y, screenMapSize.x, screenMapSize.y, mapVisShader.get());
	
	GuiRendering::popClipRect();

	const sf::Vector2f mousePos = getMousePos();
	sf::Vector2f worldMouse = Camera::screenToWorldPos(mousePos);
	drawColor(mousePos, Map::getColor(worldMouse));
	sf::Vector2f collision = Map::nearestCollectible(worldMouse);
	sf::Vector2f collisionOnScreen = Camera::worldToScreenPos(collision);
	if (collision.x > -200 && collision.y > -200)
	{
		drawColor(collisionOnScreen, sf::Color::Red);
		GuiRendering::line(mousePos.x, mousePos.y, collisionOnScreen.x, collisionOnScreen.y);

		sf::Vector2f d = collisionOnScreen - mousePos;
		float r = sqrtf(d.x * d.x + d.y * d.y);
		GuiRendering::circle(mousePos.x, mousePos.y, r);
	}
}

static sf::Vector2f nearestColorImpl(sf::Vector2f pos, sf::Color item, sf::Image &image, bool acceptBorders)
{
	sf::Vector2i center = worldToMapPos(pos);
	int nearestDist = 1000000;
	sf::Vector2i nearest(-10000, -10000);
	int size = 101;
	for (int i = 0; i < size * size; ++i)
	{
		int ix = (i % size) - size / 2;
		int iy = (i / size) - size / 2;
		int dist = ix * ix + iy * iy;
		if (dist >= nearestDist)
			continue;

		int x = center.x + ix;
		int y = center.y + iy;
		if (x < 0 || x >= (int)image.getSize().x || y < 0 || y >= (int)image.getSize().y)
		{
			if (acceptBorders)
			{
				nearestDist = dist;
				nearest.x = x;
				nearest.y = y;
			}
			continue;
		}

		sf::Color c = image.getPixel(x, y);
		if (c == item)
		{
			nearestDist = dist;
			nearest.x = x;
			nearest.y = y;
			continue;
		}
	}

	return mapToWorldPos(nearest);
}

sf::Vector2f Map::nearestCollision(sf::Vector2f pos)
{
	sf::Color wall{ 0xb9, 0x7a, 0x57, 0xFF };
	return nearestColorImpl(pos, wall, image, true);
}

sf::Vector2f Map::nearestCollectible(sf::Vector2f pos)
{
	sf::Color collectible{ 0xFF, 0x00, 0x00, 0xFF };
	return nearestColorImpl(pos, collectible, image, false);
}

sf::Vector2f Map::nearestShop(sf::Vector2f pos)
{
	sf::Color shop{ 0x00, 0xFF, 0x00, 0xFF };
	return nearestColorImpl(pos, shop, image, false);
}

sf::Color Map::getColor(sf::Vector2f pos)
{
	sf::Vector2i mapPos = worldToMapPos(pos);
	if (mapPos.x < 0 || mapPos.x >= (float)image.getSize().x || mapPos.y < 0 || mapPos.y > (float)image.getSize().y)
		return sf::Color(0, 0, 0, 0);

	sf::Color color = image.getPixel(mapPos.x, mapPos.y);
	return color;
}
