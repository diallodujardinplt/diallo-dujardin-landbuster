#include "ChoiceCommand.hpp"

using namespace std;

namespace engine {

	ChoiceCommand::ChoiceCommand(CommandType type, unsigned int playerId, unsigned int landId) {
		this->type = type;
		this->playerId = playerId;
		this->landId = landId;
	}

	string ChoiceCommand::toString() const {
		map<CommandType, string> names;
    	names[COMMAND_CHOOSE_HEADQUARTERS] = "choose_headquarters";
    	names[COMMAND_CHOOSE_REINFORCEMENT] = "choose_reinforcement";
    	names[COMMAND_BUILD_PORT] = "build_port";

    	ostringstream oss;
    	oss << "cmd player " << playerId << " : " << names[type] << " ";
    	oss << landId;
    	return oss.str();
	}

	bool ChoiceCommand::isAllowed(state::Game& game) const {
		state::Step step = game.getCurrentStep();
		shared_ptr<state::Player> player = game.getPlayers()[playerId];
		shared_ptr<state::Land> land = (landId >= 0 && landId < (int) game.getLands().size())?game.getLands()[landId] : nullptr;

		switch(type) {
			case COMMAND_CHOOSE_HEADQUARTERS:
				return (!player->getHeadquarters()) && land && (!land->getOwner());
				break;
			case COMMAND_CHOOSE_REINFORCEMENT:
				return (step == state::STEP_REINFORCEMENT && land && land->getOwner() == player);
				break;
			case COMMAND_BUILD_PORT:
				return (step == state::STEP_ACTION && land && land->getOwner() == player && (!land->hasPorts()));
				break;
			default:
				return false;
				break;
		}
	}

	void ChoiceCommand::execute(state::Game& game) {
		shared_ptr<state::Player> player = game.getPlayers()[playerId];
		shared_ptr<state::Land> land = (landId >= 0 && landId < (int) game.getLands().size())?game.getLands()[landId] : nullptr;

		switch(type) {
			case COMMAND_CHOOSE_HEADQUARTERS:
				engine::Engine::getInstance().acquireLand(land, player);
				player->setHeadquarters(land);
				game.nextPlayer();
				break;
			case COMMAND_CHOOSE_REINFORCEMENT: {
				unsigned int reinforcement = 0;
				for(auto _land : game.getLands()) {
					if (_land->getOwner() == player) reinforcement += 3;
				}
				if(reinforcement < 3) reinforcement = 3;
				land->setSoldiersNumber(land->getSoldiersNumber() + reinforcement);
				game.setCurrentStep(state::STEP_ACTION);
				break;
			}
			case COMMAND_BUILD_PORT:
				land->setPorts(true);
				game.setCurrentStep(state::STEP_MOVING);
				break;
			default:
				return;
				break;
		}
	}

	void ChoiceCommand::rollback(state::Game& game) {
		
	}

	unsigned int ChoiceCommand::getLandId() const {
		return landId;
	}

}
