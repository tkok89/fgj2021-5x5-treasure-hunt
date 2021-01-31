#include "Mankka.h"

#include <assert.h>
#include <random>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include "Resources.h"

namespace
{

struct SoundPreset
{
	std::vector<SoundResourceName> pool;
	float pitchVariance = 0;
};

struct PlaybackState
{
	std::unordered_map<SoundResourceName, sf::Sound> sounds;
};

struct MusicState
{
	MusicEnvName env = MusicEnvName::silence;
	MusicResourceName resourceName;
	std::unique_ptr<sf::Music> playingMusic;
};

}

struct Mankka::MankkaState
{
	std::unordered_map<SoundPresetName, SoundPreset> presets;
	std::unordered_map<SoundPresetName, PlaybackState> playbackStates;
	std::unordered_map<MusicEnvName, MusicResourceName> musicEnvs;
	MusicState musicState;
	std::default_random_engine rng;
};

Mankka::Mankka()
{
	auto &resources = Resources::getResources();

	state.reset(new MankkaState());

	state->presets[SoundPresetName::bonk] =
	{
		{ SoundResourceName::bonk }
	};

	state->presets[SoundPresetName::hajotus] =
	{
		{ SoundResourceName::break_ }
	};

	state->presets[SoundPresetName::button_press] =
	{
		{ SoundResourceName::button_press }
	};

	state->presets[SoundPresetName::gameover] =
	{
		{ SoundResourceName::gameover }
	};

	state->presets[SoundPresetName::monster_move] =
	{
		{
			SoundResourceName::miu,
			SoundResourceName::mjau
		},
		0.2f
	};

	state->presets[SoundPresetName::monster_ded] =
	{
		{ SoundResourceName::monster_ded }
	};

	state->presets[SoundPresetName::mouse_hit_wall] =
	{
		{ SoundResourceName::mouse_hit_wall }
	};

	state->presets[SoundPresetName::mouse_eat] =
	{
		{ SoundResourceName::mouse_mus_mus }
	};

	state->presets[SoundPresetName::movetiles] =
	{
		{
			SoundResourceName::movetiles_1,
			SoundResourceName::movetiles_2,
			SoundResourceName::movetiles_3,
		},
		0.1f
	};

	state->presets[SoundPresetName::mouse_dead] =
	{
		{ SoundResourceName::o_oui }
	};

	state->presets[SoundPresetName::peepeep] =
	{
		{ SoundResourceName::peepeep }
	};


	state->presets[SoundPresetName::togglebutton] =
	{
		{
			SoundResourceName::togglebutton_1,
			SoundResourceName::togglebutton_2
		},
		0.1f
	};

	for (const auto &presetsIt : state->presets)
	{
		for (SoundResourceName resName : presetsIt.second.pool)
		{
			sf::SoundBuffer &buf = resources.soundEffects[resName];
			state->playbackStates[presetsIt.first].sounds[resName] = sf::Sound(buf);
		}

		assert(presetsIt.second.pool.size() > 0);
	}

	// Music envs

	state->musicEnvs = {
		{ MusicEnvName::ingame, MusicResourceName::syntti130 },
		{ MusicEnvName::kidutuskammio, MusicResourceName::thebiisi }
	};
}

Mankka &Mankka::getMankka()
{
	static Mankka mankka;
	return mankka;
}

void Mankka::play(SoundPresetName presetId)
{
	const SoundPreset &preset = state->presets[presetId];
	PlaybackState &pbs = state->playbackStates[presetId];

	std::uniform_int_distribution<size_t> distribution(0, preset.pool.size() - 1);
	size_t pickIndex = distribution(state->rng);

	SoundResourceName effectName = preset.pool[pickIndex];
	sf::Sound &sound = pbs.sounds[effectName];

	if (preset.pitchVariance != 0)
	{
		std::uniform_real_distribution<float> distribution(0.f, preset.pitchVariance);
		float pitchDelta = distribution(state->rng);
		float pitch = 1 - preset.pitchVariance / 2.f + pitchDelta;
		sound.setPitch(pitch);
	}

	sound.play();
}

void Mankka::playMusic(MusicEnvName envName)
{
	if (state->musicState.env == envName)
		return;

	state->musicState.env = envName;

	if (state->musicState.env == MusicEnvName::silence)
	{
		state->musicState.playingMusic.reset();
	}
	else
	{
		state->musicState.resourceName = state->musicEnvs[envName];

		state->musicState.playingMusic =
			Resources::getResources().getMusic(state->musicState.resourceName);

		state->musicState.playingMusic->setLoop(true);
		state->musicState.playingMusic->play();
	}
}

void Mankka::stopMusic()
{
	if (state && state->musicState.playingMusic)
		state->musicState.playingMusic->stop();
}

void Mankka::test()
{
	play(SoundPresetName::movetiles);
}
