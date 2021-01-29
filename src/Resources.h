#pragma once

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

struct Resources
{
    
	static Resources &getResources()
	{
		static Resources resources;
		return resources;
	}

	std::unordered_map<int, sf::Texture> tileTextures;
    sf::Font font;
    
private:
	Resources();
};
