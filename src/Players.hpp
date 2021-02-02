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
    sf::Uint16 score, frameId;
    bool activePlayer = false;
    float posX, posY;
    float velocityX, velocityY;
    sf::Vector2f realInputVelocity;
    float inputVelocityX, inputVelocityY;
    std::string debugstring;
    OrthogonalDirection latestDirection = OrthogonalDirection::Down;
    void updatePlayer(float deltaTime, bool ownPlayer);
    void drawPlayer(bool debug, bool own);
private:
    float size = 1.0001f;
    float colliderSizeMultiplier = 0.8151287f;
    float closestEver = 100.001f;
    // mining
    struct {
        const float startSize = 0.1f;
        const float endSize = 0.5;
        const float range = 2;
        const float startPower = 10; // per second
        const float maxPower = 500; // per second
        const float timesPerFrameMin = 1;
        const float timesPerFrameMax = 10;
    } laser;
    
    struct PlayerTreasure{
        Item item;
        sf::Vector2f pos;
        sf::Uint8 id;
    };
    // my treasures
    int treasureCount = 0;
    PlayerTreasure myTreasures[26];
    float treasureMaxDistance = 1;
    const float sellingDistance = 5.0f;
    const float soldDistance = 0.01f;
};

void initializePlayers (float startX, float startY);
void updatePlayers(float deltaTime);
void drawPlayers(bool showDebugText);
Player& setActivePlayerIndex(int newActivePlayerId);
Player& getPlayer(int index);
Player& getOwnPlayer();
