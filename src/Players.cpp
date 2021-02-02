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
#include "Global.h"
#include <iostream>

namespace
{

static const float maxSpeed = 10;
static const float idleSpeed = 0.01f;
static const float inputLerp = 0.2f;
static const float accelerationLerp = 0.5f;

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

void initializePlayers (sf::Vector2f startPos) {
    for(int i = 0; i < sizeof(players)/sizeof(players[0]); i++){
        players[i].index = i;
        players[i].score = 0;
        players[i].frameId = 0;
        players[i].pos = startPos;
        players[i].velocity = sf::Vector2f(0, 0);
        players[i].inputVelocity = sf::Vector2f(0, 0);
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
        //  normalize input
        float magnitude = distanceXY(horizontalMove, verticalMove, 0, 0);
        if(magnitude > 1){
            horizontalMove /= magnitude;
            verticalMove /= magnitude;
        }
        // lerp input
        inputVelocity = sf::Vector2f(
            clamp11(lerp(inputVelocity.x, horizontalMove, inputLerp)),
            clamp11(lerp(inputVelocity.y, verticalMove, inputLerp))
            );
        realInputVelocity = sf::Vector2f(horizontalMove, verticalMove);
        // update frame counter
        ++frameId;
    }
    // accelerate
    velocity =  lerpVector2f(inputVelocity * maxSpeed, velocity, accelerationLerp);
    // move
    sf::Vector2f newPos = pos + velocity * deltaTime;
    
    // get nearest collision
    sf::Vector2f collisionPosition = g_map->nearestCollision(newPos);
    // if closer than player size move back
    sf::Vector2f direction = collisionPosition - newPos;
    float distance = distanceXY(0, 0, direction.x, direction.y);
    if(distance < closestEver){
        closestEver  = distance;
    }
    if(distance < 0.02f || !Map::isInMapArea(newPos)){
        // inside a wall
        newPos = Map::getShopPos();
    }
    else if(distance < 0.5f * colliderSizeMultiplier * size){
        // Collision omg
        velocity = sf::Vector2f(0, 0);
        sf::Vector2f newGoodPosition = newPos - (colliderSizeMultiplier * size * 0.5f - distance) * (direction / distance);
        newPos = newGoodPosition;
    }
    pos = newPos;
    
    debugstring = "x " + std::to_string((int)pos.x) +
    " y " + std::to_string((int)pos.y) +
    " iVX " + std::to_string(inputVelocity.x) +
    " iVY " + std::to_string(inputVelocity.y) +
    // " name " + std::to_string(index) +
    " score " + std::to_string(score);
    //new position ready, check if any world object is nearby
    Treasure* nearestTreasurePtr = g_map->nearestCollectible(newPos);
    if (!nearestTreasurePtr)
        return;

    Treasure& nearestTreasure = *nearestTreasurePtr;

    float distanceToNearestTreasure = magnitudeVector2(newPos -  nearestTreasure.pos);
    float debugH = 0;
    if(distanceToNearestTreasure < laser.range && ownPlayer){
        // start the drill
        float t = 1 - distanceToNearestTreasure / laser.range;
        float power = lerp(laser.startPower, laser.maxPower, t) * deltaTime;
        nearestTreasure.health -= power;
        if(nearestTreasure.health < 0){
            myTreasures[treasureCount].item = nearestTreasure.item;
            myTreasures[treasureCount].pos = nearestTreasure.pos;
            myTreasures[treasureCount].id = nearestTreasure.id;
            treasureCount++;
            treasureMaxDistance = 1 - 0.5f * (treasureCount/25);
            score++;
            Item item = g_map->pickupNearestCollectible(nearestTreasure.pos);
        }
    }
    // update treasure trail
    bool inSellingDistances = false;
    auto shopPos = Map::getShopPos();
    if(magnitudeVector2(shopPos - newPos) < sellingDistance){
        inSellingDistances = treasureCount > 0;
    }
    sf::Vector2f previous = pos;
    for(int i =  0; i < treasureCount - inSellingDistances ? 1 : 0; ++i){
        float distanceToPrevious = magnitudeVector2(myTreasures[i].pos - previous);
        if(distanceToPrevious > treasureMaxDistance){
            sf::Vector2f direction = previous - myTreasures[i].pos; // from treasure to "previous" in line
            sf::Vector2f target = previous - (direction / distanceToPrevious) * treasureMaxDistance;
            myTreasures[i].pos = lerpVector2f(myTreasures[i].pos, target, 0.15f );
        }
        previous = myTreasures[i].pos;
    }
    if(inSellingDistances){
        //move closer to shop
        float distanceToShop = magnitudeVector2(myTreasures[treasureCount-1].pos - shopPos);
        debugstring += " d2Shop " + std::to_string(distanceToShop);
        if(distanceToShop < soldDistance){
            //sold
            score += 100;
            treasureCount--;
        }
        else{
            if(distanceToShop > 1){
                sf::Vector2f direction = shopPos - myTreasures[treasureCount - 1].pos; // from treasure to "previous" in line
                sf::Vector2f target = myTreasures[treasureCount - 1].pos + (direction / distanceToShop) * 14.0f * deltaTime;
                myTreasures[treasureCount - 1].pos = lerpVector2f(myTreasures[treasureCount-1].pos, target, 0.5f );
            }
            else{
                myTreasures[treasureCount-1].pos = lerpVector2f(myTreasures[treasureCount-1].pos, shopPos, 0.15f );
            }
            debugstring += " selling";
        }
    }
}

void Player::drawPlayer(bool debug, bool own){
    if(!activePlayer) return;
    
    // Draw treasures
    sf::Vector2f s = Camera::worldToScreenSize(itemSize);
    for(int i = 0; i < treasureCount; ++i){
        
        auto pos = myTreasures[i].pos - itemSize * 0.5f;
        sf::Vector2f p = Camera::worldToScreenPos(pos);
        auto img = &Resources::getItemTexture(myTreasures[i].item);
        GuiRendering::image(img, p, s.x, s.y);
    }
    
    // what direction
    OrthogonalDirection direction = latestDirection;
    if(fabs(velocity.x) > fabs(velocity.y)) {
        if(fabs(velocity.x) > idleSpeed) {
            direction = velocity.x > 0 ? OrthogonalDirection::Right : OrthogonalDirection::Left;
        }
    }
    else{
        if(fabs(velocity.y) > idleSpeed) {
            direction = velocity.y > 0 ? OrthogonalDirection::Down : OrthogonalDirection::Up;
        }
    }
    latestDirection = direction;
    
    float playerSizeOnScreen  = Camera::worldToScreenSize(sf::Vector2f(size,size)).x;
    GuiRendering::image(own ? &Resources::getResources().getPlayerTexture(index, direction)
                        : &Resources::getResources().getFriendTexture(index, direction), Camera::worldToScreenPos(pos.x - 0.5f * size, pos.y - 0.5f * size), playerSizeOnScreen, playerSizeOnScreen);
    
    //check if any world object is nearby
    Treasure *nearestTreasurePtr = g_map->nearestCollectible(pos);
    if (nearestTreasurePtr)
    {
        Treasure& nearestTreasure = *nearestTreasurePtr;
        float distanceToNearestTreasure = magnitudeVector2(pos - nearestTreasure.pos);
        if (distanceToNearestTreasure < laser.range) {
            // start the drill
            float t = 1 - distanceToNearestTreasure / laser.range;
            float power = lerp(laser.startPower, laser.maxPower, t);
            float timesPerFrame = lerp(laser.timesPerFrameMin, laser.timesPerFrameMax, t);
            for (float f = 0; f < timesPerFrame; ++f) {
                sf::Vector2f start(pos + sf::Vector2f(getRandomNormal01() * 2 - 1 , (getRandomNormal01() * 2 - 1)) * laser.startSize);
                sf::Vector2f end(nearestTreasure.pos + sf::Vector2f(getRandomNormal01() * 2 - 1, (getRandomNormal01() * 2 - 1)) * laser.endSize);
                sf::Vector2f end2(nearestTreasure.pos + sf::Vector2f(getRandomNormal01() * 2 - 1, getRandomNormal01() * 2 - 1) * laser.endSize);

                GuiRendering::triangle(Camera::worldToScreenPos(start), Camera::worldToScreenPos(end), Camera::worldToScreenPos(end2), sf::Color::White);
            }
        }
        if(debug)
            GuiRendering::line(Camera::worldToScreenPos(pos), Camera::worldToScreenPos(nearestTreasure.pos));
        
    }
    if(debug){
        GuiRendering::text(debugstring.c_str(), 0.02f,  Camera::worldToScreenPos(pos.x, pos.y - 0.1f));
        sf::Vector2f collisionPosition = g_map->nearestCollision(pos);
        collisionPosition.x += getRandomNormal01();
        GuiRendering::line(Camera::worldToScreenPos(pos), Camera::worldToScreenPos(collisionPosition));
    }
    
}
