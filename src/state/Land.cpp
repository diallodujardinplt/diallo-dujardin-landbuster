#include "Land.hpp"

using namespace std;

namespace state {

	Land::Land() {
        soldiersNumber = 0;
        neutral = false;
        ports = false;
        type = LAND_MEADOW;
        item = ITEM_NONE;
	}

	Land::~Land() {
	}
    
	const std::vector<sf::Vector2u>& Land::getGeometry () const {
        return geometry;
    }

    void Land::setGeometry(vector<sf::Vector2u> geometry) {
    	this->geometry = geometry;
    }
        
    std::vector<std::shared_ptr<Land>> Land::getNeighborLands () const{
        return this->neighborLands;
    }
    
    void Land::addNeighborLand (std::shared_ptr<Land> land){
       this->neighborLands.push_back(land);
    }
    
    unsigned int Land::getSoldiersNumber () const{
        return this->soldiersNumber;
    }
    
    void Land::setSoldiersNumber (unsigned int soldiersNumber){
        this-> soldiersNumber=soldiersNumber;
    }
    
    std::shared_ptr<Player> Land::getOwner () const{
        return this->owner;
    }
    
    void Land::setOwner (std::shared_ptr<Player> owner){
        this->owner=owner;
    }
    
    bool Land::isNeutral () const{
        return this->neutral;
    }
    
    void Land::setNeutral (bool neutral){
        this->neutral=neutral;
    }
    
    bool Land::hasPorts () const{
        return this->ports;
    }
    
    void Land::setPorts (bool ports){
        this->ports=ports;
    }
    
    LandType Land::getType () const {
        return this->type;
    }
    
    void Land::setType (LandType type){
        this->type=type;
    }
    
    ItemType Land::getItem () const{
        return this->item;
    }
    
    void Land::setItem (ItemType item){
        this->item=item;
    }
    
}