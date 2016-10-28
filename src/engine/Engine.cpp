#include "Engine.hpp"

using namespace std;

namespace engine {
	
	Engine::Engine() {}

	Engine& Engine::getInstance() {
		static Engine engine;
		return engine;
	}

	void Engine::pushCommand(Command command) {
		commandQueue.push(command);
	}

	void Engine::flushCommands() {
		while(!commandQueue.empty()) {
			Command cmd = commandQueue.front();
			commandQueue.pop();
			if(isAllowed(cmd))
				execute(cmd);
		}
	}

	bool Engine::isAllowed(Command command) {
		state::Game& game = state::Game::getInstance();

		if(command.playerId != game.getCurrentPlayer() || !game.getPlayers()[command.playerId]->isAlive())
			return false;

		state::Step step = game.getCurrentStep();
		shared_ptr<state::Player> player = game.getPlayers()[command.playerId];
		shared_ptr<state::Land> landOne = (command.landOneId >= 0 && command.landOneId < game.getLands().size())?game.getLands()[command.landOneId] : nullptr;
		shared_ptr<state::Land> landTwo = (command.landTwoId >= 0 && command.landTwoId < game.getLands().size())?game.getLands()[command.landTwoId] : nullptr;

		switch(command.type) {
			case COMMAND_CHOOSE_HEADQUARTERS:
				return (!player->getHeadquarters()) && landOne && (!landOne->getOwner());
				break;
			case COMMAND_CHOOSE_REINFORCEMENT:
				return (step == state::STEP_REINFORCEMENT && landOne && landOne->getOwner() == player);
				break;
			case COMMAND_BUILD_PORT:
				return (step == state::STEP_ACTION && landOne && landOne->getOwner() == player && (!landOne->hasPorts()));
				break;
			case COMMAND_SKIP_ROUND:
				return true;
				break;
			case COMMAND_ABANDON:
				return true;
				break;
			case COMMAND_ATTACK:
				if (!(step==state::STEP_ACTION && landOne && landTwo && landOne->getOwner() == player && landTwo->getOwner() != landOne->getOwner() && game.areConnected(landOne, landTwo))) return false;
				if (landTwo->getOwner()) return game.getAttack(landOne, landTwo) > game.getDefense(landTwo);
				else return true;
				break;
			case COMMAND_MOVE:
				return step==state::STEP_MOVING && landOne && landTwo && landOne->getOwner() == player && landTwo->getOwner() == player && game.areConnected(landOne, landTwo);
				break;
			case COMMAND_USE_ITEM:
				//TODO
				return false;
				break;
			default:
				return false;
				break;
		}
	}

	void Engine::execute(Command command) {
		cout << command.toString() << " [EXECUTED]" << endl;

		state::Game& game = state::Game::getInstance();

		state::Step step = game.getCurrentStep();
		shared_ptr<state::Player> player = game.getPlayers()[command.playerId];
		shared_ptr<state::Land> landOne = (command.landOneId >= 0 && command.landOneId < game.getLands().size())?game.getLands()[command.landOneId] : nullptr;
		shared_ptr<state::Land> landTwo = (command.landTwoId >= 0 && command.landTwoId < game.getLands().size())?game.getLands()[command.landTwoId] : nullptr;

		switch(command.type) {
			case COMMAND_CHOOSE_HEADQUARTERS:
				player->setHeadquarters(landOne);
				landOne->setOwner(player);
				game.nextPlayer();
				break;
			case COMMAND_CHOOSE_REINFORCEMENT: {
				unsigned int reinforcement = 0;
				for(auto land : game.getLands()) {
					if (land->getOwner() == player) reinforcement += 3;
				}
				if(reinforcement < 3) reinforcement = 3;
				landOne->setSoldiersNumber(landOne->getSoldiersNumber() + reinforcement);
				game.setCurrentStep(state::STEP_ACTION);
				break;
			}
			case COMMAND_BUILD_PORT:
				landOne->setPorts(true);
				game.setCurrentStep(state::STEP_MOVING);
				break;
			case COMMAND_SKIP_ROUND:
				game.nextPlayer();
				break;
			case COMMAND_ABANDON:
				defeat(player, nullptr);
				game.nextPlayer();
				break;
			case COMMAND_ATTACK:
				if(landTwo->getOwner()) {
					int soldiers = landTwo->getSoldiersNumber() - (game.getAttack(landOne, landTwo) - game.getDefense(landTwo));
					if(soldiers > 0) landTwo->setSoldiersNumber(soldiers);
					else {
						landTwo->setSoldiersNumber(0);
						if(landTwo->getOwner()->getHeadquarters()==landTwo) defeat(landTwo->getOwner(), player);
						else landTwo->setOwner(player);
					}
				}
				else {
					landTwo->setOwner(player);
				}
				break;
			case COMMAND_MOVE: {
				unsigned int soldiers = landOne->getSoldiersNumber() / 2;
				landOne->setSoldiersNumber(landOne->getSoldiersNumber() - soldiers);
				landTwo->setSoldiersNumber(landTwo->getSoldiersNumber() + soldiers);
				game.nextPlayer();
				break;
			}
			case COMMAND_USE_ITEM:
				//TODO.
				break;
			default:
				return;
				break;
		}
	}

	void Engine::defeat(shared_ptr<state::Player> player, shared_ptr<state::Player> killer) {
		player->setAlive(false);
		for(auto land : state::Game::getInstance().getLands()) {
			if(land->getOwner() == player) {
				land->setOwner(killer);
			}
		}
	}

}
