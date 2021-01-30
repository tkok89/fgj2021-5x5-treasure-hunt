//
//  Players.hpp
//  fgj2021-lost-and-found
//
//  Created by Tomi Kokkonen on 30.1.2021.
//  Copyright © 2021 Team Manilla. All rights reserved.
//

#pragma once

#include "Resources.h"
struct Player {
    int index;
    bool activePlayer = false;
    float posX, posY;
    float velocityX, velocityY;
    float inputVelocityX, inputVelocityY;
    OrthogonalDirection latestDirection = OrthogonalDirection::Down;
    void updatePlayer(float deltaTime);
    void drawPlayer(bool debug);
private:
    float size = 1.00000f;
};

void initializePlayers (float startX, float startY);
void updatePlayers(float deltaTime);
void drawPlayers(bool showDebugText);
Player& getPlayer(int index);

