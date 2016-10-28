// Generated by dia2code
#ifndef ENGINE__ENGINE__H
#define ENGINE__ENGINE__H

#include <queue>
#include <memory>

namespace engine {
  class Command;
  class Engine;
};
namespace state {
  class Player;
  class Game;
}

#include "Command.h"
#include "state/Player.h"
#include "state/Game.h"

namespace engine {

  /// class Engine - 
  class Engine {
    // Associations
    // Attributes
  private:
    std::queue<Command> commandQueue;
    // Operations
  public:
    /// 	
    static Engine& getInstance ();
    void pushCommand (Command command);
    void flushCommands ();
    bool isAllowed (Command command);
  private:
    Engine ();
    void execute (Command command);
    void defeat (std::shared_ptr<state::Player> player, std::shared_ptr<state::Player> killer);
  };

};

#endif