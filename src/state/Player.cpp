#include "Player.hpp"

using namespace std;

namespace state {

Player::Player(unsigned int id, sf::Color color) {
	this->id = id;
	this->color = color;
    deadHero = false;
	alive = true;
}

    /*
     Getters
     */

unsigned int Player::getId() const {
	return id;
}
    
std::shared_ptr<Land> Player::getHeadquarters () const{
    return this->headquarters;
}

std::shared_ptr<Land> Player::getHeroPosition () const{
    return this->heroPosition;
}

bool Player::getDeadHero() const {
    return deadHero;
}

bool Player::isAlive() const{
    return this->alive;
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


void Player::setHeroPosition (std::shared_ptr<Land> heroPosition){
    this->heroPosition=heroPosition;
}

void Player::setDeadHero(bool deadHero) {
    this->deadHero = deadHero;
}


void Player::setAlive (bool alive){
    this->alive=alive;
}

Player::~Player() {
}

void Player::setStoredItem (ItemType storedItem){
    this->storedItem=storedItem;
}


}
