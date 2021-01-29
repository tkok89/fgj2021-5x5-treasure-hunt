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

    success = soundEffects[0].loadFromFile(resourcePath() + "assets/sounds/bonk.ogg");
    assert(success);

    success = soundEffects[1].loadFromFile(resourcePath() + "assets/sounds/break.ogg");
    assert(success);

    success = soundEffects[2].loadFromFile(resourcePath() + "assets/sounds/button_press.ogg");
    assert(success);

    success = soundEffects[3].loadFromFile(resourcePath() + "assets/sounds/gameover.ogg");
    assert(success);

    success = soundEffects[4].loadFromFile(resourcePath() + "assets/sounds/miu.ogg");
    assert(success);

    success = soundEffects[5].loadFromFile(resourcePath() + "assets/sounds/mjau.ogg");
    assert(success);

    success = soundEffects[6].loadFromFile(resourcePath() + "assets/sounds/monster_ded.ogg");
    assert(success);

    success = soundEffects[7].loadFromFile(resourcePath() + "assets/sounds/mouse_hit_wall.ogg");
    assert(success);

    success = soundEffects[8].loadFromFile(resourcePath() + "assets/sounds/mouse_mus_mus.ogg");
    assert(success);

    success = soundEffects[9].loadFromFile(resourcePath() + "assets/sounds/movetiles_1.ogg");
    assert(success);

    success = soundEffects[10].loadFromFile(resourcePath() + "assets/sounds/movetiles_2.ogg");
    assert(success);

    success = soundEffects[11].loadFromFile(resourcePath() + "assets/sounds/movetiles_3.ogg");
    assert(success);

    success = soundEffects[12].loadFromFile(resourcePath() + "assets/sounds/o_oui.ogg");
    assert(success);

    success = soundEffects[13].loadFromFile(resourcePath() + "assets/sounds/peepeep.ogg");
    assert(success);

    success = soundEffects[14].loadFromFile(resourcePath() + "assets/sounds/togglebutton_1.ogg");
    assert(success);

    success = soundEffects[15].loadFromFile(resourcePath() + "assets/sounds/togglebutton_2.ogg");
    assert(success);
}
