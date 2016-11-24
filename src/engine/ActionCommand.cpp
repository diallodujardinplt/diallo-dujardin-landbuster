#include "ActionCommand.hpp"

using namespace std;

namespace engine {

	ActionCommand::ActionCommand(CommandType type, unsigned int playerId) {
		this->type = type;
		this->playerId = playerId;
	}

	string ActionCommand::toString() const {
		map<CommandType, string> names;
    	names[COMMAND_SKIP_ROUND] = "skip_round";
    	names[COMMAND_ABANDON] = "abandon";
    	names[COMMAND_USE_ITEM] = "use_item";

    	ostringstream oss;
    	oss << "cmd player " << playerId << " : " << names[type];
    	return oss.str();
	}

	bool ActionCommand::isAllowed(state::Game& game) const {
		state::Step step = game.getCurrentStep();
		shared_ptr<state::Player> player = game.getPlayers()[playerId];

		switch(type) {
			case COMMAND_SKIP_ROUND:
				return true;
				break;
			case COMMAND_ABANDON:
				return true;
				break;
			case COMMAND_USE_ITEM:
				return true;
				break;
			default:
				return false;
				break;
		}
	}

	void ActionCommand::execute(state::Game& game) {
		shared_ptr<state::Player> player = game.getPlayers()[playerId];

		switch(type) {
			case COMMAND_SKIP_ROUND:
				game.nextPlayer();
				break;
			case COMMAND_ABANDON:
				defeat(game, player, nullptr);
				game.nextPlayer();
				break;
			case COMMAND_USE_ITEM:
				executeItem(game, player->getStoredItem(), player);
				player->setStoredItem(state::ITEM_NONE);
				break;
			default:
				return;
				break;
		}
	}

	void ActionCommand::rollback(state::Game& game) {
		
	}

}
