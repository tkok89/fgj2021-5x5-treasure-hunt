// RedLynx Gameplay Programmer Technical Assignment
// Version 1.04

#include "Game.h"

#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
	sf::RenderWindow window{ sf::VideoMode(1280, 720), Game::getName() };
	window.setKeyRepeatEnabled(false);

	sf::Texture pawnTexture;
	if (!pawnTexture.loadFromFile("pawn.png"))
	{
		std::cerr << "Error loading pawn.png" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile("block.png"))
	{
		std::cerr << "Error loading block.png" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Texture pointTexture;
	if (!pointTexture.loadFromFile("point.png"))
	{
		std::cerr << "Error loading point.png" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Font font;
	if (!font.loadFromFile("Vera.ttf"))
	{
		std::cerr << "Error loading Vera.ttf" << std::endl;
		return EXIT_FAILURE;
	}

	Game game(pawnTexture, blockTexture, pointTexture, font);
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				game.onJumpAction();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		{ // Handle movement input
			int xInput = 0, yInput = 0;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				--xInput;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				++xInput;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				--yInput;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				++yInput;

			game.setMovement(sf::Vector2i(xInput, yInput));
		}

		const sf::Time elapsedTime = clock.restart();
		game.update(elapsedTime);

		const sf::Color backgroundColor{ 75, 88, 168 };
		window.clear(backgroundColor);

		game.draw(window);
		window.display();
	}

	return EXIT_SUCCESS;
}