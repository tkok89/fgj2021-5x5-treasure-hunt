#pragma once

#include <inttypes.h>
#include <memory>

enum class SoundPresetName : uint32_t
{
	bonk,
	hajotus, // =break
	button_press,
	gameover,
	monster_move, // miu, mjau
	monster_ded,
	mouse_hit_wall,
	mouse_eat, // =mouse_mus_mus
	movetiles, // movetiles_1, movetiles_2, movetiles_3
	mouse_dead, // =o_oui
	peepeep,
	togglebutton, // togglebutton_1, togglebutton_2
};

class Mankka
{
public:
	Mankka();
	static Mankka &getMankka();

	void play(SoundPresetName presetId);

private:
	struct MankkaState;
	std::unique_ptr<MankkaState> state;
};
