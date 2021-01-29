#include"Grid.h"

#include "Game.h" // :)

sf::Vector2f getMousePos()
{
    sf::Vector2i p = sf::Mouse::getPosition();
    float r = Game::getResolution().y;
    return sf::Vector2f(p.x / r, p.y / r) - sf::Vector2f(0.5f, 0.5f);
}
