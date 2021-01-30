#pragma once

#include <memory>
#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/String.hpp>

#include "Global.h"

enum class SoundResourceName : uint32_t
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

enum class MusicResourceName : uint32_t
{
	thebiisi,
	pimpom
};

struct Resources
{
	static Resources &getResources()
	{
		static Resources resources;
		return resources;
	}

	sf::Texture placeholder;
	sf::Font font;

	std::unordered_map<SoundResourceName, sf::SoundBuffer> soundEffects;
	sf::String mapName;

	std::unique_ptr<sf::Music> getMusic(MusicResourceName musicResourceName) const;

	static std::string getResourcePath(const char *assetPath);

private:
	Resources();
};
