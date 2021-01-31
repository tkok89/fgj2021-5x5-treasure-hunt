#include "Game.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include "workingdir.h"

sf::RenderWindow *g_window = nullptr;

int main()
{
    std::cout << "Working directory: " << getWorkingDir() << std::endl;

	sf::RenderWindow window{ sf::VideoMode(int(Game::getResolution().x), int(Game::getResolution().y)), Game::getName() };
	g_window = &window;
    std::cout << "Using OpenGL " << g_window->getSettings().majorVersion << "." << g_window->getSettings().minorVersion << std::endl;
    
    
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);

	Game game;
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
                 Game::showDebugText = !Game::showDebugText;

			if (event.type == sf::Event::EventType::Resized)
			{
				float x = (float)event.size.width;
				float y = (float)event.size.height;
				Game::setResolution(x, y);
				window.setView(sf::View(sf::FloatRect(0, 0, x, y)));
			}
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
