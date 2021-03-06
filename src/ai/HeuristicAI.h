// Generated by dia2code
#ifndef AI__HEURISTICAI__H
#define AI__HEURISTICAI__H

#include <memory>

namespace state {
  class Game;
};
namespace ai {
  class AI;
}

#include "state/Game.h"
#include "AI.h"

namespace ai {

  /// class HeuristicAI - 
  class HeuristicAI : public ai::AI {
    // Operations
  public:
    HeuristicAI (unsigned int playerId);
    std::shared_ptr<engine::Command> run (std::shared_ptr<state::Game> game);
  };

};

#endif
