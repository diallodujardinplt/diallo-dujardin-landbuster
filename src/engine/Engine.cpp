#include "Engine.hpp"

using namespace std;

namespace engine {
	
	Engine::Engine(shared_ptr<state::Game> game) {
		this->game = game;
	}

	void Engine::pushCommand(shared_ptr<Command> command) {
		commandQueue.push(command);
	}

	void Engine::flushCommands() {
		while(!commandQueue.empty()) {
			shared_ptr<Command> cmd = commandQueue.front();
			commandQueue.pop();
			if(isAllowed(cmd))
				execute(cmd);
		}
		if(aiPlayers.count(this->game->getCurrentPlayer())) {
			execute(aiPlayers[this->game->getCurrentPlayer()]->run(this->game));
		}
	}

	bool Engine::isAllowed(shared_ptr<Command> command) {

		state::Game& game = *(this->game);

		if(command->getPlayerId() != (int) game.getCurrentPlayer() || !game.getPlayers()[command->getPlayerId()]->isAlive())
			return false;

		if(command->getType() == COMMAND_SKIP_ROUND || command->getType() == COMMAND_ABANDON || command->getType() == COMMAND_USE_ITEM) {
			return (static_pointer_cast<ActionCommand>(command))->isAllowed(game);
		}
		else if(command->getType() == COMMAND_CHOOSE_HEADQUARTERS || command->getType() == COMMAND_CHOOSE_REINFORCEMENT || command->getType() == COMMAND_BUILD_PORT) {
			return (static_pointer_cast<ChoiceCommand>(command))->isAllowed(game);
		}
		else if(command->getType() == COMMAND_ATTACK) {
			return (static_pointer_cast<AttackCommand>(command))->isAllowed(game);
		}
		else if(command->getType() == COMMAND_MOVE) {
			return (static_pointer_cast<MoveCommand>(command))->isAllowed(game);
		}
	}

	void Engine::execute(shared_ptr<Command> command) {
		if(!isAllowed(command)) return;

		//cout << command->toString() << " [EXECUTED]" << endl;

		state::Game& game = *(this->game);

		if(command->getType() == COMMAND_SKIP_ROUND || command->getType() == COMMAND_ABANDON || command->getType() == COMMAND_USE_ITEM) {
			return (static_pointer_cast<ActionCommand>(command))->execute(game);
		}
		else if(command->getType() == COMMAND_CHOOSE_HEADQUARTERS || command->getType() == COMMAND_CHOOSE_REINFORCEMENT || command->getType() == COMMAND_BUILD_PORT) {
			return (static_pointer_cast<ChoiceCommand>(command))->execute(game);
		}
		else if(command->getType() == COMMAND_ATTACK) {
			return (static_pointer_cast<AttackCommand>(command))->execute(game);
		}
		else if(command->getType() == COMMAND_MOVE) {
			return (static_pointer_cast<MoveCommand>(command))->execute(game);
		}
	}

	void Engine::registerAIPlayer(unsigned int playerId, shared_ptr<ai::AI> ai) {
		aiPlayers[playerId] = ai;
	}

}
