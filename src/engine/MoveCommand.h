// Generated by dia2code
#ifndef ENGINE__MOVECOMMAND__H
#define ENGINE__MOVECOMMAND__H

#include <string>

namespace engine {
  class Interaction;
};
namespace state {
  class Game;
};
namespace engine {
  class Command;
}

#include "Interaction.h"
#include "Command.h"

namespace engine {

  /// class MoveCommand - 
  class MoveCommand : public engine::Command {
    // Associations
    // Attributes
  private:
    Interaction interaction;
    float ratio;
    bool hero;
    // Operations
  public:
    MoveCommand (unsigned int playerId, Interaction interaction, float ratio, bool hero);
    std::string toString () const;
    bool isAllowed (state::Game& game) const;
    void execute (state::Game& game);
    Interaction getInteraction () const;
    float getRatio () const;
    bool getHero () const;
    void rollback (state::Game& game);
  };

};

#endif
