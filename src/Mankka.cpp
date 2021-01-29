#include "Mankka.h"

#include <vector>
#include <unordered_map>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Resources.h"

namespace
{

struct SoundVarianceGroup
{
	std::vector<uint32_t> pool;
};

struct SoundPreset
{
	SoundVarianceGroup varianceGroup;
};

}

struct Mankka::MankkaState
{
	std::unordered_map<SoundPresetName, SoundPreset> presets;
};

Mankka::Mankka()
{
	state.reset(new MankkaState());
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
