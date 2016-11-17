#ifndef DUMBAI_HPP
#define DUMBAI_HPP

#include "../engine/ChoiceCommand.hpp"
#include "../engine/Engine.hpp"
#include "../state/Player.hpp"

class DumbAI {
  private:
    std::shared_ptr<state::Player> player

  public:
    DumbAI (player:std::shared_ptr<state::Player>);
    run(game:state::Game&, playerId: unsigned int);

};

#endif