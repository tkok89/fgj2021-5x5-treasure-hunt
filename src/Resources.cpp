#include "Resources.h"

#include <iostream>
#include <assert.h>
#include "Global.h"
#include <random>

#ifdef TARGET_OS_MAC
#include "ResourcePath.hpp"
#else
std::string resourcePath(void) {
	return "";
}
#endif

std::string Resources::getResourcePath(const char* assetPath)
{
	return resourcePath() + assetPath;
}

sf::Texture* g_placeholder = nullptr;

Resources::Resources()
{
	bool success = false;
	success = placeholder.loadFromFile(getResourcePath("assets/pawn.png"));
	g_placeholder = &placeholder;
	assert(success);

	// Player textures
	std::random_device rd;
	std::default_random_engine rng(rd());
	std::uniform_int_distribution<size_t> distribution(0, 100);
	bool useCat = distribution(rng) > 50;

	std::string ownName = useCat ? "cat" : "main_character";
	std::string friendName = !useCat ? "cat" : "main_character";

	success = playerTextures[OrthogonalDirection::Left].loadFromFile(getResourcePath(("assets/" + ownName + "_left.png").c_str()));
	assert(success);
	success = playerTextures[OrthogonalDirection::Up].loadFromFile(getResourcePath(("assets/" + ownName + "_back.png").c_str()));
	assert(success);
	success = playerTextures[OrthogonalDirection::Right].loadFromFile(getResourcePath(("assets/" + ownName + "_right.png").c_str()));
	assert(success);
	success = playerTextures[OrthogonalDirection::Down].loadFromFile(getResourcePath(("assets/" + ownName + "_front.png").c_str()));
	assert(success);

	success = friendTextures[OrthogonalDirection::Left].loadFromFile(getResourcePath(("assets/" + friendName + "_left.png").c_str()));
	assert(success);
	success = friendTextures[OrthogonalDirection::Up].loadFromFile(getResourcePath(("assets/" + friendName + "_back.png").c_str()));
	assert(success);
	success = friendTextures[OrthogonalDirection::Right].loadFromFile(getResourcePath(("assets/" + friendName + "_right.png").c_str()));
	assert(success);
	success = friendTextures[OrthogonalDirection::Down].loadFromFile(getResourcePath(("assets/" + friendName + "_front.png").c_str()));
	assert(success);

	// Health
	success = healthBack.loadFromFile(getResourcePath("assets/health_back.png"));
	assert(success);
	success = healthFront.loadFromFile(getResourcePath("assets/health.png"));
	assert(success);
	success = rock.loadFromFile(getResourcePath("assets/rocks.png"));
	assert(success);


	mapName = getResourcePath("assets/map.png");

	if (!font.loadFromFile(getResourcePath("assets/Vera.ttf")))
	{
		std::cerr << "Error loading Vera.ttf" << std::endl;
	}

	// Load sound effects

	success = soundEffects[SoundResourceName::bonk]
		.loadFromFile(getResourcePath("assets/sounds/bonk.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::break_]
		.loadFromFile(getResourcePath("assets/sounds/break.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::button_press]
		.loadFromFile(getResourcePath("assets/sounds/button_press.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::gameover]
		.loadFromFile(getResourcePath("assets/sounds/gameover.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::miu]
		.loadFromFile(getResourcePath("assets/sounds/miu.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::mjau]
		.loadFromFile(getResourcePath("assets/sounds/mjau.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::monster_ded]
		.loadFromFile(getResourcePath("assets/sounds/monster_ded.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::mouse_hit_wall]
		.loadFromFile(getResourcePath("assets/sounds/mouse_hit_wall.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::mouse_mus_mus]
		.loadFromFile(getResourcePath("assets/sounds/mouse_mus_mus.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::movetiles_1]
		.loadFromFile(getResourcePath("assets/sounds/movetiles_1.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::movetiles_2]
		.loadFromFile(getResourcePath("assets/sounds/movetiles_2.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::movetiles_3]
		.loadFromFile(getResourcePath("assets/sounds/movetiles_3.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::o_oui]
		.loadFromFile(getResourcePath("assets/sounds/o_oui.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::peepeep]
		.loadFromFile(getResourcePath("assets/sounds/peepeep.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::togglebutton_1]
		.loadFromFile(getResourcePath("assets/sounds/togglebutton_1.ogg"));
	assert(success);

	success = soundEffects[SoundResourceName::togglebutton_2]
		.loadFromFile(getResourcePath("assets/sounds/togglebutton_2.ogg"));
	assert(success);

	success = itemTextures[Item::JewelS].loadFromFile(getResourcePath("assets/jewel_s.png"));
	assert(success);

	success = itemTextures[Item::JewelM].loadFromFile(getResourcePath("assets/jewel_m.png"));
	assert(success);

	success = itemTextures[Item::JewelL].loadFromFile(getResourcePath("assets/jewel_l.png"));
	assert(success);

	success = itemTextures[Item::Dynamite].loadFromFile(getResourcePath("assets/dynamite.png"));
	assert(success);

	success = itemTextures[Item::AfrikanTahti].loadFromFile(getResourcePath("assets/afrikantahti.png"));
	assert(success);

	success = itemTextures[Item::FontinaCheese].loadFromFile(getResourcePath("assets/fontina.png"));
	assert(success);

	success = shopTexture.loadFromFile(getResourcePath("assets/shop.png"));
	assert(success);
}

sf::Texture& Resources::getPlayerTexture(int pIndex, OrthogonalDirection direction) { // 0 left, 1 up, 2 right, 3 down
	return playerTextures[direction];
}

sf::Texture& Resources::getFriendTexture(int pIndex, OrthogonalDirection direction) { // 0 left, 1 up, 2 right, 3 down
	return friendTextures[direction];
}

std::unique_ptr<sf::Music> Resources::getMusic(MusicResourceName resName) const
{
	auto getPath = [](MusicResourceName name) {
		switch (name)
		{
		case MusicResourceName::thebiisi:
			return getResourcePath("assets/music/thebiisi.ogg");

		case MusicResourceName::pimpom:
			return getResourcePath("assets/music/PIMPOM.ogg");

		case MusicResourceName::syntti:
			return getResourcePath("assets/music/syntti.ogg");
		case MusicResourceName::syntti130:
		default:
			return getResourcePath("assets/music/syntti130bpm.ogg");

		}
	};

	std::unique_ptr<sf::Music> music;
	music.reset(new sf::Music());

	bool success = music->openFromFile(getPath(resName));
	assert(success);

	return music;
}

std::unique_ptr<sf::Shader> Resources::getShader(ShaderResourceName name) const
{
	std::unique_ptr<sf::Shader> shader(new sf::Shader());

	bool success = false;
	switch (name)
	{
	case ShaderResourceName::mapVis:
		success = shader->loadFromFile(
			getResourcePath("assets/shader/mapvis.frag.glsl"),
			sf::Shader::Type::Fragment
		);
		assert(success);
		break;
	}

	return shader;
}

sf::Texture& Resources::getItemTexture(Item item)
{
	return getResources().itemTextures[item];
}

