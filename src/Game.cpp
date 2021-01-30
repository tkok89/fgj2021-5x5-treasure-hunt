#include "Game.h"

#include "GuiRendering.h"
#include "GuiRenderingState.h"
#include "SfmlGuiRendering.h"
#include "GuiRenderInfo.h"
#include "Mankka.h"
#include "Resources.h"
#include <algorithm>
#include <cmath>
#include <SFML/Window/Mouse.hpp>

sf::Vector2f Game::resolution{ 1280,720 };

Game::Game()
{
	m_guiText.setFont(Resources::getResources().font);
}

void Game::update(sf::Time elapsedTime)
{
	SfmlGuiRendering::setResolution(getResolution().x, getResolution().y);
	SfmlGuiRendering::setDefaultFont(Resources::getResources().font);
	
	Mankka::getMankka().play(MusicEnvName::ingame);
}

void Game::draw(sf::RenderWindow& window)
{
	GuiRendering::image(g_placeholder, getMousePos().x, getMousePos().y, 0.1f, 0.1f);
	GuiRendering::text("lol", 0.02f, 0, 0);

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

}
