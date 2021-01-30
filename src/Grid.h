#pragma once

#include <vector>
#include <assert.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

enum { GridSize = 5 };

const sf::Vector2f tileSize = sf::Vector2f(1, 1) / float(GridSize + 2);
const sf::Vector2f gridPos = sf::Vector2f(-0.5f, -0.5f) + tileSize;
const sf::Vector2f gridSize = tileSize * float(GridSize);
sf::Vector2f getMousePos();
extern sf::RenderWindow *g_window;

struct Tile
{
	int type = 0;
};

struct ShapePart
{
	Tile tile;
	int x = 0;
	int y = 0;
};

struct Shape
{
	std::vector<ShapePart> parts;

	Shape offset(int x, int y)
	{
		Shape result;
		for (const ShapePart &shapePart : parts)
		{
			result.parts.push_back(shapePart);
			result.parts.back().x += x;
			result.parts.back().y += y;
		}
		return result;
	}
};

enum PushDirection
{
	PushUp,
	PushDown,
	PushLeft,
	PushRight,
};

struct Grid
{
	Tile grid[GridSize][GridSize] = { };

	Tile &operator()(int x, int y) { assert(contains(x, y)); return grid[x][y]; }
	const Tile &operator()(int x, int y) const { assert(contains(x, y)); return grid[x][y]; }
	bool contains(int x) const { return x >= 0 && x < GridSize; }
	bool contains(int x, int y) const { return contains(x) && contains(y); }

	bool fits(const ShapePart &shapePart) const
	{
		if (!contains(shapePart.x, shapePart.y))
			return false;

		return true;
	}

	bool fits(const Shape &shape) const
	{
		for (const ShapePart &shapePart : shape.parts)
		{
			if (!contains(shapePart.x, shapePart.y))
				return false;
		}

		return true;
	}

	void solidify(const Shape &shape)
	{
		assert(fits(shape)); 

		for (const ShapePart &shapePart : shape.parts)
		{
			operator()(shapePart.x, shapePart.y) = shapePart.tile;
		}
	}

	void push(const Shape &shape)
	{

	}

	Tile *getOptional(int x, int y)
	{
		if (contains(x, y))
			return &operator()(x, y);

		return nullptr;
	}
};

// HUOM: Vuotaa Tile-pointterin scopen ulkopuolelle
#define ForGrid(p_tile) \
Tile *p_tile = &grid(0,0); \
for (int p_tile##I = 0, p_tile##X = 0, p_tile##Y = 0; p_tile##I < GridSize * GridSize; ++p_tile##I, p_tile##X = p_tile##I % GridSize, p_tile##Y = p_tile##I / GridSize, p_tile = grid.getOptional(tileX, tileY))
