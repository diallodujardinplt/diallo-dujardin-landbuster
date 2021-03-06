#include "DumbAI.hpp"

/**
 * DumbAI Class
 * If there is a free land next to one f the and chose by the AI, it attacks it
 * If there is no more free land. The game ends.
**/

using namespace ai;
using namespace engine;
using namespace state;
using namespace std;


DumbAI::DumbAI(unsigned int playerId) : AI(playerId) {}

shared_ptr<engine::Command> DumbAI::run(shared_ptr<state::Game> game){

	/*	unsigned int playerId = player->getId();

			//shared_ptr<state::Land> targetId =  state::land::getId(); 

			//sf::Vector2i aiPosition;
			//aiPosition.x = 
			//aiPosition.y = 
			unsigned int targetId = game.getCell(rand() % 5 / CELL_WIDTH, rand() % 5 / CELL_HEIGHT).land->getId();

				//setSelectedInfoLand(target); 
				
				engine.pushCommand(make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_HEADQUARTERS, playerId, targetId));
			
				engine.pushCommand(make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_REINFORCEMENT, playerId, targetId));
				
				engine.pushCommand(make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_REINFORCEMENT, playerId, targetId));

				engine.pushCommand(make_shared<engine::ChoiceCommand>(engine::COMMAND_BUILD_PORT, playerId, targetId));
				
				//pour faire une attaque
				vector<engine::Interaction> attacks;
				//attacks.push_back(engine::Interaction(, targetId);
				engine.pushCommand(make_shared<engine::AttackCommand>(playerId, attacks));
			*/

	return make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_HEADQUARTERS, playerId, 0);

}	
