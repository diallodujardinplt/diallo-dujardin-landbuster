#include "Game.hpp"

using namespace std;

namespace state {

	Game::Game() {
		cells.resize(GRID_WIDTH);
		for(vector< vector<Cell> >::iterator it = cells.begin(); it != cells.end(); ++it) {
			it->resize(GRID_HEIGHT);
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
			this->players.push_back(make_shared<Player>(players.size(), playersColors[i]));
		}

		this->currentPlayer = 0;
		this->currentStep = STEP_REINFORCEMENT;
		this->activatedItem = ITEM_NONE;

	}

	void Game::generateMap() {
		// Generate cells
		vector<vector<int>> ncells(GRID_WIDTH, vector<int>(GRID_HEIGHT, -1));
		unsigned int mediumSize = 41;
		vector<unsigned int> landsSizes = generateAreas(ncells);

		// Generate lands
		for (unsigned int i = 0; i < landsSizes.size(); ++i) {
			if (landsSizes[i] != 0) {
				this->lands.push_back(make_shared<Land>(this->lands.size()));
				vector<sf::Vector2u> geometry;
				for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
					for (unsigned int x = 0; x < GRID_WIDTH; ++x) {

						sf::Vector2u pos(x, y);

						if (ncells[x][y] == i) {
							cells[x][y].position = pos;
							cells[x][y].land = this->lands.back();
							geometry.push_back(sf::Vector2u(x, y));
						}
					}
				}

				this->lands.back()->setGeometry(geometry);

			}
		}

		// Build lands graph

		for (unsigned int i = 0; i < this->lands.size(); ++i) {
			shared_ptr<Land> land = this->lands[i];
			for (unsigned int j = 0; j < land->getGeometry().size(); ++j) {
				unsigned int x = land->getGeometry()[j].x, y = land->getGeometry()[j].y;

				if(x>0 && cells[x-1][y].land != land)
					land->addNeighborLand(cells[x-1][y].land);
				if(x<GRID_WIDTH-1 && cells[x+1][y].land != land)
					land->addNeighborLand(cells[x+1][y].land);
				if(y>0 && cells[x][y-1].land != land)
					land->addNeighborLand(cells[x][y-1].land);
				if(y<GRID_HEIGHT-1 && cells[x][y+1].land != land)
					land->addNeighborLand(cells[x][y+1].land);

			}
		}


		// Generate water

		unsigned int meanSize = 5;
		unsigned int meanCount = 3;

		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> rndCount(meanCount - meanCount/2, meanCount + meanCount/2);
		unsigned int count = rndCount(gen);

		for (unsigned int i = 0; i < count; ++i) {

			uniform_int_distribution<> rndSize(meanSize - meanSize/2, meanSize + meanSize/2);
			unsigned int size = rndSize(gen);

			uniform_int_distribution<> rndStart(0, this->lands.size()-1);
			unsigned int start = rndStart(gen);

			std::shared_ptr<Land> it = this->lands[start];
			it->setType(LAND_WATER);

			for (unsigned int j = 0; j < size; ++j) {

				uniform_int_distribution<> rnd(0, it->getNeighborLands().size()-1);
				unsigned int l = rnd(gen);

				it = it->getNeighborLands()[l];
				it->setType(LAND_WATER);

			}

		}

		// Set soldiers numbers
		unsigned int meanPop = 25;
		unsigned int varPop = 12;
		normal_distribution<> d(meanPop, varPop);
		for (vector< shared_ptr<Land> >::iterator land_it = lands.begin(); land_it != lands.end(); ++land_it) {
			if ((*land_it)->getType() != LAND_WATER) {
				unsigned int sn = round(d(gen));
				if (sn > meanPop*4) sn = 0;
				(*land_it)->setSoldiersNumber(sn);
			}
		}

		// Set lands types
		uniform_int_distribution<> rndType(0, 9);
		LandType distType[10] = {LAND_MEADOW, LAND_MEADOW, LAND_MEADOW, LAND_MEADOW, LAND_FOREST, LAND_FOREST, LAND_FOREST, LAND_DESERT, LAND_DESERT, LAND_MOUNTAIN};
		for (vector< shared_ptr<Land> >::iterator land_it = lands.begin(); land_it != lands.end(); ++land_it) {
			if ((*land_it)->getType() != LAND_WATER) {
				unsigned int i = rndType(gen);
				(*land_it)->setType(distType[i]);
			}
		}
	}

	vector<unsigned int> Game::generateAreas(vector< vector<int> >& ncells) {

		struct vector2u {
			vector2u(unsigned int x, unsigned int y, unsigned int it) {
				this->x = x;
				this->y = y;
				this->it = it;
			}

			unsigned int x,y, it;
		};

		unsigned int meanSize = MEAN_LAND_SIZE;

		std::vector< std::vector<vector2u> > index(GRID_WIDTH, std::vector<vector2u>(GRID_HEIGHT, vector2u(-1, -1, 0)));

		// Set lands sizes

		unsigned int gridSize = GRID_WIDTH * GRID_HEIGHT;
		unsigned int landsCount = gridSize / meanSize;

		vector<unsigned int> sizes(landsCount);

		random_device rd;
		mt19937 gen(rd());
		normal_distribution<> d(meanSize, meanSize/6);

		for(vector<unsigned int>::iterator size = sizes.begin(); size != sizes.end(); ++size) {
			(*size) = round(d(gen));
		}

		while (util_total(sizes) < gridSize) sizes[util_min(sizes)]++;
		while (util_total(sizes) > gridSize) sizes[util_max(sizes)]--;

		vector<unsigned int> realSizes(landsCount);
		for(vector<unsigned int>::iterator rs = realSizes.begin(); rs != realSizes.end(); ++rs) {
			(*rs) = 0;
		}

		// Draw lands

		uniform_int_distribution<> dx(0, GRID_WIDTH - 1), dy(0, GRID_HEIGHT - 1);

		vector<vector2u> emptyCells;
		for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
			for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
				vector2u v(x,y,emptyCells.size());
				index[x][y].x = emptyCells.size();
				emptyCells.push_back(v);
			}
		}

		for (unsigned int i = 0; i < landsCount; ++i) {

			uniform_int_distribution<> randEmpty(0, emptyCells.size() - 1);
			unsigned int k = randEmpty(gen), x = emptyCells[k].x, y = emptyCells[k].y;
			ncells[x][y] = i;
			realSizes[i]++;
			for (unsigned int u = k+1; u < emptyCells.size(); ++u) {
				emptyCells[u].it--;;
				index[emptyCells[u].x][emptyCells[u].y].x--;
			}
			emptyCells.erase(emptyCells.begin() + k);

			vector<vector2u> possibilities;
			unsigned int it;
			if(x>0 && ncells[x-1][y] == -1) {
				index[x-1][y].y = possibilities.size();
				possibilities.push_back(vector2u(x-1, y, index[x-1][y].x));
			}
			if(x<GRID_WIDTH-1 && ncells[x+1][y] == -1) {
				index[x+1][y].y = possibilities.size();
				possibilities.push_back(vector2u(x+1, y, index[x+1][y].x));
			}
			if(y>0 && ncells[x][y-1] == -1) {
				index[x][y-1].y = possibilities.size();
				possibilities.push_back(vector2u(x, y-1, index[x][y-1].x));
			}
			if(y<GRID_HEIGHT-1 && ncells[x][y+1] == -1) {
				index[x][y+1].y = possibilities.size();
				possibilities.push_back(vector2u(x, y+1, index[x][y+1].x));
			}

			for (unsigned int j = 1; j < sizes[i]; ++j) {

				if(!possibilities.empty()) {
					uniform_int_distribution<> randPossible(0, possibilities.size() - 1);
					unsigned int l = randPossible(gen), x = possibilities[l].x, y = possibilities[l].y;
					ncells[x][y] = i;
					realSizes[i]++;
					for (unsigned int u = possibilities[l].it+1; u < emptyCells.size(); ++u) {
						emptyCells[u].it--;
						index[emptyCells[u].x][emptyCells[u].y].x--;
					}
					emptyCells.erase(emptyCells.begin() + possibilities[l].it);
					for (unsigned int u = l+1; u < possibilities.size(); ++u) {
						index[possibilities[u].x][possibilities[u].y].y--;
					}
					possibilities.erase(possibilities.begin() + l);

					if(x>0 && ncells[x-1][y] == -1 && index[x-1][y].y == -1) {
						index[x-1][y].y = possibilities.size();
						possibilities.push_back(vector2u(x-1, y, index[x-1][y].x));
					}
					if(x<GRID_WIDTH-1 && ncells[x+1][y] == -1 && index[x+1][y].y == -1) {
						index[x+1][y].y = possibilities.size();
						possibilities.push_back(vector2u(x+1, y, index[x+1][y].x));
					}
					if(y>0 && ncells[x][y-1] == -1 && index[x][y-1].y == -1) {
						index[x][y-1].y = possibilities.size();
						possibilities.push_back(vector2u(x, y-1, index[x][y-1].x));
					}
					if(y<GRID_HEIGHT-1 && ncells[x][y+1] == -1 && index[x][y+1].y == -1) {
						index[x][y+1].y = possibilities.size();
						possibilities.push_back(vector2u(x, y+1, index[x][y+1].x));
					}
				}

			}

			for (unsigned int x=0; x < GRID_WIDTH; ++x) {
				for (unsigned int y=0; y < GRID_HEIGHT; ++y) {
					index[x][y].y = -1;
				}
			}

		}

		// Fill blanks
		for(unsigned int x=0; x<GRID_WIDTH; ++x) {
			for(unsigned int y=0; y<GRID_HEIGHT; ++y) {
				if(ncells[x][y] == -1) {

					vector<vector2u> neighbors;
					if(x>0 && ncells[x-1][y] != -1) neighbors.push_back(vector2u(x-1, y, 0));
					if(x<GRID_WIDTH-1 && ncells[x+1][y] != -1) neighbors.push_back(vector2u(x+1, y, 0));
					if(y>0 && ncells[x][y-1] != -1) neighbors.push_back(vector2u(x, y-1, 0));
					if(y<GRID_HEIGHT-1 && ncells[x][y+1] != -1) neighbors.push_back(vector2u(x, y+1, 0));

					if(!neighbors.empty()) {
						uniform_int_distribution<> rnd(0, neighbors.size()-1);
						unsigned int u = rnd(gen);
						unsigned int id = ncells[neighbors[u].x][neighbors[u].y];
						ncells[x][y] = id;
						realSizes[id]++;
					}

				}
			}
		}
		for(unsigned int x=GRID_WIDTH-1; x>=0 && x<GRID_WIDTH; x--) {
			for(unsigned int y=GRID_HEIGHT-1; y>=0 && y<GRID_HEIGHT; y--) {
				if(ncells[x][y] == -1) {

					vector<vector2u> neighbors;
					if(x>0 && ncells[x-1][y] != -1) neighbors.push_back(vector2u(x-1, y, 0));
					if(x<GRID_WIDTH-1 && ncells[x+1][y] != -1) neighbors.push_back(vector2u(x+1, y, 0));
					if(y>0 && ncells[x][y-1] != -1) neighbors.push_back(vector2u(x, y-1, 0));
					if(y<GRID_HEIGHT-1 && ncells[x][y+1] != -1) neighbors.push_back(vector2u(x, y+1, 0));

					if(!neighbors.empty()) {
						uniform_int_distribution<> rnd(0, neighbors.size()-1);
						unsigned int u = rnd(gen);
						unsigned int id = ncells[neighbors[u].x][neighbors[u].y];
						ncells[x][y] = id;
						realSizes[id]++;
					}

				}
			}
		}

		// Compute lands sizes
		unsigned int realLandsCount = 0;
		for(vector<unsigned int>::iterator rs = realSizes.begin(); rs != realSizes.end(); ++rs) {
			if ((*rs) != 0) realLandsCount++;
		}

		// Remove tiny lands
		for(unsigned int i = 0; i < realSizes.size(); ++i) {
			if(realSizes[i] < meanSize / 3) {

				unsigned int neighbor = 0;
				for(unsigned int x = 0; x < GRID_WIDTH; ++x) {
					for(unsigned int y = 0; y < GRID_HEIGHT; ++y) {
						if(ncells[x][y] == i) {
							if(x>0 && ncells[x-1][y] != i) {
								neighbor = ncells[x-1][y];
							}
							if(x<GRID_WIDTH-1 && ncells[x+1][y] != i) {
								neighbor = ncells[x+1][y];
							}
							if(y>0 && ncells[x][y-1] != i) {
								neighbor = ncells[x][y-1];
							}
							if(y<GRID_HEIGHT-1 && ncells[x][y+1] != i) {
								neighbor = ncells[x][y+1];
							}
						}
					}
				}

				for(unsigned int x = 0; x < GRID_WIDTH; ++x) {
					for(unsigned int y = 0; y < GRID_HEIGHT; ++y) {
						if(ncells[x][y] == i) ncells[x][y] = neighbor;
					}
				}

				realSizes[neighbor] += realSizes[i];
				realSizes[i] = 0;

			}
		}

		// Debug display
		/*char c[72]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','&','$','*','*','*','*','*','*','*','*'};
		for(unsigned int y=0; y<GRID_HEIGHT; ++y) {
			for(unsigned int x=0; x<GRID_WIDTH; ++x) {
				int ch = ncells[x][y];
				if (ch == -1) cout << ".";
				else cout << c[ch];
			}
			cout << endl;
		}*/

		return realSizes;

	}

	unsigned int Game::util_total(vector<unsigned int>& v) {
		unsigned int t = 0;
		for (vector<unsigned int>::const_iterator e = v.begin(); e != v.end(); ++e) {
			t += (*e);
		}
		return t;
	}

	unsigned int Game::util_min(vector<unsigned int>& v) {
		unsigned int pos = 0, val = v[0];
		for (unsigned int i = 1; i < v.size(); ++i) {
			if (v[i] < val) {
				pos = i;
				val = v[i];
			}
		}
		return pos;
	}

	unsigned int Game::util_max(vector<unsigned int>& v) {
		unsigned int pos = 0, val = v[0];
		for (unsigned int i = 1; i < v.size(); ++i) {
			if (v[i] > val) {
				pos = i;
				val = v[i];
			}
		}
		return pos;
	}

	unsigned int Game::getDefense(shared_ptr<Land> land) const {
		//TODOO
		return land->getSoldiersNumber();
	}

	unsigned int Game::getAttack(shared_ptr<Land> attacker, shared_ptr<Land> defender) const {
		return attacker->getSoldiersNumber();
	}

	bool Game::areConnected(shared_ptr<Land> landOne, shared_ptr<Land> landTwo) const {
		return getConnection(landOne, landTwo) > 0;
	}

	unsigned int Game::getConnection(shared_ptr<Land> landOne, shared_ptr<Land> landTwo) const {
		for(auto n : landOne->getNeighborLands()) {
			if(n == landTwo) return 2;
		}
		return 0;
		//TODO : water
	}

	unsigned int Game::getCurrentPlayer() const {
        return currentPlayer;
    }

    void Game::nextPlayer() {
    	currentPlayer = (currentPlayer==players.size()-1)?0:currentPlayer+1;
    	setCurrentStep(STEP_REINFORCEMENT);
    	if(!players[currentPlayer]->isAlive()) nextPlayer();
    }

    Step Game::getCurrentStep () const {
        return this->currentStep;
    }
    
    void Game::setCurrentStep (Step step){
        this->currentStep=step;
    }

    ItemType Game::getActivatedItem () const {
        return this->activatedItem;
    }
    
    void Game::setActivatedItem (ItemType item){
        this->activatedItem=item;
    }

	vector< shared_ptr<Land> > Game::getLands() const {
		return lands;
	}

	vector< shared_ptr<Player> > Game::getPlayers() const {
		return players;
	}
        
    Cell& Game::getCell(unsigned int x, unsigned int y) {
    	return cells[x][y];
    }
             

}
