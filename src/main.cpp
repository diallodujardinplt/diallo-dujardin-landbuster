#include <SFML/Graphics.hpp>
#include <iostream>
#include "state.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(800,600,32), "Land Buster");
	//std::shared_ptr<state::Game> game = state::Game::getInstance(6);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		//game->render(window);
		window.display();	
	}
	return 0;
}

