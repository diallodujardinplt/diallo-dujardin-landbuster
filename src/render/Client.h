// Generated by dia2code
#ifndef RENDER__CLIENT__H
#define RENDER__CLIENT__H

#include <memory>

namespace state {
  class Player;
  class Land;
};
namespace render {
  class Client;
};
namespace state {
  class Game;
};
namespace engine {
  class Engine;
  class Command;
};
namespace render {
  class Renderer;
}

#include "state/Player.h"
#include "state/Land.h"
#include "engine/Engine.h"
#include "engine/Command.h"
#include "state/Game.h"
#include "Renderer.h"

namespace render {

  /// class Client - 
  class Client {
    // Associations
    // Attributes
  private:
    std::shared_ptr<state::Player> player;
    std::shared_ptr<state::Land> selectedInfoLand;
    std::shared_ptr<state::Land> selectedLand;
    bool debugMode;
    // Operations
  public:
    ~Client ();
    static Client& getInstance ();
    void run (state::Game& game, std::shared_ptr<engine::Engine> engine);
    std::shared_ptr<state::Player> getPlayer () const;
    void setPlayer (std::shared_ptr<state::Player> player);
    std::shared_ptr<state::Land> getSelectedInfoLand () const;
    void setSelectedInfoLand (std::shared_ptr<state::Land> land);
    std::shared_ptr<state::Land> getSelectedLand () const;
    void setSelectedLand (std::shared_ptr<state::Land> land);
    bool getDebugMode () const;
  private:
    Client ();
    void sendCommand (std::shared_ptr<engine::Engine> engine, std::shared_ptr<engine::Command> command);
  };

};

#endif
