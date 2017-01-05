// Generated by dia2code
#ifndef SERVER__SERVER__H
#define SERVER__SERVER__H

#include <vector>
#include <memory>

namespace engine {
  class Command;
}

#include "engine/Command.h"

namespace server {

  /// class Server - 
  class Server {
    // Associations
    // Attributes
  private:
    std::vector< std::shared_ptr<Command> > history;
    // Operations
  private:
    void pushCommand (std::shared_ptr<Command> command);
    std::vector< std::shared_ptr<Command> > getCommands (unsigned int from) const;
  };

};

#endif
