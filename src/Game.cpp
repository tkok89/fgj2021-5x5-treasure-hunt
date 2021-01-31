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
    Camera::setCameraPos(lerpVector2f(Camera::getCameraPos(), sf::Vector2f(getOwnPlayer().posX, getOwnPlayer().posY), clamp01(elapsedTime.asSeconds() * cameraLerpPerSecond)));
    debugText = "Lerp " + std::to_string(clamp01(elapsedTime.asSeconds() * cameraLerpPerSecond));

    // Network
    // own player
    NetPlayer* activePlayer = GameClient::getClient().getMyPlayer();
    if(activePlayer != nullptr){
        debugText += " Has active " + std::to_string(activePlayer->id);
        Player p = setActivePlayerIndex(activePlayer->id);
        activePlayer->position = sf::Vector2f(p.posX, p.posY);
        activePlayer->velocity = sf::Vector2f(p.velocityX, p.velocityY);
        activePlayer->score = p.score;
    
        GameNetState state = GameClient::gameNetState;
        if(state.unread){
            // update
            for (NetPlayer& player : state.players)
            {
                if(player.id != activePlayer->id){
                    Player &otherP = getPlayer(player.id);
                    otherP.activePlayer = true;
                    otherP.posX = player.position.x;
                    otherP.posY = player.position.y;
                    otherP.velocityX = player.velocity.x;
                    otherP.velocityY = player.velocity.y;
                    otherP.score = player.score;
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

static  std::string stateString;
void printfHorrorString()
{
  GameNetState& netState = GameClient::getClient().gameNetState;
  stateString.clear();
  stateString.append(std::string("playerAmount: "));
  stateString.append(std::to_string(netState.players.size()));

  stateString.append(std::string("\nim host: "));
  stateString.append(std::to_string(GameClient::imHost));
  if (GameClient::imHost)
  {
      stateString.append(std::string("muh clients: "));
      stateString.append(std::to_string(GameClient::connectedClientAmount));
  }

  //  stateString.append(std::to_string());
   stateString.append(std::string("\nim client: "));
   stateString.append(std::to_string(GameClient::connectedToHost));

   for (const NetPlayer& playah : netState.players)
   {
       stateString.append(std::string("\n id #"));
       stateString.append(std::to_string(playah.id));
       if (playah.id == GameClient::myPlayerId)
           stateString.append(std::string(" (me!)"));
       stateString.append(std::string("\n\tpos: \t("));
       stateString.append(std::to_string(playah.position.x));
       stateString.append(std::string(", "));
       stateString.append(std::to_string(playah.position.y));
       stateString.append(std::string("\n\tvel: \t("));
       stateString.append(std::to_string(playah.velocity.x));
       stateString.append(std::string(", "));
       stateString.append(std::to_string(playah.velocity.y));
       stateString.append(std::string(")\n "));
   }
   GuiRendering::text(stateString, 0.02f, 0.4, -0.3);
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

    if (showDebugText)
    {
        printfHorrorString();
    }

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
