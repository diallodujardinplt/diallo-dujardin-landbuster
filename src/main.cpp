#include <SFML/Graphics.hpp>
#include <iostream>
#include <json/json.h>
#include <microhttpd.h>

#include "common.hpp"
#include "state.h"
#include "engine.h"
#include "render.h"
#include "ai.h"
#include "server.h"
#include "client.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc > 1) {
		// Server mode
		server::Server server;
		server.run();
	}
	else {
		client::Client client;
		client.run();
	}

	/*shared_ptr<state::Game> game=make_shared<state::Game>();

	game->init(2);
	game->generateMap();

	shared_ptr<engine::Engine> engine=make_shared<engine::Engine>(game);
	shared_ptr<render::Client> client = make_shared<render::Client>(game, engine);

	//engine->registerAIPlayer(1, make_shared<ai::MinMaxAI>(1));
	
	client->addAttributedPlayer(0);
	//engine->registerAIPlayer(1, make_shared<ai::HeuristicAI>(1));
	client->addAttributedPlayer(1);

	thread threadEngine(engineThreadFunction, engine);
	thread threadClient(clientThreadFunction, client);

	threadClient.join();
	threadEngine.join();*/

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

