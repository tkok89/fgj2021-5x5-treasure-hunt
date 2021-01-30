#include "Game.h"

#include "GuiRendering.h"
#include "GuiRenderingState.h"
#include "SfmlGuiRendering.h"
#include "Input.h"
#include "GuiRenderInfo.h"
#include "GridViz.h"
#include "Menu.h"
#include "Resources.h"
#include <algorithm>
#include <cmath>
#include <SFML/Window/Mouse.hpp>

Grid *g_grid = nullptr;
IncomingTiles *g_incomingTiles = nullptr;
Score *g_score = nullptr;

sf::Vector2f Game::resolution{ 1280,720 };

Game::Game()
{
	m_guiText.setFont(Resources::getResources().font);
	g_grid = &grid;
	g_incomingTiles = &incomingTiles;
	g_score = &score;
}

void Game::update(sf::Time elapsedTime)
{
	Input::update();

	SfmlGuiRendering::setResolution(getResolution().x, getResolution().y);
}

void Game::draw(sf::RenderWindow& window)
{
	GuiRendering::image(g_placeholder, getMousePos().x, getMousePos().y, 0.1f, 0.1f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		slidingTiles.start(sf::Vector2i(1u, 42u), tileSize.x);
	}
	if (slidingTiles.active)
		slidingTiles.updateSlide();

	GridViz::render(grid, gridPos, gridSize, slidingTiles.getMoveRow(), slidingTiles.currentPos);

	Input::drawIncomingTileHover();

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

void Game::SlidingTiles::updateSlide()
{
	const sf::Time elapsedTime = tileClock.getElapsedTime();
	if (elapsedTime > duration)
	{
		active = false;
		moveThisRowToDirection.x = 42;
		moveThisRowToDirection.y = 42;
		currentPos = 0.0f;
		return;
	}
	const float phase = elapsedTime / duration;
	const float sqt = phase * phase;
	currentPos = sqt / (2.0f * (sqt - phase) + 1.0f) * tileLength;
}

void Game::SlidingTiles::start(sf::Vector2i moveThisRowToDirectionParam, float tileLengthParam)
{
	active = true;
	tileClock.restart();
	moveThisRowToDirection = moveThisRowToDirectionParam;
	tileLength = tileLengthParam;
}
