#include "state.h"
#include "Game.h"

using namespace std;

namespace state {

	Game::Game() {
	}

	Game::~Game() {
	}

	Game& Game::getInstance() {
		static Game game;
		return game;
	}

	void Game::init(unsigned int playersNumber) {
		// Create players
		sf::Color playersColors[6];
		playersColors[0] = sf::Color(255,0,0);
		playersColors[1] = sf::Color(0,255,255);
		playersColors[2] = sf::Color(255,255,0);
		playersColors[3] = sf::Color(128,0,255);
		playersColors[4] = sf::Color(34,34,34);
		playersColors[5] = sf::Color(255,0,255);

		for (unsigned int i=0; i<6; ++i) {
			this->players.push_back(make_shared<Player>(playersColors[i]));
		}

		this->currentPlayer = this->players.begin();
		this->currentStep = STEP_REINFORCEMENT;
		this->activatedItem = ITEM_NONE;

		// Generate map
		// Generate cells
		std::vector<std::vector<unsigned int>> cells(64, std::vector<unsigned int>(64, 0));
		unsigned int mediumSize = 41;
		unsigned landsCount = 0;
		
		// TODO (temp code)
		landsCount = 6;
		for (unsigned int x = 0; x < 64; ++x) {
			for (unsigned int y = 0; y < 64; ++y) {
				if(x < 16 && y < 16) cells[x][y] = 0;
				else if(x >= 16 && y < 16) cells[x][y] = 1;
				else if(x < 16 && y >= 16 && y < 40) cells[x][y] = 2;
				else if(x >= 16 && y >= 16 && y < 40) cells[x][y] = 3;
				else if(x < 16 && y >= 40) cells[x][y] = 4;
				else cells[x][y] = 5;
			}
		}

		// Generate lands
		for (unsigned int i = 0; i < landsCount; ++i) {
			std::vector<sf::Vector2u> geometry;
			for (unsigned int x = 0; x < 64; ++x) {
				for (unsigned int y = 0; y < 64; ++y) {
					if (cells[x][y] == i)
						geometry.push_back(sf::Vector2u(x, y));
				}
			}
			this->lands.push_back(make_shared<Land>(geometry));
		}

	}

}
