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
#include "Camera.h"
#include "Map.h"
#include <cmath>
#include <string>

namespace
{

static const float maxSpeed = 10;
static const float idleSpeed = 0.01;
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

static Player players[8];
static int ownPlayerId = 0;
}

void initializePlayers (float startX, float startY) {
    for(int i = 0; i < sizeof(players)/sizeof(players[0]); i++){
        players[i].index = i;
        players[i].posX = startX;
        players[i].posY = startY;
        players[i].velocityX = 0;
        players[i].velocityY = 0;
        players[i].inputVelocityX =0;
        players[i].inputVelocityY =0;
    }
    players[0].activePlayer = true;
}

void drawPlayers(bool showDebugText){
    for(int i = 0; i < sizeof(players)/sizeof(players[0]); i++){
        players[i].drawPlayer(showDebugText, i == ownPlayerId);
    }
}

void updatePlayers(float deltaTime){
    for(int i = 0; i < sizeof(players)/sizeof(players[0]); i++){
        players[i].updatePlayer(deltaTime, i == ownPlayerId);
    }
}

Player& setActivePlayerIndex(int newOwnPlayerId){
    players[newOwnPlayerId].activePlayer = true;
    ownPlayerId = newOwnPlayerId;
    return players[newOwnPlayerId];
}

Player& getPlayer(int index){
    return players[index];
}

Player& getOwnPlayer(){
    return players[ownPlayerId];
}


void Player::updatePlayer(float deltaTime, bool ownPlayer){
    if(!activePlayer) return;
    if(ownPlayer){
        // read input
        float horizontalMove = 0;
        float verticalMove = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            horizontalMove -= 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            horizontalMove += 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            verticalMove -= 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            verticalMove += 1;
        }   
        // lerp input
        inputVelocityX = clamp11( lerp(inputVelocityX, horizontalMove, inputLerp));
        inputVelocityY = clamp11( lerp(inputVelocityY, verticalMove, inputLerp));
    }
    // accelerate
    velocityX = lerp(inputVelocityX * maxSpeed, velocityX, accelerationLerp);
    velocityY = lerp(inputVelocityY * maxSpeed, velocityY, accelerationLerp);
    // move
    float newPosX = posX + velocityX * deltaTime;
    float newPosY = posY + velocityY * deltaTime;
    
    // get nearest collision
    sf::Vector2f collisionPosition = g_map->nearestCollision(sf::Vector2f(newPosX, newPosY));
    // if closer than player size move back
    sf::Vector2f direction = collisionPosition - sf::Vector2f(newPosX, newPosY);
    float distance = distanceXY(0, 0, direction.x, direction.y);
    if(distance < closestEver){
        closestEver  = distance;
    }
    if(distance < 0.02f || !Map::isInMapArea(newPosX, newPosY)){
        // inside a wall
        newPosX = Map::getShopPos().x;
        newPosY = Map::getShopPos().y;
    }
    else if(distance < 0.5f * size){
        // Collision omg
        velocityX = 0;
        velocityY = 0;
        sf::Vector2f newGoodPosition = sf::Vector2f(newPosX, newPosY) - (size * 0.5f - distance) * (direction / distance);
        newPosY = newGoodPosition.y;
        newPosX = newGoodPosition.x;
    }
    
    // get back, if in wall
    posX = newPosX;
    posY = newPosY;
    //new position ready, check if any world object is nearby
    
    debugstring = "x " + std::to_string(posX) +
    " y " + std::to_string(posY) +
    " name " + std::to_string(index) +
    " distance " + std::to_string(distance);
}

void Player::drawPlayer(bool debug, bool own){
    if(!activePlayer) return;
    // what direction
    OrthogonalDirection direction = latestDirection;
    if(fabs( velocityX) > fabs(velocityY)) {
        if(fabs(velocityX) > idleSpeed) {
            direction = velocityX > 0 ? OrthogonalDirection::Right : OrthogonalDirection::Left;
        }
    }
    else{
        if(fabs(velocityY) > idleSpeed) {
            direction = velocityY > 0 ? OrthogonalDirection::Down : OrthogonalDirection::Up;
        }
    }
    latestDirection = direction;
    
    float playerSizeOnScreen  = Camera::worldToScreenSize(sf::Vector2f(size,size)).x;
    GuiRendering::image(own ? &Resources::getResources().getPlayerTexture(index, direction)
                        : &Resources::getResources().getFriendTexture(index, direction), Camera::worldToScreenPos(posX- 0.5f*size, posY-0.5f*size), playerSizeOnScreen, playerSizeOnScreen);
    if(debug){
        GuiRendering::text(debugstring.c_str(), 0.02f,  Camera::worldToScreenPos(posX, posY - 0.1f));
        sf::Vector2f collisionPosition = g_map->nearestCollision(sf::Vector2f(posX, posY));
        GuiRendering::line(Camera::worldToScreenPos(posX, posY), Camera::worldToScreenPos(collisionPosition));
    }
}
