#include "Input.h"

#include "Game.h"
#include "GuiRendering.h"
#include "GuiRenderingState.h"
#include "SfmlGuiRendering.h"
#include "GuiRenderInfo.h"
#include "GridViz.h"
#include "Menu.h"
#include "Resources.h"
#include <algorithm>
#include <cmath>
#include <SFML/Window/Mouse.hpp>

static void updateCursorIcon()
{
	sf::Cursor cursor;
	sf::Image image;
	image.loadFromFile(Resources::getResourcePath("assets/main_character_right.png"));
	cursor.loadFromPixels(image.getPixelsPtr(), image.getSize(), sf::Vector2u(0, 0));
	g_window->setMouseCursor(cursor);
}

static void updateIncompingTileInput()
{
	Grid &grid = *g_grid;

	sf::Vector2f p = getMousePos();
	bool up = p.y < gridPos.y;
	bool down = p.y > gridPos.y + gridSize.y;
	bool left = p.x < gridPos.x;
	bool right = p.x > gridPos.x + gridSize.x;
	int directionsPressed = (up ? 1 : 0) + (down ? 1 : 0) + (left ? 1 : 0) + (right ? 1 : 0);
	if (directionsPressed == 1)
	{
		sf::Vector2f colOrRow;
		colOrRow.x = (p.x - gridPos.x) / tileSize.x;
		colOrRow.y = (p.y - gridPos.y) / tileSize.y;
		int col = int(colOrRow.x);
		int row = int(colOrRow.y);
		if (up)
		{
			for (int y = 1; y < GridSize; ++y)
				grid(col, y - 1) = grid(col, y);

			grid(col, GridSize - 1) = g_incomingTiles->up;
		}
		else if (down)
		{
			for (int y = 0; y + 1 < GridSize; ++y)
				grid(col, y + 1) = grid(col, y);

			grid(col, 0) = g_incomingTiles->down;
		}
		else if (left)
		{
			for (int x = 1; x + 1 < GridSize; ++x)
				grid(x - 1, row) = grid(x, row);

			grid(GridSize - 1, row) = g_incomingTiles->left;
		}
		else
		{
			for (int x = 0; x + 1 < GridSize; ++x)
				grid(x + 1, row) = grid(x, row);

			grid(0, row) = g_incomingTiles->right;
		}
	}
}

static void gridTick()
{
	Grid &grid = *g_grid;

	//// Clear
	//ForGrid(clearTile)
	//{
	//	Tile &tile = *clearTile;
	//	tile.willReceive = 0;
	//}

	// Growing
	ForGrid(growing)
	{
		Tile &tile = *growing;
		if (tile.type == TileType::Empty)
			continue;

		tile.willReceive+= 1;

		switch (tile.type)
		{
		case TileType::Empty:
			continue;
		case TileType::EasySeed:
			tile.willReceive += 1;
			break;
		case TileType::EasyFlower:
			tile.willReceive += 1;
			break;
		case TileType::HardSeed:
			tile.willReceive += 1;
			break;
		case TileType::HardFlower:
			tile.willReceive += 1;
			break;
		case TileType::Sprinkler:
		{
			ForGrid(neighborTile)
			{
				if ((growingY == neighborTileY + 1 || growingY == neighborTileY - 1) && (growingX == neighborTileX + 1 || growingX == neighborTileX - 1))
					neighborTile->willReceive += 3;
			}
		}
		break;
		case TileType::Sun:
		{
			ForGrid(neighborTile)
			{
				if ((growingY <= neighborTileY + 1 && growingY >= neighborTileY - 1) && (growingX <= neighborTileX + 1 || growingX >= neighborTileX - 1))
					neighborTile->willReceive += 1;
			}
		}
		break;
		case TileType::COUNT:
			assert(false);
			break;
		}
	}

	// Lifetime
	ForGrid(lifetime)
	{
		Tile &tile = *lifetime;
		if (tile.willReceive == 0)
			continue;

		g_score->risingNumbers.push_back({ tile.willReceive, getPos(lifetimeX, lifetimeY) });

		tile.lifeTime += tile.willReceive;
		tile.willReceive = 0;
	}


	// Flowers grow
	ForGrid(growTile)
	{
		Tile &tile = *growTile;
		switch (tile.type)
		{
		case TileType::Empty:
			continue;
		case TileType::EasySeed:
			if (tile.lifeTime > 3)
			{
				g_score += 2;
				tile.type = TileType::EasyFlower;
				tile.lifeTime = 0;
			}
			break;
		case TileType::EasyFlower:
			if (tile.lifeTime > 3)
			{
				g_score += 10;
				tile.type = TileType::Empty;
				tile.lifeTime = 0;
			}
			break;
		case TileType::HardSeed:
			if (tile.lifeTime > 10)
			{
				g_score += 4;
				tile.type = TileType::HardFlower;
				tile.lifeTime = 0;
			}
			break;
		case TileType::HardFlower:
			if (tile.lifeTime > 10)
			{
				g_score += 100;
				tile.type = TileType::Empty;
				tile.lifeTime = 0;
			}
			break;
		case TileType::Sprinkler:
			break;
		case TileType::Sun:
			break;
		case TileType::COUNT:
			assert(false);
			break;
		}
	}
}

void Input::update()
{
	updateCursorIcon();

	sf::Vector2f p = (getMousePos() - gridPos);
	p.x = p.x / tileSize.x;
	p.y = p.y / tileSize.y;
	
	int x = int(p.x);
	int y = int(p.y);

	Grid &grid = *g_grid;

	static bool wasPressed = false;
	bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool clicked = !wasPressed && isPressed;
	wasPressed = isPressed;
	if (clicked && grid.contains(x, y))
	{
		gridTick();

		grid(x, y) = g_incomingTiles->dragged;
		g_incomingTiles->dragged = IncomingTiles::randomTile();
	}
}

void Input::drawIncomingTileHover()
{
	sf::Vector2f p = (getMousePos() - gridPos);
	p.x = floorf(p.x / tileSize.x) * tileSize.x + gridPos.x;
	p.y = floorf(p.y / tileSize.y) * tileSize.y + gridPos.y;
	if (g_grid->contains(int(p.x), int(p.y)))
	{
		const sf::Texture &texture = Resources::getResources().tileTextures[g_incomingTiles->dragged.type];
		GuiRendering::image(&texture, p.x, p.y, tileSize.x, tileSize.y);
	}	
}
