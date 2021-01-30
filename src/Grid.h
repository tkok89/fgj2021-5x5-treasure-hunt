#pragma once

#include <vector>
#include <assert.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <random>
#include <sstream>

enum { GridSize = 5 };

const sf::Vector2f tileSize = sf::Vector2f(1, 1) / float(GridSize + 2);
const sf::Vector2f gridPos = sf::Vector2f(-0.5f, -0.5f) + tileSize;
const sf::Vector2f gridSize = tileSize * float(GridSize);
sf::Vector2f getMousePos();
sf::Vector2f getPos(int x, int y);
extern sf::RenderWindow *g_window;

enum class TileType : int
{
	Empty,
	EasySeed,
	EasyFlower,
	HardSeed,
	HardFlower,
	Sprinkler,
	Sun,
	COUNT,
};

enum class WallType : int
{
	WallFree = 0b0000,
	WallUp = 0b0001,
	WallDown = 0b0010,
	WallDownUp = 0b0011,
	WallLeft = 0b0100,
	WallLeftUp = 0b0101,
	WallLeftDown = 0b0110,
	WallLeftDownUp = 0b0111,
	WallRight = 0b1000,
	WallRightUp = 0b1001,
	WallRightDown = 0b1010,
	WallRightDownUp = 0b1011,
	WallRightLeft = 0b1100,
	WallRightLeftUp = 0b1101,
	WallRightLeftDown = 0b1110,
	WallRightLeftDownUp = 0b1111,
	COUNT,
};

inline std::string getWallName(WallType tile)
{
	int t = int(tile);

	std::stringstream s;
	s << "wall";
	if (tile == WallType::WallFree)
		s << "_free";

	if (t & int(WallType::WallUp))
		s << "_up";
	if (t & int(WallType::WallDown))
		s << "_down";
	if (t & int(WallType::WallLeft))
		s << "_left";
	if (t & int(WallType::WallRight))
		s << "_right";

	return s.str();
}

inline std::string getTileName(TileType tile)
{
	switch (tile)
	{
	case TileType::Empty:
		return "empty";
	case TileType::EasySeed:
		return "easy_seed";
	case TileType::EasyFlower:
		return "easy_flower";
	case TileType::HardSeed:
		return "hard_seed";
	case TileType::HardFlower:
		return "hard_flower";
	case TileType::Sprinkler:
		return "sprinkler";
	case TileType::Sun:
		return "sun";
	case TileType::COUNT:
		break;
	}

	assert(false);
	return "ERROR";
}

struct Tile
{
	TileType type = TileType::Empty;
	int lifeTime = 0;
	int willReceive = 0;
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

struct IncomingTiles
{
	Tile dragged = randomTile();

	Tile up;
	Tile down;
	Tile left;
	Tile right;

	static Tile randomTile()
	{
		TileType tileTypes[] = { TileType::EasySeed, TileType::HardSeed, TileType::Sprinkler, TileType::Sun };

		static std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(0, sizeof(tileTypes) / sizeof(TileType) - 1);
		TileType tileType = tileTypes[distribution(generator)];
		Tile tile;
		tile.type = tileType;
		return tile;
	}
};

struct RisingNumber
{
	int number = 0;
	sf::Vector2f pos;
	sf::Time time;
};

struct Score
{
	int score = 0;
	std::vector<RisingNumber> risingNumbers;
};

extern Grid *g_grid;
extern IncomingTiles *g_incomingTiles;
extern Score *g_score;

// HUOM: Vuotaa Tile-pointterin scopen ulkopuolelle
#define ForGrid(p_tile) \
Tile *p_tile = &grid(0,0); \
for (int p_tile##I = 0, p_tile##X = 0, p_tile##Y = 0; p_tile##I < GridSize * GridSize; ++p_tile##I, p_tile##X = p_tile##I % GridSize, p_tile##Y = p_tile##I / GridSize, p_tile = grid.getOptional(p_tile##X, p_tile##Y))
