// Generated by dia2code
#ifndef STATE__GAME__H
#define STATE__GAME__H

#include <vector>
#include <memory>

namespace state {
  class Game;
  class Cell;
  class Player;
  class Land;
}

#include "Step.h"
#include "ItemType.h"
#include "Cell.h"
#include "Player.h"
#include "Land.h"

namespace state {

  /// class Game - 
  class Game {
    // Associations
    // Attributes
  private:
    Step currentStep;
    std::vector< std::shared_ptr<Player> >::iterator currentPlayer;
    ItemType activatedItem;
    std::vector< std::shared_ptr<Player> > players;
    std::vector< std::shared_ptr<Land> > lands;
    std::vector< std::vector<Cell> > cells;
    // Operations
  public:
    ~Game ();
    static Game& getInstance ();
    void init (unsigned int playersNumber);
    std::shared_ptr<Player> getCurrentPlayer () const;
    void nextPlayer ();
    Step getCurrentStep () const;
    void setCurrentStep (Step step);
    ItemType getActivatedItem () const;
    void setActivatedItem (ItemType item);
    std::vector< std::shared_ptr<Land> > getLands () const;
    std::vector< std::shared_ptr<Player> > getPlayers () const;
    Cell& getCell (unsigned int x, unsigned int y);
    void generateMap ();
  private:
    Game ();
    std::vector<unsigned int> generateAreas (std::vector< std::vector<int> >& ncells);
    unsigned int util_total (std::vector<unsigned int>& v);
    unsigned int util_min (std::vector<unsigned int>& v);
    unsigned int util_max (std::vector<unsigned int>& v);
  };

};

#endif
