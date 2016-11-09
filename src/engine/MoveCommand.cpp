#include "MoveCommand.hpp"

using namespace std;

namespace engine {

	MoveCommand::MoveCommand(unsigned int playerId, Interaction interaction, float ratio, bool hero) {
		this->type = COMMAND_MOVE;
		this->playerId = playerId;
		this->interaction = interaction;
		this->ratio = ratio;
		this->hero = hero;
	}

	string MoveCommand::toString() const {
    	ostringstream oss;
    	oss << "cmd player " << playerId << " : " << "move" << " ";
    	oss << "{" << interaction.landOneId << " -> " << interaction.landTwoId << "}";
    	return oss.str();
	}

	bool MoveCommand::isAllowed(state::Game& game) const {
		state::Step step = game.getCurrentStep();
		shared_ptr<state::Player> player = game.getPlayers()[playerId];
		shared_ptr<state::Land> landOne = (interaction.landOneId >= 0 && interaction.landOneId < (int) game.getLands().size())?game.getLands()[interaction.landOneId] : nullptr;
		shared_ptr<state::Land> landTwo = (interaction.landTwoId >= 0 && interaction.landTwoId < (int) game.getLands().size())?game.getLands()[interaction.landTwoId] : nullptr;

		return step==state::STEP_MOVING && landOne && landTwo && landOne->getOwner() == player && landTwo->getOwner() == player && game.getConnection(landOne, landTwo) != state::CONNECTION_NONE;
	}

	void MoveCommand::execute(state::Game& game) {
		shared_ptr<state::Player> player = game.getPlayers()[playerId];
		shared_ptr<state::Land> landOne = (interaction.landOneId >= 0 && interaction.landOneId < (int) game.getLands().size())?game.getLands()[interaction.landOneId] : nullptr;
		shared_ptr<state::Land> landTwo = (interaction.landTwoId >= 0 && interaction.landTwoId < (int) game.getLands().size())?game.getLands()[interaction.landTwoId] : nullptr;

		unsigned int soldiers = landOne->getSoldiersNumber() * ratio;
		landOne->setSoldiersNumber(landOne->getSoldiersNumber() - soldiers);
		landTwo->setSoldiersNumber(landTwo->getSoldiersNumber() + soldiers);
		if (hero && player->getHeroPosition() == landOne)
			player->setHeroPosition(landTwo);
		game.nextPlayer();
	}

	Interaction MoveCommand::getInteraction() const {
		return interaction;
	}

	float MoveCommand::getRatio() const {
		return ratio;
	}

	bool MoveCommand::getHero() const {
		return hero;
	}

}
