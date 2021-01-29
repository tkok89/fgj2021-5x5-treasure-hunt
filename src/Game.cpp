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

Game::Game()
{
	m_guiText.setFont(Resources::getResources().font);
}

void Game::update(sf::Time elapsedTime)
{
}

void Game::draw(sf::RenderWindow& window)
{
	GridViz::render(grid, sf::Vector2f(-0.5f, -0.5f), sf::Vector2f(1, 1));

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
