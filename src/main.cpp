#include <SFML/Graphics.hpp>
#include <iostream>

#include "common.hpp"
#include "state.h"
#include "engine.h"
#include "render.h"

int main() {
	
	render::Client& client = render::Client::getInstance();

	state::Game& game = state::Game::getInstance();

	game.init(6);
	game.generateMap();

	client.setPlayer(0);
	client.run();

/*	//sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_WIDTH + 200, GRID_HEIGHT * CELL_HEIGHT, 32), "Land Buster");
*	sf::Clock framerateClock, clickClock;
*	while (window.isOpen()) {
*		DumbAI.run(3);
*		window.clear();
*
*		renderer.render(window);
*
*		window.display();
*
*		framerateClock.restart();
*	} 
*/
	
	return 0;
}

