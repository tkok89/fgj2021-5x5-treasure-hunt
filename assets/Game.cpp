// RedLynx Gameplay Programmer Technical Assignment
// Version 1.04

#include "Game.h"

Game::Game(const sf::Texture& pawnTexture, const sf::Texture& blockTexture, const sf::Texture& pointTexture, const sf::Font& font)
{
	m_pawnSprite.setTexture(pawnTexture);
	m_blockSprite.setTexture(blockTexture);
	m_pointSprite.setTexture(pointTexture);

	m_jumpText.setFont(font);
	m_moveText.setFont(font);
}

void Game::update(sf::Time elapsedTime)
{
	// Put game update logic here

	m_jumpTimerSeconds -= elapsedTime.asSeconds();
}

void Game::draw(sf::RenderWindow& window)
{
	// Replace everything here with own drawing logic

	m_pawnSprite.setPosition(100.0f, 100.0f);
	window.draw(m_pawnSprite);

	m_blockSprite.setPosition(200.0f, 300.0f);
	window.draw(m_blockSprite);
	m_blockSprite.setPosition(120.0f, 340.0f);
	window.draw(m_blockSprite);

	m_pointSprite.setPosition(500.0f, 200.0f);
	window.draw(m_pointSprite);
	m_pointSprite.setPosition(580.0f, 200.0f);
	window.draw(m_pointSprite);

	if (m_jumpTimerSeconds > 0.0f)
	{
		m_jumpText.setString("jump!");
		m_jumpText.setPosition(300.0f, 100.0f + m_jumpTimerSeconds * 100.0f);
		window.draw(m_jumpText);
	}

	m_moveText.setPosition(400.0f, 400.0f);
	window.draw(m_moveText);
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
