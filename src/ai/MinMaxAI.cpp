#include "MinMaxAI.hpp"

using namespace std;

namespace ai {

	MinMaxAI::MinMaxAI(unsigned int playerId) : AI(playerId) {}

	shared_ptr<engine::Command> MinMaxAI::run(shared_ptr<state::Game> game) {

		unsigned int depth = 1;

		unsigned int maxVal = 0;
		shared_ptr<engine::Command> bestShot = nullptr;

		for(auto possibility : getPossibilities(game, playerId)) {
			/*string s;
			cin >> s;
			if(s == "exit") exit(EXIT_SUCCESS);

			cout << "possibility : " << possibility->toString() << endl;*/

			shared_ptr<state::Game> possibleGame = make_shared<state::Game>(game);
			shared_ptr<engine::Engine> possibleEngine = make_shared<engine::Engine>(possibleGame);
			possibleEngine->pushCommand(possibility);
			possibleEngine->flushCommands();
			unsigned int val = min(possibleGame, possibleEngine, depth);
			if (val > maxVal) {
				maxVal = val;
				bestShot = possibility;
			}
		}

		return bestShot;

	}

	unsigned int MinMaxAI::min(shared_ptr<state::Game> game, shared_ptr<engine::Engine> engine, unsigned int depth) {

		/*string s;
		cin >> s;
		if(s == "exit") exit(EXIT_SUCCESS);*/

		if (depth == 0 || game->isFinished()) {
			return eval(game, depth);
		}

		unsigned int minVal = 0xFFFFFFFF;

		for(auto possibility : getPossibilities(game, game->getCurrentPlayer())) {
			shared_ptr<state::Game> possibleGame = make_shared<state::Game>(game);
			shared_ptr<engine::Engine> possibleEngine = make_shared<engine::Engine>(possibleGame);
			possibleEngine->pushCommand(possibility);
			possibleEngine->flushCommands();
			unsigned int val = max(possibleGame, possibleEngine, depth-1);
			if (val < minVal) {
				minVal = val;
			}
		}

		return minVal;

	}

	unsigned int MinMaxAI::max(shared_ptr<state::Game> game, shared_ptr<engine::Engine> engine, unsigned int depth) {

		/*string s;
		cin >> s;
		if(s == "exit") exit(EXIT_SUCCESS);*/

		if (depth == 0 || game->isFinished()) {
			return eval(game, depth);
		}

		unsigned int maxVal = 0;

		for(auto possibility : getPossibilities(game, game->getCurrentPlayer())) {
			shared_ptr<state::Game> possibleGame = make_shared<state::Game>(game);
			shared_ptr<engine::Engine> possibleEngine = make_shared<engine::Engine>(possibleGame);
			possibleEngine->pushCommand(possibility);
			possibleEngine->flushCommands();
			unsigned int val = min(possibleGame, possibleEngine, depth-1);
			if (val > maxVal) {
				maxVal = val;
			}
		}

		return maxVal;

	}

	unsigned int MinMaxAI::eval(shared_ptr<state::Game> game, unsigned int distance) {
		if (game->isFinished()) {
			if (game->getPlayers()[playerId]->isAlive()) {
				return 0xFFFFFF - distance;
			}
			else {
				return 0;
			}
		}

		unsigned int portsCount = 0;
		for(auto land : game->getPlayerLands(playerId)) {
			if (land->hasPorts())
				portsCount++;
		}

		return game->getSoldiersCount(game->getPlayers()[playerId]) + 5 * portsCount;
	}

}