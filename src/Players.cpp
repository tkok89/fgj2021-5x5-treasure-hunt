//
//  Players.cpp
//  fgj2021-lost-and-found
//
//  Created by Tomi Kokkonen on 30.1.2021.
//  Copyright © 2021 Team Manilla. All rights reserved.
//

#include "Players.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "GuiRendering.h"
#include "Resources.h"

#include <cmath>

namespace
{

static const float maxSpeed = 10;
static const float inputLerp = 0.5f;
static const float accelerationLerp = 0.5f;

inline float lerp(float a, float b, float t){
    return a * t + (1 - t) * b;
}

inline float distanceXY(float x0, float y0, float x1, float y1){
    return sqrt((x0-x1)*(x0-x1) + (y0-y1)*(y0-y1));
}

inline float clamp11(float x){
    if(x > 1){
        return 1;
    }
    if(x < -1){
        return -1;
    }
    return x;
}

struct Player {
    int index;
    bool activePlayer = false;
    float posX, posY;
    float velocityX, velocityY;
    float inputVelocityX, inputVelocityY;
    
    void updatePlayer(float deltaTime){
        if(!activePlayer) return;
        //read input
        float horizontalMove = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            horizontalMove -= 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            horizontalMove += 1;
        }
        float verticalMove = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            verticalMove -= 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            verticalMove += 1;
        }
        inputVelocityX =clamp11( lerp(inputVelocityX, horizontalMove, inputLerp));
        inputVelocityY =clamp11( lerp(inputVelocityY, verticalMove, inputLerp));
        //accelerate
        velocityX = lerp(inputVelocityX * maxSpeed, velocityX, accelerationLerp);
        velocityY = lerp(inputVelocityY * maxSpeed, velocityY, accelerationLerp);
        //move
        float newPosX = posX + velocityX * deltaTime;
        float newPosY = posY + velocityY * deltaTime;
        // try new position
        // float distance = worldCollision(this);
        // get back, if in wall
        posX = newPosX;
        posY = newPosY;
        //new position ready, check if any world object is nearby
    }
    
    void drawPlayer(){
        GuiRendering::image(&Resources::getResources().placeholder, posX, posY, 0.1, 0.1);
    }

} player1, player2, player3, player4;

static Player* players[4];

}

void initializePlayers () {
    players[0] = &player1;
    players[1] = &player2;
    players[2] = &player3;
    players[3] = &player4;
    for(int i = 0; i < sizeof(players)/sizeof(players[0]); i++){
        players[i]->index = 0;
        players[i]->posX = 0;
        players[i]->posY = 0;
        players[i]->velocityX = 0;
        players[i]->velocityY = 0;
        players[i]->inputVelocityX =0;
        players[i]->inputVelocityY =0;
    }
}

void drawPlayers(){
    for(int i = 0; i < sizeof(players)/sizeof(players[0]); i++){
        players[i]->drawPlayer();
    }
}

void updatePlayers(float deltaTime){
    for(int i = 0; i < sizeof(players)/sizeof(players[0]); i++){
        players[i]->updatePlayer(deltaTime);
    }
}


