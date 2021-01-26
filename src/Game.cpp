#include "Game.h"

#include "GuiRendering.h"
#include "GuiRenderingState.h"
#include "SfmlGuiRendering.h"
#include "GuiRenderInfo.h"
#include "Menu.h"
#include <algorithm>
#include <cmath>

Game::Game(const sf::Texture& pawnTexture, const sf::Texture& blockTexture, const sf::Texture& pointTexture, const sf::Font& font)
{
	m_pawnSprite.setTexture(pawnTexture);
	m_blockSprite.setTexture(blockTexture);
	m_pointSprite.setTexture(pointTexture);

	m_jumpText.setFont(font);
	m_moveText.setFont(font);
	m_guiText.setFont(font);
}

void Game::update(sf::Time elapsedTime)
{
	// Put game update logic here

	m_jumpTimerSeconds -= elapsedTime.asSeconds();
}

void Game::draw(sf::RenderWindow& window)
{
	void *menuStatePtr;
	Menu::render(menuStatePtr);

	SfmlGuiRendering::flush(window);

	// Replace everything here with own drawing logic

	//m_pawnSprite.setPosition(100.0f, 100.0f);
	//window.draw(m_pawnSprite);

	m_blockSprite.setPosition(200.0f, 300.0f);
	window.draw(m_blockSprite);
	m_blockSprite.setPosition(120.0f, 340.0f);
	window.draw(m_blockSprite);

	m_pointSprite.setPosition(500.0f, 200.0f);
	window.draw(m_pointSprite);
	m_pointSprite.setPosition(580.0f, 200.0f);
	window.draw(m_pointSprite);

	gui(window);
}

void Game::onJumpAction()
{
	m_jumpTimerSeconds = 1.0f;
}

void Game::setMovement(sf::Vector2i direction)
{
	sf::String text;
	text = (direction.x < 0 ? "left " : (direction.x > 0 ? "right " : ""));
	text += (direction.y < 0 ? "up" : (direction.y > 0 ? "down" : ""));
	m_moveText.setString(text);
}

void Game::gui(sf::RenderWindow& window)
{
	static sf::Clock clock;
	float t = clock.getElapsedTime().asSeconds();

	GuiRendering::startThread();
	{
		GuiRenderInfo i;
		i.type = GuiRenderInfoType::Image;
		i.image = m_pawnSprite.getTexture();
		i.x = -0.2f;
		i.y = -0.2f;
		i.w = 0.4f;
		i.h = 0.4f;
		i.uvW = 1;
		i.uvH = 1;
		i.color.r = 0.7f;
		i.color.g = 0.1f;
		i.color.b = 0.9f;
		i.color.a = 1.0f;

		auto ss = [t](float offset)
		{
			return std::sin(t + offset) * 0.25f + 0.4f;
		};

		GuiRendering::pushClipRect(ss(0) - 0.5f, ss(1) - 0.5f, ss(2), ss(2.5f));
		GuiRendering::render(i);
		GuiRendering::popClipRect();
	}
	{
		GuiRenderInfo i;
		i.type = GuiRenderInfoType::Text;
		i.strPtr = "Hello!";
		i.strLen = 6;
		i.x = -0.4f;
		i.y = -0.4f;
		i.w = 0.7f;
		i.h = 0.7f;
		i.font = &m_guiText;
		i.fontHeight = 0.1f;
		i.color.r = 0.7f;
		i.color.g = 0.1f;
		i.color.b = 0.9f;
		i.color.a = 1.0f;
		i.outline = 2;
		i.outlineColor.r = 1.0f;
		i.outlineColor.g = 1.0f;
		i.outlineColor.b = 1.0f;
		i.outlineColor.a = 0.6f;
		GuiRendering::render(i);

		i.strPtr = nullptr;
		i.font = &m_moveText;
		i.fontHeight = 0.03f;
		i.color = { 1 };
		i.outlineColor = { 0,0,0,1 };
		i.x = -0.2f;
		i.y = -0.2f;
		GuiRendering::render(i);
	}
	GuiRendering::endThread();
}
