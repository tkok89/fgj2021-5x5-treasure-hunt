#pragma once

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

struct Resources
{
	static Resources &getResources()
	{
		static Resources resources;
		return resources;
	}

	std::unordered_map<int, sf::Texture> tileTextures;
  sf::Font font;

	std::unordered_map<uint32_t, sf::SoundBuffer> soundEffects;

private:
	Resources();
};
