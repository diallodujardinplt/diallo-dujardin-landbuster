#include "Engine.hpp"

using namespace std;

namespace engine {
	
	Engine::Engine() {}

	Engine& Engine::getInstance() {
		static Engine engine;
		return engine;
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
	}

	bool Engine::isAllowed(shared_ptr<Command> command) {
		state::Game& game = state::Game::getInstance();

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

		cout << command->toString() << " [EXECUTED]" << endl;

		state::Game& game = state::Game::getInstance();

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

	void Engine::defeat(shared_ptr<state::Player> player, shared_ptr<state::Player> killer) {
		player->setAlive(false);
		for(auto land : state::Game::getInstance().getLands()) {
			if(land->getOwner() == player) {
				land->setOwner(killer);
			}
		}
	}

	void Engine::acquireLand(shared_ptr<state::Land> land, shared_ptr<state::Player> newOwner) {
		//Kill soldiers, hero if present, divide all troops by 2 if dead hero
		if(land->getOwner()) {
			land->setSoldiersNumber(0);
			if(land->getOwner()->getHeroPosition()==land) {
				land->getOwner()->setHeroPosition(nullptr);
				land->getOwner()->setDeadHero(true);
				for(auto l : state::Game::getInstance().getLands()) {
					if(l->getOwner() == land->getOwner())
						l->setSoldiersNumber(l->getSoldiersNumber() / 2);
				}
			}
			if(land->getOwner()->getHeadquarters()==land) defeat(land->getOwner(), newOwner);
		}

		//Acquire land
		land->setOwner(newOwner);

		//Acquire hero if not dead yet and four different land types acquired
		if(newOwner && !newOwner->getHeroPosition()) {
			map<state::LandType, bool> possessions;
			possessions[state::LAND_MEADOW] = false;
			possessions[state::LAND_FOREST] = false;
			possessions[state::LAND_DESERT] = false;
			possessions[state::LAND_COASTAL] = false;
			possessions[state::LAND_MOUNTAIN] = false;
			unsigned int diffTypes = 0;
			for(auto l : state::Game::getInstance().getLands()) {
				if (l->getOwner() == newOwner) {
					if(!possessions[l->getType()]) {
						possessions[l->getType()] = true;
						diffTypes++;
					}
				}
			}
			if(diffTypes>=4 && !newOwner->getDeadHero()) {
				newOwner->setHeroPosition(newOwner->getHeadquarters());
			}
		}

		//Gather item if present
		if(newOwner && land->getItem() != state::ITEM_NONE) {
			acquireItem(land->getItem(), newOwner);
			land->setItem(state::ITEM_NONE);
		}
	}

	void Engine::acquireItem(state::ItemType item, shared_ptr<state::Player> player) {

		if(item == state::ITEM_ATTACK_ANYWHERE || item == state::ITEM_UNLIMITED_ATTACK || item == state::ITEM_ATTACK_AND_BUILD) {
			player->setStoredItem(item);
		}
		else if(item != state::ITEM_NONE) {
			executeItem(item, player);
		}

	}

	void Engine::executeItem(state::ItemType item, shared_ptr<state::Player> player) {

		if(item == state::ITEM_ATTACK_ANYWHERE || item == state::ITEM_UNLIMITED_ATTACK || item == state::ITEM_ATTACK_AND_BUILD || item == state::ITEM_PLAY_TWICE) {
			state::Game::getInstance().setActivatedItem(item);
		}
		else if (item == state::ITEM_SOLDIERS_BONUS) {
			for(auto land : state::Game::getInstance().getLands()) {
				if(land->getOwner() == player)
					land->setSoldiersNumber(1.1 * land->getSoldiersNumber());
			}
		}
		else if (item == state::ITEM_SOLDIERS_MALUS) {
			for(auto land : state::Game::getInstance().getLands()) {
				if(land->getOwner() == player)
					land->setSoldiersNumber(0.9 * land->getSoldiersNumber());
			}
		}
		else if (item == state::ITEM_LOSE_LAND) {
			vector< shared_ptr<state::Land> > lands;
			for(auto l : state::Game::getInstance().getLands()) {
				if (l->getOwner() == player && player->getHeadquarters()!=l)
					lands.push_back(l);
			}

			// TO BE CHECKED
			/*if(lands.empty()) return;
			shared_ptr<state::Land> max = lands[0];
			for(auto l : lands) {
				if (l->getSoldiersNumber()>max->getSoldiersNumber()) max=l;
			}

			auto max = lands.begin();
			for(auto it = lands.begin(); it != lands.end(); ++it) {
				if ((*it)->getSoldiersNumber() > (*max)->getSoldiersNumber()) max=it;
			}
			lands.erase(max);

			if(lands.empty()) return;

			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> randLand(0, lands.size() - 1);
			unsigned int index = randLand(gen);
			
			acquireLand(lands[index], nullptr);*/
		}
		else if (item == state::ITEM_FAT_BONUS) {
			shared_ptr<state::Player> max1 = state::Game::getInstance().getPlayers()[0], max2 = state::Game::getInstance().getPlayers()[1];
			for(auto p : state::Game::getInstance().getPlayers()) {
				if (state::Game::getInstance().getSoldiersCount(p) > state::Game::getInstance().getSoldiersCount(max1)) max1 = p;
				else if (state::Game::getInstance().getSoldiersCount(p) > state::Game::getInstance().getSoldiersCount(max2)) max2 = p;
			}
/*
			vector< shared_ptr<state::Land> > lands;
			for(auto l : state::Game::getInstance().getLands()) {
				if (l->getOwner() == max1 && player->getHeadquarters()!=l)
					lands.push_back(l);
			}

			if(lands.empty()) return;
			shared_ptr<state::Land> max = lands[0];
			for(auto l : lands) {
				if (l->getSoldiersNumber()>max->getSoldiersNumber()) max=l;
			}

			auto max = lands.begin();
			for(auto it = lands.begin(); it != lands.end(); ++it) {
				if ((*it)->getSoldiersNumber() > (*max)->getSoldiersNumber()) max=it;
			}
		*/}

	}

}
