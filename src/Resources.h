#pragma once

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

struct Resources
{
	static Resources &getResources()
	{
		static Resources resources;
		return resources;
	}

	std::unordered_map<int, sf::Texture> tileTextures;

private:
	Resources()
	{
		tileTextures[0].loadFromFile("assets/block.png");;
		tileTextures[1].loadFromFile("assets/pawn.png");
	}
};
