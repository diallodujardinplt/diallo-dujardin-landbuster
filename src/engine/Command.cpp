#include "Command.hpp"

using namespace std;

namespace engine {

    CommandType Command::getType() const {
        return type;
    }

    unsigned int Command::getPlayerId() const {
        return playerId;
    }

    void Command::defeat(state::Game& game, shared_ptr<state::Player> player, shared_ptr<state::Player> killer) {
		vector<shared_ptr<state::Land>> lands;
		player->setAlive(false);
		for(auto land : game.getLands()) {
			if(land->getOwner() == player) {
				land->setOwner(killer);
				lands.push_back(land);
			}
		}
		executedDefeatLands.push(lands);
	}

	void Command::rollbackDefeat(state::Game& game, shared_ptr<state::Player> player, shared_ptr<state::Player> killer) {
		for(auto land : executedDefeatLands.top()) {
			land->setOwner(player);
		}
		player->setAlive(true);
		executedDefeatLands.pop();
	}

	void Command::acquireLand(state::Game& game, shared_ptr<state::Land> land, shared_ptr<state::Player> newOwner) {
		//Kill soldiers, hero if present, divide all troops by 2 if dead hero
		if(land->getOwner()) {
			land->setSoldiersNumber(0);
			if(land->getOwner()->getHeroPosition()==land) {
				land->getOwner()->setHeroPosition(nullptr);
				land->getOwner()->setDeadHero(true);
				for(auto l : game.getLands()) {
					if(l->getOwner() == land->getOwner())
						l->setSoldiersNumber(l->getSoldiersNumber() / 2);
				}
			}
			if(land->getOwner()->getHeadquarters()==land) defeat(game, land->getOwner(), newOwner);
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
			for(auto l : game.getLands()) {
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
			acquireItem(game, land->getItem(), newOwner);
			land->setItem(state::ITEM_NONE);
		}
	}

	void Command::rollbackAcquireLand(state::Game& game, shared_ptr<state::Land> land, shared_ptr<state::Player> newOwner) {

	}

	void Command::acquireItem(state::Game& game, state::ItemType item, shared_ptr<state::Player> player) {

		if(item == state::ITEM_ATTACK_ANYWHERE || item == state::ITEM_UNLIMITED_ATTACK || item == state::ITEM_ATTACK_AND_BUILD) {
			player->setStoredItem(item);
		}
		else if(item != state::ITEM_NONE) {
			executeItem(game, item, player);
		}

	}

	void Command::rollbackAcquireItem(state::Game& game, state::ItemType item, shared_ptr<state::Player> player) {

	}

	void Command::executeItem(state::Game& game, state::ItemType item, shared_ptr<state::Player> player) {

		if(item == state::ITEM_ATTACK_ANYWHERE || item == state::ITEM_UNLIMITED_ATTACK || item == state::ITEM_ATTACK_AND_BUILD || item == state::ITEM_PLAY_TWICE) {
			game.setActivatedItem(item);
		}
		else if (item == state::ITEM_SOLDIERS_BONUS) {
			for(auto land : game.getLands()) {
				if(land->getOwner() == player)
					land->setSoldiersNumber(1.1 * land->getSoldiersNumber());
			}
		}
		else if (item == state::ITEM_SOLDIERS_MALUS) {
			for(auto land : game.getLands()) {
				if(land->getOwner() == player)
					land->setSoldiersNumber(0.9 * land->getSoldiersNumber());
			}
		}
		else if (item == state::ITEM_LOSE_LAND) {
			vector< shared_ptr<state::Land> > lands;
			for(auto l : game.getLands()) {
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
			shared_ptr<state::Player> max1 = game.getPlayers()[0], max2 = game.getPlayers()[1];
			for(auto p : game.getPlayers()) {
				if (game.getSoldiersCount(p) > game.getSoldiersCount(max1)) max1 = p;
				else if (game.getSoldiersCount(p) > game.getSoldiersCount(max2)) max2 = p;
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

	void Command::rollbackExecuteItem(state::Game& game, state::ItemType item, shared_ptr<state::Player> player) {

	}

}
