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
    void drawPlayer(bool debug, bool own);
private:
    float size = 1.0001f;
    float colliderSizeMultiplier = 0.8151287f;
    float closestEver = 100.001f;
};

void initializePlayers (float startX, float startY);
void updatePlayers(float deltaTime);
void drawPlayers(bool showDebugText);
Player& setActivePlayerIndex(int newActivePlayerId);
Player& getPlayer(int index);
Player& getOwnPlayer();
