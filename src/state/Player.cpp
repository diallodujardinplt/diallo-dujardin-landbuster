#include "Player.h"

using namespace std;

namespace state {

Player::Player(sf::Color color) {
	this->color = color;
}

    /*
     Getters
     */
    
std::shared_ptr<Land> Player::getHeadquarters () const{
    return this->headquarters;
}
    
unsigned int Player::getItemLifetime () const{
    return this->itemLifetime;
}

std::shared_ptr<Land> Player::getHeroPosition () const{
    return this->heroPosition;
}

bool Player::isAlive() const{
    return true;
}

ItemType Player::getStoredItem() const{
    return this->storedItem;
}

sf::Color Player::getColor () const{
    return this->color;
}

    /*
     Setters
     */


void Player::setHeadquarters (std::shared_ptr<Land> headquarters){
    this->headquarters=headquarters;    
}


void Player::setItemLifetime (unsigned int timeLeft){
    this->itemLifetime=timeLeft;
}


void Player::setHeroPosition (std::shared_ptr<Land> heroPosition){
    this->heroPosition=heroPosition;
}


void Player::setAlive (bool alive){
    this->alive=true;
}

Player::~Player() {
}

void Player::setStoredItem (ItemType storedItem){
    this->storedItem=storedItem;
}

}
