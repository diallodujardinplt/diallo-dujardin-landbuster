#include "Command.hpp"

using namespace std;

namespace engine {

	Command::Command(CommandType type, int playerId, int landOneId, int landTwoId) {
		this->type = type;
		this->playerId = playerId;
		this->landOneId = landOneId;
		this->landTwoId = landTwoId;
	}

	string Command::toString() const {
    	map<CommandType, string> names;
    	names[COMMAND_CHOOSE_HEADQUARTERS] = "choose_headquarters";
    	names[COMMAND_CHOOSE_REINFORCEMENT] = "choose_reinforcement";
    	names[COMMAND_BUILD_PORT] = "build_port";
    	names[COMMAND_SKIP_ROUND] = "skip_round";
    	names[COMMAND_ABANDON] = "abandon";
    	names[COMMAND_ATTACK] = "attack";
    	names[COMMAND_MOVE] = "move";
    	names[COMMAND_USE_ITEM] = "use_item";

    	ostringstream oss;
    	oss << "cmd player " << playerId << " : " << names[type] << " ";
    	if(landOneId>=0) oss << landOneId;
    	else oss << "_";
    	if(landTwoId>=0) oss << " " << landTwoId;
    	return oss.str();
	}

}
