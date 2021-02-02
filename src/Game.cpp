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

std::string g_currentTextInput;
sf::Vector2f g_resolution{ 1280,720 };
sf::Clock syncClock;
sf::Clock timeFromStart;
sf::Time syncCycle = sf::seconds(0.1f);
bool Game::showDebugText = false;

Game::Game()
{
	m_guiText.setFont(Resources::getResources().font);
    initializePlayers(Map::getShopPos().x, Map::getShopPos().y);
    timeFromStart.restart();
    Mankka::getMankka().playMusic(MusicEnvName::ingame);
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
    // when moved
    if( magnitudeVector2( getOwnPlayer().realInputVelocity) > 0.3f){
        lastMove = timeFromStart.getElapsedTime().asSeconds();
    }
    if( timeFromStart.getElapsedTime().asSeconds() - lastMove > 1.5f){
        float t = timeFromStart.getElapsedTime().asSeconds() - lastMove - 1.5f;
        t /= zoomDuration;
        t = clamp01(t);
        g_screenHeightInWorldCoordinates = cameraMinSize + t  * (maxSize - cameraMinSize);
        debugText += " zoom " + std::to_string(g_screenHeightInWorldCoordinates);
    }
    else{
        g_screenHeightInWorldCoordinates = 0.95f * g_screenHeightInWorldCoordinates + 0.05f *  cameraMinSize;
    }

    // Network
    // own player
    NetPlayer* activePlayer = GameClient::getClient().getMyPlayer();
    if(activePlayer != nullptr){
        debugText += " Has active " + std::to_string(activePlayer->id);
        Player p = setActivePlayerIndex(activePlayer->id);
        activePlayer->position = sf::Vector2f(p.posX, p.posY);
        activePlayer->velocity = p.realInputVelocity;
        activePlayer->score = p.score;
        activePlayer->randomId = p.frameId;
    
        GameNetState* state = &GameClient::gameNetState;
        if(state->unread){
            // update   
            for (NetPlayer& player : state->players)
            {
                if(player.id != activePlayer->id){
                    Player& otherP = getPlayer(player.id);
                    if(otherP.frameId != player.randomId){
                        otherP.activePlayer = true;
                        otherP.posX = player.position.x;
                        otherP.posY = player.position.y;
                        otherP.inputVelocityX = player.velocity.x;
                        otherP.inputVelocityY = player.velocity.y;
                        otherP.score = player.score;
                        otherP.frameId = player.randomId;
                    }
                }
            }
            state->unread = false;
        }
        else{
            debugText += " skip net ";
        }
        
        // update map
        for (NetTreasure netTreasure : state->treasures)
        {
            if (netTreasure.itemState != ItemState::OnWorld)
            {
                for (unsigned i = 0; i < map.treasures.size(); i++)
                {
                    if (map.treasures[i].id == netTreasure.id)
                    {
                        map.treasures.erase(map.treasures.begin() + i);
                        // gone, deleted, nada
                        break;
                    }
                }
                continue;
            }

            bool found = false;
            for (Treasure& treasure : map.treasures)
            {
                if (treasure.id == netTreasure.id)
                {
                    found = true;
                }
            }

            if (found == false)
            {
                Treasure newlySyncedTreasure;
                newlySyncedTreasure.id = netTreasure.id;
                newlySyncedTreasure.item = netTreasure.itemType;
                newlySyncedTreasure.pos = netTreasure.position;
                map.treasures.push_back(newlySyncedTreasure);
            }
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

static void encodeAnsi(sf::Uint32 codepoint, std::back_insert_iterator<std::string> output, char replacement = '?', const std::locale &locale = std::locale())
{
    // NOTE: Copied from SFML/System/Utf.inl Utf<32>::encodeAnsi to avoid template errors
    
    // On Windows, gcc's standard library (glibc++) has almost
    // no support for Unicode stuff. As a consequence, in this
    // context we can only use the default locale and ignore
    // the one passed as parameter.

#if defined(SFML_SYSTEM_WINDOWS) &&                       /* if Windows ... */                          \
    (defined(__GLIBCPP__) || defined (__GLIBCXX__)) &&     /* ... and standard library is glibc++ ... */ \
    !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) /* ... and STLPort is not used on top of it */

    (void)locale; // to avoid warnings

    char character = 0;
    if (wctomb(&character, static_cast<wchar_t>(codepoint)) >= 0)
        *output++ = character;
    else if (replacement)
        *output++ = replacement;

    return output;

#else

    // Get the facet of the locale which deals with character conversion
    const std::ctype<wchar_t> &facet = std::use_facet< std::ctype<wchar_t> >(locale);

    // Use the facet to convert each character of the input string
    *output++ = facet.narrow(static_cast<wchar_t>(codepoint), replacement);
#endif
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

    encodeAnsi(unicode, std::back_inserter(g_currentTextInput), '?');
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

    if (GuiRendering::button("Music", sf::Vector2f(x, y + h * 0), sf::Vector2f(0.2f, h2)))
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
        if (GuiRendering::button("Host", sf::Vector2f(x, y + h * 2), sf::Vector2f(0.2f, h2)))
            GameClient::getClient().host();
        if (GuiRendering::button("Single Player", sf::Vector2f(x, y + h * 4), sf::Vector2f(0.2f, h2)))
            GameClient::getClient().host();
    }
    if (GuiRendering::button("Lights", sf::Vector2f(x, y + h * 6), sf::Vector2f(0.2f, h2)))
        g_map->lightsOn = !g_map->lightsOn;

    if (joining)
    {
        GuiRendering::text("Connecting...", 0.028f, sf::Vector2f(x, y + h * 1));
    }
    else if (!GameClient::imHost && !GameClient::connectedToHost)
    {
        static bool joinToggled = false;
        if (GuiRendering::button("Join", sf::Vector2f(x, y + h * 1), sf::Vector2f(0.2f, h2)))
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
