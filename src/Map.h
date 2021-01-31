#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include "Global.h"

const sf::Vector2f mapSize(100, 100);
const sf::Vector2f topLeft = mapSize * -0.5f;

enum class Item
{
	JewelS,
	JewelM,
	JewelL,
	Dynamite,
	AfrikanTahti,
};

struct Treasure
{
	Item item;
	sf::Vector2f pos;
};

struct Map
{
	Map();
	void randomize();
	void draw();

	sf::Vector2f nearestCollision(sf::Vector2f pos);
	sf::Vector2f nearestCollectible(sf::Vector2f pos);
	sf::Vector2f nearestShop(sf::Vector2f pos);
	static sf::Vector2f getShopPos() { return g_map->shops.front(); }
    static bool isInMapArea(float x, float y) { return x> topLeft.x && y > topLeft.y && x < topLeft.x + mapSize.x  &&  y < topLeft.y + mapSize.y;}
	sf::Color getColor(sf::Vector2f pos);

	Item pickupNearestCollectible(sf::Vector2f pos);

	sf::Image image;
	sf::Texture texture;
	sf::Image mapSDFImage;
	std::unique_ptr<sf::Shader> mapVisShader;
	std::vector<Treasure> treasures;
	std::vector<sf::Vector2f> shops;
};

