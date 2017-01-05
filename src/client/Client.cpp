#include "Client.hpp"
#include "../render/Client.hpp"
#include "../engine/Engine.hpp"

using namespace std;

namespace client {

	Client::Client() {
		nextCommandIndex = 0;

		game = make_shared<state::Game>();
		game->init(2);
		game->generateMap();

		engine = make_shared<engine::Engine>(game);
		gui = make_shared<render::Client>(game, engine);

		gui->addAttributedPlayer(0);
		gui->addAttributedPlayer(1);
	}

	void Client::run() {
		thread threadEngine(engine::engineThreadFunction, engine);
		thread threadClient(render::clientThreadFunction, gui);

		threadClient.join();
		threadEngine.join();
	}

	int Client::requestGetStatus() {
		
	}

	int Client::requestCreateGame(unsigned int numPlayers) {

	}

	int Client::requestPushCommand(shared_ptr<engine::Command> command) {

	}

	int Client::requestGetCommands() {

	}

}
