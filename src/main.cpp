#include <SFML/Graphics.hpp>
#include <iostream>

#include "common.hpp"
#include "state.h"
#include "render.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(800,600,32), "Land Buster");
	
	state::Game& game = state::Game::getInstance();
	game.init(6);

	render::Renderer& renderer = render::Renderer::getInstance();
	renderer.init();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		renderer.render(window);
		window.display();	
	}

	return 0;
}

