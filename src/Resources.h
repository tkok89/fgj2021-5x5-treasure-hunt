#pragma once

#include <memory>
#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "Map.h"
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

enum OrthogonalDirection: uint32_t
{
    Left = 0,
    Up = 1,
    Right = 2,
    Down = 3
};

enum class ShaderResourceName
{
	mapVis
};

extern sf::Texture *g_placeholder;

enum class MusicResourceName : uint32_t
{
	thebiisi,
	pimpom,
	syntti,
	syntti130
};

struct Resources
{
	static Resources &getResources()
	{
		static Resources resources;
		return resources;
	}

    sf::Texture placeholder;
    sf::Texture healthFront;
    sf::Texture healthBack;
    sf::Texture rock;
    sf::Texture playerTextures[4];
    sf::Texture friendTextures[4];
	sf::Font font;

	std::unordered_map<SoundResourceName, sf::SoundBuffer> soundEffects;
	std::unordered_map<Item, sf::Texture> itemTextures;
	std::string mapName;

	std::unique_ptr<sf::Music> getMusic(MusicResourceName musicResourceName) const;
    sf::Texture& getPlayerTexture(int pIndex, OrthogonalDirection direction); // 0 left, 1 up, 2 right, 3 down
    sf::Texture& getFriendTexture(int pIndex, OrthogonalDirection direction); // 0 left, 1 up, 2 right, 3 down
	std::unique_ptr<sf::Shader> getShader(ShaderResourceName name) const;
	sf::Texture shopTexture;

	static sf::Texture &getItemTexture(Item item);
	static sf::Texture &getShopTexture() { return getResources().shopTexture; };

	static std::string getResourcePath(const char *assetPath);

private:
	Resources();
};
