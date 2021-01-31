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
std::string g_currentTextInput;
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
        activePlayer->velocity = sf::Vector2f(p.inputVelocityX, p.inputVelocityY);
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
                    otherP.inputVelocityX = player.velocity.x;
                    otherP.inputVelocityY = player.velocity.y;
                    otherP.score = player.score;
                }
            }
            state.unread = false;
        }
        
        
    }
    
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

void Game::textInput(sf::Uint32 unicode)
{
    if (unicode == 8) // backspace
    {
        while (g_currentTextInput.length() > 0 && (g_currentTextInput.back() & 0b11000000) == 0b10000000)
        {
            g_currentTextInput.resize(g_currentTextInput.length() - 1u);
        }

        if (g_currentTextInput.length() > 0)
            g_currentTextInput.resize(g_currentTextInput.length() - 1u);

        return;
    }

    sf::String string(unicode);
    g_currentTextInput += string;
}

static  std::string stateString;
void printfHorrorString()
{
  GameNetState& netState = GameClient::getClient().gameNetState;
  stateString.clear();
  stateString.append(std::string("playerAmount: "));
  stateString.append(std::to_string(netState.players.size()));

  if (GameClient::imHost)
  {
      stateString.append(std::string("\nim host, my client amount: "));
      stateString.append(std::to_string(GameClient::connectedClientAmount));
  }

  //  stateString.append(std::to_string());
  if(GameClient::connectedToHost)
   stateString.append(std::string("\nim client."));

   for (const NetPlayer& playah : netState.players)
   {
       stateString.append(std::string("\n id #"));
       stateString.append(std::to_string(playah.id));
       if (playah.id == GameClient::myPlayerId)
           stateString.append(std::string(" (me!)"));
       stateString.append(std::string("\n\tpos:\t("));
       stateString.append(std::to_string(playah.position.x));
       stateString.append(std::string(", "));
       stateString.append(std::to_string(playah.position.y));
       stateString.append(std::string(")\n\tvel: \t("));
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

    if (showDebugText)
        GuiRendering::text(g_currentTextInput.c_str(), 0.02f, 0, 0);

	SfmlGuiRendering::flush(window);
}

void Game::gui(sf::RenderWindow& window)
{
    const float x = -0.5f * g_resolution.x / g_resolution.y + 0.01f;
    const float y = -0.5f + 0.01f;
    const float h = 0.04f;
    const float h2 = 0.035f;

    if (GuiRendering::button("Music", sf::Vector2f(x, y + h * 0), sf::Vector2f(0.1f, h2)))
    {
        static bool musicToggled = false;
        musicToggled = !musicToggled;
        if (musicToggled)
            Mankka::getMankka().playMusic(MusicEnvName::ingame);
        else
            Mankka::getMankka().playMusic(MusicEnvName::silence);
    }

    static bool joining = false;
    if (!GameClient::imHost && !GameClient::connectedToHost && !joining)
    {
        if (GuiRendering::button("Host", sf::Vector2f(x, y + h * 2), sf::Vector2f(0.1f, h2)))
            GameClient::getClient().host();
    }

    if (joining)
    {
        GuiRendering::text("Connecting...", 0.028f, sf::Vector2f(x, y + h * 1));
    }
    else if (!GameClient::imHost && !GameClient::connectedToHost)
    {
        static bool joinToggled = false;
        if (GuiRendering::button("Join", sf::Vector2f(x, y + h * 1), sf::Vector2f(0.1f, h2)))
        {
            joinToggled = !joinToggled;
            g_currentTextInput.clear();
        }

        if (joinToggled)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                joining = true;
                while (!g_currentTextInput.empty() && ((g_currentTextInput.back() - '0') > 9 || (g_currentTextInput.back() - '0') < 0))
                    g_currentTextInput.resize(g_currentTextInput.length() - 1u);

                GameClient::getClient().join(g_currentTextInput);
            }

            static std::string joinText;
            joinText = "Type host IP: " + g_currentTextInput + "|";
            GuiRendering::text(joinText.c_str(), 0.028f, sf::Vector2f(x + 0.1f, y + h * 1));
        }
    }
}
