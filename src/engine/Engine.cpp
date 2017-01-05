#include "Engine.hpp"

using namespace std;

namespace engine {
	
	Engine::Engine(shared_ptr<state::Game> game) {
		this->game = game;
		commandQuit = false;
	}

	Engine::Engine(Engine& engine) {
		commandQueue = engine.commandQueue;
		game = engine.game;
		aiPlayers = engine.aiPlayers;
		commandQuit = engine.commandQuit;
	}

	void Engine::pushCommand(shared_ptr<Command> command) {
		commandQueue.push(command);
	}

	void Engine::flushCommands() {
		while(!commandQueue.empty()) {
			shared_ptr<Command> cmd = commandQueue.front();
			mutex.lock();
			game->mutex.lock();
			commandQueue.pop();
			if(isAllowed(cmd))
				execute(cmd);
			game->mutex.unlock();
			mutex.unlock();
		}
		if(aiPlayers.count(this->game->getCurrentPlayer())) {
			game->mutex.lock();
			execute(aiPlayers[this->game->getCurrentPlayer()]->run(this->game));
			game->mutex.unlock();
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

	void Engine::operator()() {
		while (true) {
			if (commandQuit) break;
			flushCommands();
			this_thread::sleep_for(chrono::milliseconds(300));
		}
	}

	void engineThreadFunction(shared_ptr<engine::Engine> engine) {
		(*engine)();
	}

	shared_ptr<Command> commandFromJSON(Json::Value jcmd) {
		CommandType type = (CommandType) jcmd["type"].asInt();
		shared_ptr<Command> cmd;
		if (type == COMMAND_SKIP_ROUND || type == COMMAND_ABANDON || type == COMMAND_USE_ITEM) {
			shared_ptr<ActionCommand> scmd = make_shared<ActionCommand>((CommandType) jcmd["type"].asInt(), jcmd["playerId"].asInt());
			cmd = scmd;
		}
		else if (type == COMMAND_CHOOSE_HEADQUARTERS || type == COMMAND_CHOOSE_REINFORCEMENT || type == COMMAND_BUILD_PORT) {
			shared_ptr<ChoiceCommand> scmd = make_shared<ChoiceCommand>((CommandType) jcmd["type"].asInt(), jcmd["playerId"].asInt(), jcmd["landId"].asInt());
			cmd = scmd;
		}
		else if (type == COMMAND_ATTACK) {
			std::vector<Interaction> interactions;
			for (int i = 0; i < jcmd["interactions"].size(); i++) {
				interactions.push_back(Interaction(jcmd["interactions"][i]["landOne"].asInt(), jcmd["interactions"][i]["landTwo"].asInt()));
			}
			shared_ptr<AttackCommand> scmd = make_shared<AttackCommand>(jcmd["playerId"].asInt(), interactions);
			cmd = scmd;
		}
		else {
			shared_ptr<MoveCommand> scmd = make_shared<MoveCommand>(jcmd["playerId"].asInt(), Interaction(jcmd["interaction"]["landOne"].asInt(), jcmd["interaction"]["landTwo"].asInt()), jcmd["ratio"].asFloat(), jcmd["hero"].asBool());
			cmd = scmd;
		}
		return cmd;
	}

}
