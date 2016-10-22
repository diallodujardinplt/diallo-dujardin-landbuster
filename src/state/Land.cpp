#include "Land.hpp"

using namespace std;

namespace state {

	Land::Land() {
	}

	Land::~Land() {
	}
    
	const std::vector<sf::Vector2u>& Land::getGeometry () const {
        return geometry;
    }

    void Land::setGeometry(vector<sf::Vector2u> geometry) {
    	this->geometry = geometry;
    }
    
    
        /*
         Getters, Setters and methodes
         */
        
    std::vector<std::shared_ptr<Land>> Land::getNeighborLands () const{
        return this->neighborLands;
    }
    
    void Land::addNeighborLand (std::shared_ptr<Land> land){
       
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
        return true;
    }
    
    void Land::setNeutral (bool neutral){
        this->neutral=true;
    }
    
    bool Land::hasPorts () const{
        return false;
    }
    
    void Land::setPorts (bool ports){
        this->ports=false;
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