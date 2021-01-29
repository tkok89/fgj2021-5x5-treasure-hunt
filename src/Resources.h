#pragma once

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

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

struct Resources
{
	static Resources &getResources()
	{
		static Resources resources;
		return resources;
	}

	std::unordered_map<int, sf::Texture> tileTextures;
  sf::Font font;

	std::unordered_map<SoundResourceName, sf::SoundBuffer> soundEffects;

private:
	Resources();
};
