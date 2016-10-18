#include "Land.h"

using namespace std;

namespace state {
    std::vector<sf::Vector2u> geometry;
    std::vector<std::shared_ptr<Land>> neighborLands;
    std::shared_ptr<Player> owner;
    bool neutral;
    bool ports;
    LandType type;
    ItemType item;

	Land::Land(vector<sf::Vector2u> geometry) {
		this->geometry = geometry;
	}

	Land::~Land() {
	}
    
	std::vector<sf::Vector2u> Player::getGeometry () const{
        return this->;
    }
    
    
    std::vector<std::shared_ptr<Land>> Player::getNeighborLands () const{
        return this->;
    }
    
    void addNeighborLand (std::shared_ptr<Land> land){
       
    }
    
    unsigned int getSoldiersNumber () const{
        return this->soldiersNumber;
    }
    
    void setSoldiersNumber (unsigned int soldiersNumber);
    std::shared_ptr<Player> getOwner () const;
    void setOwner (std::shared_ptr<Player> owner);
    bool isNeutral () const;
    void setNeutral (bool neutral);
    bool hasPorts () const;
    void setPorts (bool ports);
    virtual LandType getType () const ;
    virtual LandType setType (LandType type);
    ItemType getItem () const;
    void setItem (ItemType item);
  };

}

