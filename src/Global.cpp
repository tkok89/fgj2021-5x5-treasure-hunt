#include "Global.h"

#include "Game.h"

sf::Vector2f getMousePos()
{
    sf::Vector2i p = sf::Mouse::getPosition(*g_window);
    sf::Vector2f r = Game::getResolution();
    return sf::Vector2f(p.x / r.y, p.y / r.y) - sf::Vector2f(0.5f * r.x / r.y, 0.5f);
}


float getRandomNormal01() {
    static std::random_device rd;
    static std::default_random_engine rng(rd());
    static std::normal_distribution<float> distribution(0.5f, 0.2f);
    return distribution(rng);
}
