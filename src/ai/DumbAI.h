// Generated by dia2code
#ifndef AI__DUMBAI__H
#define AI__DUMBAI__H

#include <memory>

namespace state {
  class Player;
  class Game;
};
namespace engine {
  class Engine;
};
namespace ai {
  class AI;
}

#include "state/Player.h"
#include "AI.h"

namespace ai {

  /// class DumbAI - 
  class DumbAI : public ai::AI {
    // Operations
  public:
    DumbAI (std::shared_ptr<state::Player> player);
    virtual void  run (state::Game&  game, engine::Engine& engine);
  };

};

#endif