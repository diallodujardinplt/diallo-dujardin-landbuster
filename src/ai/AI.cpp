#include "AI.hpp"

using namespace std;

namespace ai {

	AI::AI(shared_ptr<state::Player> player) {
		this->player = player;
	}

	vector<shared_ptr<engine::Command>> AI::getPossibilities(shared_ptr<state::Game> game, unsigned int playerId) {

		std::vector<shared_ptr<engine::Command>> possibilities;
		possibilities.push_back(make_shared<engine::ActionCommand>(engine::COMMAND_SKIP_ROUND, playerId));
		possibilities.push_back(make_shared<engine::ActionCommand>(engine::COMMAND_ABANDON, playerId));

		if (!game->getPlayers()[playerId]->getHeadquarters()) {
			for (auto land : game->getLands()) {
				if (!land->getOwner()) {
					possibilities.push_back(make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_HEADQUARTERS, playerId, land->getId()));
				}
			}
		}	
		else {

			vector<shared_ptr<state::Land>> ownLands = game->getPlayerLands(game->getPlayers()[playerId]->getId());

			switch (game->getCurrentStep()) {

				case state::STEP_REINFORCEMENT:

					for (auto land : ownLands) {
						possibilities.push_back(make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_REINFORCEMENT, playerId, land->getId()));
					}
					break;

				case state::STEP_ACTION:

					// Build port

					for (auto land : ownLands) {
						bool hasWaterNeighbor = false;
						for (auto neighbor : land->getNeighborLands()) {
							if (neighbor->getType() == state::LAND_WATER) {
								hasWaterNeighbor = true;
								break;
							}
						}
						if (hasWaterNeighbor && !land->hasPorts())
							possibilities.push_back(make_shared<engine::ChoiceCommand>(engine::COMMAND_BUILD_PORT, playerId, land->getId()));
					}

					// Attack

					for (auto land : ownLands) {
						for (auto neighbor : land->getNeighborLands()) {
							if (neighbor->getOwner()->getId() != playerId && game->getAttack(land, neighbor) > game->getDefense(neighbor)) {
								vector<engine::Interaction> interactions;
								interactions.push_back(engine::Interaction(land->getId(), neighbor->getId()));
								possibilities.push_back(make_shared<engine::AttackCommand>(playerId, interactions));
							}
						}
					}
				
					break;

				case state::STEP_MOVING:

					for (auto land1 : ownLands) {
						for (auto land2 : ownLands) {
							if (land1 != land2 && game->getConnection(land1, land2) != state::CONNECTION_NONE) {
								for (float ratio = 0.0; ratio <= 1.0; ratio += 0.25) {						
									if (game->getPlayers()[playerId]->getHeroPosition() == land1)
										possibilities.push_back(make_shared<engine::MoveCommand>(playerId, engine::Interaction(land1->getId(), land2->getId()), ratio, true));
									possibilities.push_back(make_shared<engine::MoveCommand>(playerId, engine::Interaction(land1->getId(), land2->getId()), ratio, false));
								}
							}
						}
					}
					break;

				default:
					break;

			}

		}

		return possibilities;
	}

}