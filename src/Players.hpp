//
//  Players.hpp
//  fgj2021-lost-and-found
//
//  Created by Tomi Kokkonen on 30.1.2021.
//  Copyright © 2021 Team Manilla. All rights reserved.
//

#pragma once

#include "Resources.h"
#include <string>

struct Player {
    int index;
    bool activePlayer = false;
    float posX, posY;
    float velocityX, velocityY;
    float inputVelocityX, inputVelocityY;
    std::string debugstring;
    OrthogonalDirection latestDirection = OrthogonalDirection::Down;
    void updatePlayer(float deltaTime, bool ownPlayer);
    void drawPlayer(bool debug);
private:
    float size = 0.951287f;
};

void initializePlayers (float startX, float startY);
void updatePlayers(float deltaTime);
void drawPlayers(bool showDebugText);
Player& getPlayer(int index);
