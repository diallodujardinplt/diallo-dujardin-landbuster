#include "MinMaxAI.hpp"

using namespace std;

namespace ai {

	MinMaxAI::MinMaxAI(shared_ptr<state::Player> player) : AI(player) {}

	void MinMaxAI::run(shared_ptr<state::Game> game, shared_ptr<engine::Engine> engine) {

		unsigned int depth = 8;

		unsigned int maxVal = 0;
		shared_ptr<engine::Command> bestShot = nullptr;

		for(auto possibility : getPossibilities(game, player->getId())) {
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

		engine->pushCommand(bestShot);
		engine->flushCommands();

	}

	unsigned int MinMaxAI::min(shared_ptr<state::Game> game, shared_ptr<engine::Engine> engine, unsigned int depth) {

		if (depth == 0 || game->isFinished()) {
			return eval(game, depth);
		}

		unsigned int minVal = 0xFFFFFFFF;

		for(auto possibility : getPossibilities(game, player->getId())) {
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

		if (depth == 0 || game->isFinished()) {
			return eval(game, depth);
		}

		unsigned int maxVal = 0;

		for(auto possibility : getPossibilities(game, player->getId())) {
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
			if (game->getPlayers()[player->getId()]->isAlive()) {
				return 0xFFFFFF - distance;
			}
			else {
				return 0;
			}
		}

		return game->getSoldiersCount(game->getPlayers()[player->getId()]);
	}

}