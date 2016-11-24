#include "Land.hpp"

using namespace std;

namespace state {

	Land::Land(unsigned int id) {
        this->id = id;
        soldiersNumber = 0;
        fort = false;
        ports = false;
        type = LAND_MEADOW;
        item = ITEM_NONE;
        itemLifetime = 0;
	}

    Land::Land(shared_ptr<Land> toCopy) {
        id = toCopy->id;
        geometry = toCopy->geometry;
        soldiersNumber = toCopy->soldiersNumber;
        owner = nullptr;
        fort = toCopy->fort;
        ports = toCopy->ports;
        type = toCopy->type;
        item = toCopy->item;
        itemLifetime = toCopy->itemLifetime;
    }

	Land::~Land() {
	}

    unsigned int Land::getId() const {
        return id;
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
        if (find(this->neighborLands.begin(), this->neighborLands.end(), land) == this->neighborLands.end())
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
    
    bool Land::hasFort () const{
        return this->fort;
    }
    
    void Land::setFort (bool fort){
        this->fort = fort;
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

    unsigned int Land::getItemLifetime () const{
        return this->itemLifetime;
    }

    void Land::setItemLifetime (unsigned int timeLeft){
        this->itemLifetime=timeLeft;
    }


    
}