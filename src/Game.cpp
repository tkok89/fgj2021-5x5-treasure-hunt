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
#include "GameClient.h"
#include "Map.h"

sf::Vector2f g_resolution{ 1280,720 };
sf::Clock syncClock;
sf::Time syncCycle = sf::seconds(0.1f);
bool Game::showDebugText = false;
Game::Game()
{
	m_guiText.setFont(Resources::getResources().font);
    initializePlayers(Map::getShopPos().x, Map::getShopPos().y);
}

void Game::update(sf::Time elapsedTime)
{
	SfmlGuiRendering::setResolution(getResolution().x, getResolution().y);
	SfmlGuiRendering::setDefaultFont(Resources::getResources().font);
	
    updatePlayers(elapsedTime.asSeconds());
	GameClient::getClient().update();

    // Update camera
    Camera::setCameraPos(lerpVector2f(Camera::getCameraPos(), sf::Vector2f(getPlayer(0).posX, getPlayer(0).posY), clamp01(elapsedTime.asSeconds() * cameraLerpPerSecond)));
    debugText = "Lerp " + std::to_string(clamp01(elapsedTime.asSeconds() * cameraLerpPerSecond));

    // Network
    // own player
    NetPlayer* activePlayer = GameClient::getClient().getMyPlayer();
    if(activePlayer != nullptr){
        Player p = setActivePlayerIndex(activePlayer->id);
        activePlayer->position = sf::Vector2f(p.posX, p.posX);
        activePlayer->velocity = sf::Vector2f(p.velocityX, p.velocityY);§
    
        GameNetState state = GameClient::gameNetState;
        if(state.unread){
            // update
            for (NetPlayer& player : state.players)
            {
                if(player.id != activePlayer->id){
                    Player otherP = getPlayer(player.id);
                    otherP.posX = player.position.x;
                    otherP.posY = player.position.y;
                    otherP.velocityX = player.velocity.x;
                    otherP.velocityY = player.velocity.y;
                }
            }
            state.unread = false;
        }
        
        
    }
    
    // remove sate
    
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		GameClient::getClient().host();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		GameClient::getClient().join();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        Mankka::getMankka().play(MusicEnvName::ingame);
    
	if (syncClock.getElapsedTime() > syncCycle)
	{
		syncClock.restart();
		if (GameClient::connectedToHost)
			GameClient::getClient().sendFrequentDataToHost();

		if (GameClient::connectedClientAmount != 0u)
		{
			GameClient::getClient().sendGameState(GameClient::gameNetState);
		}
	}
}

void Game::draw(sf::RenderWindow& window)
{
	GuiRendering::startThread();

	map.draw();

	gui(window);

	GuiRendering::endThread();

    drawPlayers(showDebugText);
    if(showDebugText)
        GuiRendering::text(debugText, 0.02f, -0.5, -0.5);

	if (GameClient::connectedClientAmount != 0 || GameClient::connectedToHost)
	{
		GameNetState netState = GameClient::gameNetState;
		for (NetPlayer playah : netState.players)
		{
			GuiRendering::text("_o_", 0.02f, playah.position.x, playah.position.y);
		}
	}

	SfmlGuiRendering::flush(window);
}

void Game::gui(sf::RenderWindow& window)
{

}
