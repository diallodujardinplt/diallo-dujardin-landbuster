// Generated by dia2code
#ifndef ENGINE__ACTIONCOMMAND__H
#define ENGINE__ACTIONCOMMAND__H

#include <string>

namespace state {
  class Game;
};
namespace engine {
  class Command;
  class Engine;
}

#include "CommandType.h"
#include "Command.h"
#include "Engine.h"

namespace engine {

  /// class ActionCommand - 
  class ActionCommand : public engine::Command {
    // Operations
  public:
    ActionCommand (CommandType type, unsigned int playerId);
    std::string toString () const;
    bool isAllowed (state::Game& game) const;
    void execute (state::Game& game);
  };

};

#endif