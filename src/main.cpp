#include <SFML/Graphics.hpp>
#include <iostream>

#include "common.hpp"
#include "state.h"
#include "engine.h"
#include "render.h"

using namespace std;

int main() {
	
	render::Client& client = render::Client::getInstance();

	shared_ptr<state::Game> game=make_shared<state::Game>();

	game->init(6);
	game->generateMap();

	shared_ptr<engine::Engine> engine=make_shared<engine::Engine>(game);

	client.addAttributedPlayer(0);
	client.run(*game, engine);

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

