// Generated by dia2code
#ifndef STATE__LAND__H
#define STATE__LAND__H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace state {
  class Player;
  class Land;
}

#include "Player.h"
#include "LandType.h"
#include "ItemType.h"

namespace state {

  /// class Land - 
  class Land {
    // Attributes
  private:
    std::vector<sf::Vector2u> geometry;
    std::vector<std::shared_ptr<Land>> neighborLands;
    unsigned int soldiersNumber;
    std::shared_ptr<Player> owner;
    bool neutral;
    bool ports;
    LandType type;
    ItemType item;
    // Operations
  public:
    Land (std::vector<sf::Vector2u> geometry);
    ~Land ();
    std::vector<sf::Vector2u> getGeometry () const;
    std::vector<std::shared_ptr<Land>> getNeighborLands () const;
    void addNeighborLand (std::shared_ptr<Land> land);
    unsigned int getSoldiersNumber () const;
    void setSoldiersNumber (unsigned int soldiersNumber);
    std::shared_ptr<Player> getOwner () const;
    void setOwner (std::shared_ptr<Player> owner);
    bool isNeutral () const;
    void setNeutral (bool neutral);
    bool hasPorts () const;
    void setPorts (bool ports);
    LandType getType () const;
    LandType setType (LandType type);
    ItemType getItem () const;
    void setItem (ItemType item);
  };

};

#endif