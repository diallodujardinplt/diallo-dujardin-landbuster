#include <SFML/Graphics.hpp>
#include <iostream>

#include "common.hpp"
#include "state.h"
#include "engine.h"
#include "render.h"

int main() {
	
	render::Client& client = render::Client::getInstance();

	state::Game& game = state::Game::getInstance();
	engine::Engine& engine = engine::Engine::getInstance();

	game.init(6);
	game.generateMap();

	client.setPlayer(0);
	client.run();

	return 0;
}

