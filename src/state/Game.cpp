#include "Game.hpp"

using namespace std;

namespace state {

	Game::Game() {
		cells.resize(64);
		for(vector< vector<Cell> >::iterator it = cells.begin(); it != cells.end(); ++it) {
			it->resize(64);
		}
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
		std::vector<std::vector<unsigned int>> ncells(64, std::vector<unsigned int>(64, 0));
		unsigned int mediumSize = 41;
		unsigned landsCount = 0;
		
		// TODO (temp code)
		landsCount = 6;
		
		for (unsigned int y = 0; y < 64; ++y) {
			for (unsigned int x = 0; x < 64; ++x) {
				if(x < 16 && y < 16) ncells[x][y] = 0;
				else if(x >= 16 && y < 16) ncells[x][y] = 1;
				else if(x < 16 && y >= 16 && y < 40) ncells[x][y] = 2;
				else if(x >= 16 && y >= 16 && y < 40) ncells[x][y] = 3;
				else if(x < 16 && y >= 40) ncells[x][y] = 4;
				else ncells[x][y] = 5;
			}
		}

		// Generate lands
		for (unsigned int i = 0; i < landsCount; ++i) {
			this->lands.push_back(make_shared<Land>());
			std::vector<sf::Vector2u> geometry;
			for (unsigned int y = 0; y < 64; ++y) {
				for (unsigned int x = 0; x < 64; ++x) {

					sf::Vector2u pos(x, y);

					if (ncells[x][y] == i) {
						cells[x][y].position = pos;
						cells[x][y].land = this->lands.back();
						geometry.push_back(sf::Vector2u(x, y));
					}
				}
			}

			this->lands.back()->setGeometry(geometry);
			this->lands.back()->setType(LAND_MOUNTAIN);
			this->lands.back()->setSoldiersNumber(30);
		}

		this->lands.back()->setOwner(this->players.front());
		this->lands.back()->setType(LAND_FOREST);
		this->lands.back()->setSoldiersNumber(54);

		this->lands.front()->setType(LAND_WATER);

		this->lands[3]->setType(LAND_MEADOW);
		this->lands[4]->setType(LAND_MEADOW);
		this->lands[4]->setOwner(this->players[2]);


		// Debug display of cells with borders
		/*cout << "CELLS" << endl;
		for (unsigned int y = 0; y < 64; ++y) {
			//Borders top
			for (unsigned int x = 0; x < 64; ++x) {
				cout << " ";
				if ((y>0)?(cells[x][y-1] != cells[x][y]):false) cout << "_";
				else cout << " ";
				cout << " ";
			}
			cout << endl;
			//Cells
			for (unsigned int x = 0; x < 64; ++x) {
				if ((x>0)?(cells[x-1][y] != cells[x][y]):false) cout << "|";
				else cout << " ";
				cout << cells[x][y];
				if ((x<63)?(cells[x+1][y] != cells[x][y]):false) cout << "|";
				else cout << " ";
			}
			cout << endl;
			//Borders bottom
			for (unsigned int x = 0; x < 64; ++x) {
				cout << " ";
				if ((y<63)?(cells[x][y+1] != cells[x][y]):false) cout << "_";
				else cout << " ";
				cout << " ";
			}
		}
		cout << "ENDCELLS" << endl;*/

	}

	vector< shared_ptr<Land> > Game::getLands() const {
		return lands;
	}

	vector< shared_ptr<Player> > Game::getPlayers() const {
		return players;
	}
        
    std::shared_ptr<Player> Game::getCurrentPlayer () const{
            return *currentPlayer;
    }
    
    Step Game::getCurrentStep () const {
            return this->currentStep;
    }
    
    void Game::setCurrentStep (Step step){
            this->currentStep=step;
    }
    
    ItemType Game::getActivatedItem () const{
            return this->activatedItem;
    }
    
    void Game::setActivatedItem (ItemType item){
            this->activatedItem=item;
    }

    Cell& Game::getCell(unsigned int x, unsigned int y) {
    	return cells[x][y];
    }
             

}
