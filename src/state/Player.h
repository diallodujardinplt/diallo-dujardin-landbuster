// Generated by dia2code
#ifndef STATE__PLAYER__H
#define STATE__PLAYER__H

#include <memory>

namespace state {
  class Land;
}

#include "Land.h"
#include "ItemType.h"

namespace state {

  /// class Player - 
  class Player {
    // Associations
    // Attributes
  private:
    /// 		
    std::shared_ptr<Land> headquarters;
    unsigned int itemLifetime;
    std::shared_ptr<Land> heroPosition;
    bool alive;
    ItemType storedItem;
    sf::Color color;
    // Operations
  public:
    Player (sf::Color color);
    ~Player ();
    std::shared_ptr<Land> getHeadquarters () const;
    void setHeadquarters (std::shared_ptr<Land> headquarters);
    unsigned int getItemLifetime () const;
    void setItemLifetime (unsigned int timeLeft);
    std::shared_ptr<Land> getHeroPosition () const;
    void setHeroPosition (std::shared_ptr<Land> heroPosition);
    bool isAlive () const;
    void setAlive (bool alive);
    ItemType getStoredItem () const;
    void setStoredItem (ItemType storedItem);
    sf::Color getColor () const;
  };

};

#endif
