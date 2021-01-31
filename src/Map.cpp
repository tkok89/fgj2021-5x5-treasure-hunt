#include "Map.h"

#include "Camera.h"
#include <cmath>
#include "Resources.h"
#include "GuiRendering.h"
#include "GuiRenderInfo.h"
#include <SFML/Window/Keyboard.hpp>
#include <random>
#include <assert.h>
#include <iostream>
#include "Game.h"


Map *g_map = nullptr;

namespace
{

static const sf::Color wallColor { 0xb9, 0x7a, 0x57, 0xFF };

static int max(int a, int b)
{
	return a > b ? a : b;
}

static int min(int a, int b)
{
	return a < b ? a : b;
}

static sf::Vector2i nearestEdgePoint(sf::Vector2i pos, sf::Image &image, int kernelSize = 101)
{
	if (image.getPixel(pos.x, pos.y) == wallColor)
		return sf::Vector2i(int(pos.x), int(pos.y));

	int nearestDist = 1000000;
	sf::Vector2i nearest(-1, -1);

	int xStart = max(0, pos.x - kernelSize / 2);
	int xEnd = min(image.getSize().x - 1, pos.x + kernelSize / 2);
	int yStart = max(0, pos.y - kernelSize / 2);
	int yEnd = min(image.getSize().y - 1, pos.y + kernelSize / 2);
	
	for (int y = yStart; y < yEnd; ++y)
	for (int x = xStart; x < xEnd; ++x)
	{
		sf::Vector2i diff = sf::Vector2i(x - pos.x, y - pos.y);
		int dist = diff.x * diff.x + diff.y * diff.y;
		if (dist > nearestDist)
			continue;

		sf::Color c = image.getPixel(x, y);
		if (c != wallColor)
			continue;

		nearestDist = dist;
		nearest.x = x;
		nearest.y = y;
		continue;
	}

	return nearest;
}

sf::Image generateSDF(sf::Image image)
{
	sf::Image sdf;
	if (sdf.loadFromFile(Resources::getResourcePath("assets/sdf.png")))
	{
		std::cout << "Sdf loaded from cache." << std::endl;
		return sdf;
	}

	std::cout << "Generating sdf." << std::endl;
	sdf = image;

	uint32_t kernelSize = 128;
	float maxLength = ((kernelSize + 3) / 2) * sqrtf(2.f);

	for (uint32_t y = 0; y < image.getSize().y; ++y)
	{
		std::cout << "Map SDF generation progress: " << y << "/" << image.getSize().y
			<< " (" << ((float)y/(float)image.getSize().y) * 100.f << " %)" << std::endl;

		for (uint32_t x = 0; x < image.getSize().x; ++x)
		{
			sf::Vector2i pos((int)x, (int)y);
			sf::Vector2i nearest = nearestEdgePoint(pos, image, kernelSize);
			float lengthNorm = 1.0f;
			if (nearest.x == pos.x && nearest.y == pos.y)
			{
				lengthNorm = 0;
			}
			else if (nearest.x == -1)
			{
				lengthNorm = 1;
			}
			else
			{
				sf::Vector2i diff;
				diff.x = int(nearest.x) - int(pos.x);
				diff.y = int(nearest.y) - int(pos.y);
				float length = sqrtf(float(diff.x * diff.x + diff.y * diff.y));
				lengthNorm = length / maxLength;
				assert(lengthNorm > 0 && lengthNorm <= 1.0f);
			}

			sdf.setPixel(
				x,
				y,
				sf::Color(
					(uint8_t)(lengthNorm * 255),
					(uint8_t)(lengthNorm * 255),
					(uint8_t)(lengthNorm * 255),
					255
				)
			);
		}
	}

	bool success = sdf.saveToFile(Resources::getResourcePath("assets/sdf.png"));
	if (!success)
		std::cerr << "generate SDF done. Failed to save. :(" << std::endl;
	else
		std::cout << "generate SDF done. Cached." << std::endl;

	return sdf;
}

}

Map::Map()
{
	bool success = image.loadFromFile(Resources::getResources().mapName);
	assert(success);

	mapSDFImage = generateSDF(image);

	success = mapSDFTexture.loadFromImage(mapSDFImage);
	assert(success);

	success = texture.loadFromImage(image);
	assert(success);

	mapVisShader = Resources::getResources().getShader(ShaderResourceName::mapVis);

	g_map = this;

	randomize();
}

void Map::randomize()
{
	const sf::Vector2f topLeft = mapSize / -2.0f;
	const sf::Vector2f tileSize = mapSize / 5.0f;
	static std::default_random_engine rng;

	treasures.clear();

	for (int i = 0; i < 25; ++i)
	{
		std::uniform_int_distribution<int> itemDistribution((int)Item::JewelS, (int)Item::Dynamite);
		std::uniform_real_distribution<float> distribution(0.05f, 0.95f);
		int x = i % 5;
		int y = i / 5;
		Item item = Item(itemDistribution(rng));

		sf::Vector2f p;
		p.x = topLeft.x + tileSize.x * (x + distribution(rng));
		p.y = topLeft.y + tileSize.y * (y + distribution(rng));
		bool ok = false;
		for (int retry = 0; retry < 100; ++retry)
		{
			sf::Vector2f dist = nearestCollision(p) - p;
			if (dist.x * dist.x + dist.y * dist.y > 0.5f)
			{
				ok = true;
				break;
			}

			p.x = topLeft.x + tileSize.x * (x + distribution(rng));
			p.y = topLeft.y + tileSize.y * (y + distribution(rng));
		}

		if (ok)
			treasures.push_back({ item, p });
	}

	std::uniform_int_distribution<int> itemDistribution(0, (int)treasures.size() - 1);
	int afrikanTahdenPaikka = itemDistribution(rng);
	treasures[afrikanTahdenPaikka].item = Item::AfrikanTahti;

	shops.clear();

	{
		std::uniform_real_distribution<float> distribution(0.4f, 0.6f);

		sf::Vector2f p;
		p.x = topLeft.x + mapSize.x * distribution(rng);
		p.y = topLeft.y + mapSize.y * distribution(rng);
		bool ok = false;
		for (int retry = 0; retry < 200; ++retry)
		{
			sf::Vector2f hit = nearestCollision(p);
			sf::Vector2f dist = hit - p;
			if (dist.x * dist.x + dist.y * dist.y > 1.0f)
			{
				ok = true;
				break;
			}

			p.x = topLeft.x + mapSize.x * distribution(rng);
			p.y = topLeft.y + mapSize.y * distribution(rng);
		}

		assert(ok);
		shops.push_back(p);
	}
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

static void renderItems()
{
    // Draw map
	{
		sf::Vector2f s = Camera::worldToScreenSize(itemSize) * 4.0f;
		sf::Vector2f p = Camera::worldToScreenPos(Map::getShopPos()) - s * 0.5f;
		GuiRendering::image(&Resources::getShopTexture(), p - sf::Vector2f(0, s.y * 0.33f), s.x, s.y);
	}

	sf::Vector2f s = Camera::worldToScreenSize(itemSize);
	for (const Treasure &treasure : g_map->treasures)
	{
        
        auto pos = treasure.pos - itemSize * 0.5f;
		sf::Vector2f p = Camera::worldToScreenPos(pos);
		GuiRendering::image(&Resources::getItemTexture(treasure.item), p, s.x, s.y);
        
        // Health
        float t = treasure.health / maxHealth;
        
        if(t > 0.99f) continue;
        float h = 0.23f;
        pos.y -= itemSize.y * 0.6f - h;
        sf::Vector2f hPos = Camera::worldToScreenPos(pos);
        GuiRendering::image(&Resources::getResources().healthBack, hPos, Camera::worldToScreenSize(sf::Vector2f(itemSize.x, h)));
        GuiRendering::image(&Resources::getResources().healthFront, hPos, Camera::worldToScreenSize(sf::Vector2f(itemSize.x*t, h)));
	}
}

void Map::draw()
{
	float r = g_resolution.x / g_resolution.y;
	GuiRendering::pushClipRect(-0.5f * r, -0.5f, r, 1.0f);

	sf::Vector2f topLeft = Camera::worldToScreenPos(-mapSize * 0.5f);
	sf::Vector2f screenMapSize = Camera::worldToScreenSize(mapSize);
	
	mapVisShader->setUniform("mapTex", texture);
	mapVisShader->setUniform("mapSDFTex", mapSDFTexture);
	GuiRendering::imageShaded(&texture, topLeft.x, topLeft.y, screenMapSize.x, screenMapSize.y, mapVisShader.get());
	
	renderItems();

	GuiRendering::popClipRect();

	const sf::Vector2f mousePos = getMousePos();
	sf::Vector2f worldMouse = Camera::screenToWorldPos(mousePos);
	if (Game::showDebugText)
		drawColor(mousePos, Map::getColor(worldMouse));

	if (Game::showDebugText)
	{
		sf::Vector2f collision = Map::nearestCollision(worldMouse);
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

    if(Game::showDebugText)
	{
		sf::Vector2f collision = Map::nearestCollectible(worldMouse).pos;
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

    if(Game::showDebugText)
	{
		sf::Vector2f collision = Map::nearestShop(worldMouse);
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
}

static sf::Vector2f nearestColorImpl(sf::Vector2f pos, sf::Color item, sf::Image &image, bool acceptBorders)
{
	sf::Vector2i center = worldToMapPos(pos);
	int nearestDist = 1000000;
	sf::Vector2i nearest(-10000, -10000);
	int kernelSize = 25;
	for (int i = 0; i < kernelSize * kernelSize; ++i)
	{
		int ix = (i % kernelSize) - kernelSize / 2;
		int iy = (i / kernelSize) - kernelSize / 2;
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
	return nearestColorImpl(pos, wallColor, image, true);
}

Treasure& Map::nearestCollectible(sf::Vector2f pos)
{
	float nearestDist = 10000000000.0f;
    Treasure* nearest;
	for (Treasure& treasure : treasures)
	{
		sf::Vector2f diff = treasure.pos - pos;
		float dist = diff.x * diff.x + diff.y * diff.y;
		if (dist >= nearestDist)
			continue;

		nearestDist = dist;
		nearest = &treasure;
	}

	return *nearest;
}

sf::Vector2f Map::nearestShop(sf::Vector2f pos)
{
	float nearestDist = 10000000000.0f;
	sf::Vector2f nearest(-1000, -1000);
	for (sf::Vector2f shop : shops)
	{
		sf::Vector2f diff = shop - pos;
		float dist = diff.x * diff.x + diff.y * diff.y;
		if (dist >= nearestDist)
			continue;

		nearestDist = dist;
		nearest = shop;
	}

	return nearest;
}

Item Map::pickupNearestCollectible(sf::Vector2f pos)
{
	if (treasures.empty())
		return Item::JewelS;

	float nearestDist = 4.0f;
	int nearest = -1;
	for (int i = 0; i < treasures.size(); ++i)
	{
		sf::Vector2f diff = treasures[i].pos - pos;
		float dist = diff.x * diff.x + diff.y * diff.y;
		if (dist >= nearestDist)
			continue;

		nearestDist = dist;
		nearest = i;
	}

	if (nearest == -1)
		return Item::JewelS;

	Item result = treasures[nearest].item;
	treasures.erase(treasures.begin() + nearest);
	return result;
}

sf::Color Map::getColor(sf::Vector2f pos)
{
	sf::Vector2i mapPos = worldToMapPos(pos);
	if (mapPos.x < 0 || mapPos.x >= (float)image.getSize().x || mapPos.y < 0 || mapPos.y > (float)image.getSize().y)
		return sf::Color(0, 0, 0, 0);

	sf::Color color = image.getPixel(mapPos.x, mapPos.y);
	return color;
}
