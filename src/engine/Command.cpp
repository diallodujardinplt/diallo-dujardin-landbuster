#include "Command.hpp"

using namespace std;

namespace engine {

    CommandType Command::getType() const {
        return type;
    }

    unsigned int Command::getPlayerId() const {
        return playerId;
    }

}
