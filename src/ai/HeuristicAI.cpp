#include "HeuristicAI.hpp"

using namespace std;

namespace ai {

	HeuristicAI::HeuristicAI(unsigned int playerId) : AI(playerId) {}

	shared_ptr<engine::Command> HeuristicAI::run(shared_ptr<state::Game> game) {

		shared_ptr<state::Player> player = game->getPlayers()[playerId];

		if (!player->getHeadquarters()) {
			unsigned int max = 0, maxId = 0;
			for (auto land : game->getLands()) {
				if (!land->getOwner() && land->getSoldiersNumber() > max) {
					max = land->getSoldiersNumber();
					maxId = land->getId();
				}
			}
			return make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_HEADQUARTERS, playerId, maxId);
		}

		if (game->getCurrentStep() == state::STEP_REINFORCEMENT) {
			return make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_REINFORCEMENT, playerId, player->getHeadquarters()->getId());
		}
		else if (game->getCurrentStep() == state::STEP_ACTION) {
			for (auto land : game->getPlayerLands(playerId)) {
				bool hasConquerableNeighbor = false;
				for (auto neighbor : land->getNeighborLands()) {
					if (neighbor->getOwner() != player && game->getAttack(land, neighbor) > game->getDefense(neighbor)) {
						vector<engine::Interaction> interactions;
						interactions.push_back(engine::Interaction(land->getId(), neighbor->getId()));
						return make_shared<engine::AttackCommand>(playerId, interactions);
					}
				}
			}
			for (auto land : game->getPlayerLands(playerId)) {
				if (!land->hasPorts()) {
					return make_shared<engine::ChoiceCommand>(engine::COMMAND_BUILD_PORT, playerId, land->getId());
				}
			}
		}
		else if (game->getCurrentStep() == state::STEP_MOVING) {
			vector<unsigned int> explored;
			while (explored.size() < game->getPlayerLands(playerId).size()) {
				unsigned int max = 0, maxId = 0;
				for (auto land : game->getPlayerLands(playerId)) {
					if (land->getSoldiersNumber() > max && find(explored.begin(), explored.end(), land->getId()) == explored.end()) {
						max = land->getSoldiersNumber();
						maxId = land->getId();
					}
				}
				for (auto neighbor : game->getLands()[maxId]->getNeighborLands()) {
					if (neighbor->getOwner() == player) {
						return make_shared<engine::MoveCommand>(playerId, engine::Interaction(maxId, neighbor->getId()), 0.5, true);
					}
				}
			}
		}

	}

}
