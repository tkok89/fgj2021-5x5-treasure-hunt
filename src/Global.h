#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <assert.h>
#include <random>

struct Map;

extern sf::RenderWindow *g_window;
extern sf::Vector2f g_resolution;
extern Map *g_map;
sf::Vector2f getMousePos();

const sf::Vector2f itemSize(1, 1);

inline sf::Vector2f lerpVector2f(sf::Vector2f a, sf::Vector2f b, float t){
    assert(a == a);
    assert(b == b);
    return sf::Vector2f(b.x * t + (1-t)*a.x, b.y * t + (1-t) * a.y);
}

inline float clamp01(float a){
    return a > 0 ? (a > 1 ? 1 : a) : 0;
}

float getRandomNormal01();

inline float magnitudeVector2(sf::Vector2f vec){
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}
