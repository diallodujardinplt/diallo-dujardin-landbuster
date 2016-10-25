#include <SFML/Graphics.hpp>
#include <iostream>

#include "common.hpp"
#include "state.h"
#include "render.h"

int main() {
	
	render::Client& client = render::Client::getInstance();

	state::Game& game = state::Game::getInstance();
	game.init(6);
	game.generateMap();

	client.setPlayer(0);
	client.setSelectedInfoLand(game.getLands()[0]);
	client.setSelectedLand(game.getLands()[1]);

	client.run();

	return 0;
}

