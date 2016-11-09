#include "AttackCommand.hpp"

using namespace std;

namespace engine {

	AttackCommand::AttackCommand(unsigned int playerId, vector<Interaction> interactions) {
		this->type = COMMAND_ATTACK;
		this->playerId = playerId;
		this->interactions = interactions;
	}

	string AttackCommand::toString() const {
    	ostringstream oss;
    	oss << "cmd player " << playerId << " : " << "attack" << " ";
    	for(vector<Interaction>::const_iterator it = interactions.begin(); it != interactions.end(); ++it) {
    		oss << "{" << it->landOneId << " -> " << it ->landTwoId << "} ";
    	}
    	return oss.str();
	}

	bool AttackCommand::isAllowed(state::Game& game) const {
		state::Step step = game.getCurrentStep();
		shared_ptr<state::Player> player = game.getPlayers()[playerId];
		shared_ptr<state::Land> landOne = (interactions[0].landOneId >= 0 && interactions[0].landOneId < (int) game.getLands().size())?game.getLands()[interactions[0].landOneId] : nullptr;
		shared_ptr<state::Land> landTwo = (interactions[0].landTwoId >= 0 && interactions[0].landTwoId < (int) game.getLands().size())?game.getLands()[interactions[0].landTwoId] : nullptr;

		if (!(step==state::STEP_ACTION && landOne && landTwo && landOne->getOwner() == player && landTwo->getOwner() != landOne->getOwner() && game.getConnection(landOne, landTwo) != state::CONNECTION_NONE)) return false;
		if (landTwo->getOwner()) return game.getAttack(landOne, landTwo) > game.getDefense(landTwo);
		return true;
	}

	void AttackCommand::execute(state::Game& game) {
		shared_ptr<state::Player> player = game.getPlayers()[playerId];
		shared_ptr<state::Land> landOne = (interactions[0].landOneId >= 0 && interactions[0].landOneId < (int) game.getLands().size())?game.getLands()[interactions[0].landOneId] : nullptr;
		shared_ptr<state::Land> landTwo = (interactions[0].landTwoId >= 0 && interactions[0].landTwoId < (int) game.getLands().size())?game.getLands()[interactions[0].landTwoId] : nullptr;

		if(landTwo->getOwner()) {
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> randDmg(8, 12);
			float chance_factor = ((float) randDmg(gen)) / 10.0;
			int soldiers = landTwo->getSoldiersNumber() - (game.getAttack(landOne, landTwo) - game.getDefense(landTwo)) * chance_factor;
			if(soldiers > 0) landTwo->setSoldiersNumber(soldiers);
			else {
				landTwo->setSoldiersNumber(0);
				if(landTwo->getOwner()->getHeroPosition()==landTwo) {
					landTwo->getOwner()->setHeroPosition(nullptr);
					for(auto l : game.getLands()) {
						if(l->getOwner() == player)
							l->setSoldiersNumber(l->getSoldiersNumber() / 2);
					}
				}
				if(landTwo->getOwner()->getHeadquarters()==landTwo) Engine::getInstance().defeat(landTwo->getOwner(), player);
				else landTwo->setOwner(player);
			}
		}
		else {
			landTwo->setOwner(player);
		}
		game.setCurrentStep(state::STEP_MOVING);
	}

	vector<Interaction> AttackCommand::getInteractions() const {
		return interactions;
	}

}
