#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

struct Map;

extern sf::RenderWindow *g_window;
extern sf::Vector2f g_resolution;
extern Map *g_map;
sf::Vector2f getMousePos();

