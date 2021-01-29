#include"Grid.h"

#include "Game.h" // :)

sf::Vector2f getMousePos()
{
    sf::Vector2i p = sf::Mouse::getPosition();
    sf::Vector2f r = Game::getResolution();
    return sf::Vector2f(p.x / r.x, p.y / r.y);
}
