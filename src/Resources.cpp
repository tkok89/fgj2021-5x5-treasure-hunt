#include "Resources.h"

#include <iostream>
#include <assert.h>

#ifdef TARGET_OS_MAC
#include "ResourcePath.hpp"
#else
std::string resourcePath(void){
    return "";
}
#endif

Resources::Resources()
{
    bool success = false;
    success = tileTextures[0].loadFromFile(resourcePath() + "assets/block.png");
    assert(success);

    success = tileTextures[1].loadFromFile(resourcePath() + "assets/pawn.png");
    assert(success);

    if (!font.loadFromFile(resourcePath() + "assets/Vera.ttf"))
    {
        std::cerr << "Error loading Vera.ttf" << std::endl;
    }

    success = soundEffects[SoundResourceName::bonk]
        .loadFromFile(resourcePath() + "assets/sounds/bonk.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::break_]
        .loadFromFile(resourcePath() + "assets/sounds/break.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::button_press]
        .loadFromFile(resourcePath() + "assets/sounds/button_press.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::gameover]
        .loadFromFile(resourcePath() + "assets/sounds/gameover.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::miu]
        .loadFromFile(resourcePath() + "assets/sounds/miu.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::mjau]
        .loadFromFile(resourcePath() + "assets/sounds/mjau.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::monster_ded]
        .loadFromFile(resourcePath() + "assets/sounds/monster_ded.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::mouse_hit_wall]
        .loadFromFile(resourcePath() + "assets/sounds/mouse_hit_wall.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::mouse_mus_mus]
        .loadFromFile(resourcePath() + "assets/sounds/mouse_mus_mus.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::movetiles_1]
        .loadFromFile(resourcePath() + "assets/sounds/movetiles_1.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::movetiles_2]
        .loadFromFile(resourcePath() + "assets/sounds/movetiles_2.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::movetiles_3]
        .loadFromFile(resourcePath() + "assets/sounds/movetiles_3.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::o_oui]
        .loadFromFile(resourcePath() + "assets/sounds/o_oui.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::peepeep]
        .loadFromFile(resourcePath() + "assets/sounds/peepeep.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::togglebutton_1]
        .loadFromFile(resourcePath() + "assets/sounds/togglebutton_1.ogg");
    assert(success);

    success = soundEffects[SoundResourceName::togglebutton_2]
        .loadFromFile(resourcePath() + "assets/sounds/togglebutton_2.ogg");
    assert(success);
}
