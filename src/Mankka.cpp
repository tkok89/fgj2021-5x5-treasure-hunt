#include "Mankka.h"

#include <vector>
#include <unordered_map>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Resources.h"

namespace
{

struct SoundPreset
{
	std::vector<SoundResourceName> pool;
};

}

struct Mankka::MankkaState
{
	std::unordered_map<SoundPresetName, SoundPreset> presets;
};

Mankka::Mankka()
{
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
		}
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
		}
	};

	state->presets[SoundPresetName::bonk] =
	{
		{ SoundResourceName::bonk }
	};
}

Mankka &Mankka::getMankka()
{
	static Mankka mankka;
	return mankka;
}

void Mankka::play(SoundPresetName presetId)
{
	//state->
}
