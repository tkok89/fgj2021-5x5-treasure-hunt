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
#include "Players.hpp"
#include "Camera.h"

sf::Vector2f g_resolution{ 1280,720 };

Game::Game()
{
	m_guiText.setFont(Resources::getResources().font);
    initializePlayers();
}

void Game::update(sf::Time elapsedTime)
{
	SfmlGuiRendering::setResolution(getResolution().x, getResolution().y);
	SfmlGuiRendering::setDefaultFont(Resources::getResources().font);
	
	Mankka::getMankka().play(MusicEnvName::ingame);
    updatePlayers(elapsedTime.asSeconds());
    
    // Update camera
    Camera::setCameraPos(lerpVector2f(Camera::getCameraPos(), sf::Vector2f(getPlayer(0).posX, getPlayer(0).posY), clamp01(elapsedTime.asSeconds() * cameraLerpPerSecond)));
    debugText = "Lerp " + std::to_string(clamp01(elapsedTime.asSeconds() * cameraLerpPerSecond));
}

void Game::draw(sf::RenderWindow& window)
{
	GuiRendering::startThread();
	//GuiRendering::image(g_placeholder, getMousePos().x, getMousePos().y, 0.1f, 0.1f);

	map.draw();

	gui(window);

	GuiRendering::endThread();

    drawPlayers(showDebugText);
    if(showDebugText)
        GuiRendering::text(debugText, 0.02f, -0.5, -0.5);
	SfmlGuiRendering::flush(window);
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
