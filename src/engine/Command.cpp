#include "Command.hpp"

using namespace std;

namespace engine {

	Command::Command(CommandType type, int playerId, int landOneId, int landTwoId) {
		this->type = type;
		this->playerId = playerId;
		this->landOneId = landOneId;
		this->landTwoId = landTwoId;
	}

}
