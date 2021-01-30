#include"Grid.h"

#include "Game.h" // :)

sf::Vector2f getMousePos()
{
    sf::Vector2i p = sf::Mouse::getPosition(*g_window);
    sf::Vector2f r = Game::getResolution();
    return sf::Vector2f(p.x / r.y, p.y / r.y) - sf::Vector2f(0.5f * r.x / r.y, 0.5f);
}

sf::Vector2f getPos(int x, int y)
{
    sf::Vector2f place = gridPos + sf::Vector2f(x * tileSize.x, y * tileSize.y);
    return place;
}

