#pragma once

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Grid.h"

enum class SoundResourceName
{
	bonk,
	break_,
	button_press,
	gameover,
	miu,
	mjau,
	monster_ded,
	mouse_hit_wall,
	mouse_mus_mus,
	movetiles_1,
	movetiles_2,
	movetiles_3,
	o_oui,
	peepeep,
	togglebutton_1,
	togglebutton_2
};

extern sf::Texture *g_placeholder;

struct Resources
{
	static Resources &getResources()
	{
		static Resources resources;
		return resources;
	}

	sf::Texture placeholder;
	std::unordered_map<TileType, sf::Texture> tileTextures;
	sf::Font font;

	std::unordered_map<SoundResourceName, sf::SoundBuffer> soundEffects;

	static std::string getResourcePath(const char *assetPath);

private:
	Resources();
};
