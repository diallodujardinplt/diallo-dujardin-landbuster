// Generated by dia2code
#ifndef STATE__LAND__H
#define STATE__LAND__H

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>

namespace state {
  class Player;
  class Land;
  class Cell;
}

#include "Player.h"
#include "LandType.h"
#include "ItemType.h"
#include "Cell.h"

namespace state {

  /// class Land - 
  class Land {
    // Associations
    // Attributes
  private:
    unsigned int id;
    std::vector<sf::Vector2u> geometry;
    std::vector< std::shared_ptr<Land> > neighborLands;
    unsigned int soldiersNumber;
    std::shared_ptr<Player> owner;
    bool fort;
    bool ports;
    LandType type;
    ItemType item;
    unsigned int itemLifetime;
    // Operations
  public:
    Land (unsigned int id);
    Land (std::shared_ptr<Land> toCopy);
    ~Land ();
    unsigned int getId () const;
    const std::vector<sf::Vector2u>& getGeometry () const;
    void setGeometry (std::vector<sf::Vector2u> geometry);
    std::vector< std::shared_ptr<Land> > getNeighborLands () const;
    void addNeighborLand (std::shared_ptr<Land> land);
    unsigned int getSoldiersNumber () const;
    void setSoldiersNumber (unsigned int soldiersNumber);
    std::shared_ptr<Player> getOwner () const;
    void setOwner (std::shared_ptr<Player> owner);
    bool hasFort () const;
    void setFort (bool fort);
    bool hasPorts () const;
    void setPorts (bool ports);
    LandType getType () const;
    void setType (LandType type);
    ItemType getItem () const;
    void setItem (ItemType item);
    unsigned int getItemLifetime () const;
    void setItemLifetime (unsigned int timeLeft);
  };

};

#endif
