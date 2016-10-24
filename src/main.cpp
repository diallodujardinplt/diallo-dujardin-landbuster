#include <SFML/Graphics.hpp>
#include <iostream>

#include "common.hpp"
#include "state.h"
#include "render.h"

int main() {
	
	render::Client client;

	state::Game& game = state::Game::getInstance();
	game.init(6);
	game.generateMap();

	client.run();

	return 0;
}

