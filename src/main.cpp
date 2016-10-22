#include <SFML/Graphics.hpp>
#include <iostream>

#include "common.hpp"
#include "state.h"
#include "render.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_WIDTH, GRID_HEIGHT * CELL_HEIGHT, 32), "Land Buster");
	
	state::Game& game = state::Game::getInstance();
	game.init(6);

	render::Renderer& renderer = render::Renderer::getInstance();
	renderer.init();

	sf::Clock framerateClock;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time framerateElapsed = framerateClock.getElapsedTime();
		if (framerateElapsed.asMilliseconds() > 30) {
			window.clear();
			renderer.render(window);
			window.display();

			framerateClock.restart();
		}
		
	}

	return 0;
}

