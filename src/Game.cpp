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

Game::Game()
{
	m_guiText.setFont(Resources::getResources().font);
}

void Game::update(sf::Time elapsedTime)
{
	int xInput = 0;
	int yInput = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		--xInput;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		++xInput;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		--yInput;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		++yInput;

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
		}
		else if (down)
		{
			for (int y = 0; y + 1 < GridSize; ++y)
				grid(col, y + 1) = grid(col, y);
		}
		else if (left)
		{
			for (int x = 1; x + 1 < GridSize; ++x)
				grid(x - 1, row) = grid(x, row);
		}
		else
		{
			for (int x = 0; x + 1 < GridSize; ++x)
				grid(x + 1, row) = grid(x, row);
		}
	}
}

void Game::draw(sf::RenderWindow& window)
{
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
		float x = gridPos.x + floorf(colOrRow.x) * tileSize.x;
		float y = gridPos.y + floorf(colOrRow.y) * tileSize.y;

		if (up)
		{
			y = gridPos.y - tileSize.y;
		}
		else if (down)
		{
			y = gridPos.y + gridSize.y;
		}
		else if (left)
		{
			x = gridPos.x - tileSize.x;
		}
		else
		{
			x = gridPos.x + gridSize.x;
		}

		GuiRendering::image(&Resources::getResources().tileTextures[0], x, y, tileSize.x, tileSize.y);
	}

	GridViz::render(grid, gridPos, gridSize);

	void *menuStatePtr;
	Menu::render(menuStatePtr);

	SfmlGuiRendering::flush(window);

	gui(window);
}

void Game::setMovement(sf::Vector2i direction)
{
	const char *text;
	text = (direction.x < 0 ? "left " : (direction.x > 0 ? "right " : ""));
	text = (direction.y < 0 ? "up" : (direction.y > 0 ? "down" : ""));

	GuiRendering::text(&m_guiText, text, 0.02f, -0.3f, -0.45f);
}

void Game::gui(sf::RenderWindow& window)
{
	//static sf::Clock clock;
	//float t = clock.getElapsedTime().asSeconds();

}
