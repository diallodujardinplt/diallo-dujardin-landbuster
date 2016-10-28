#include "Engine.hpp"

using namespace std;

namespace engine {
	
	Engine::Engine() {}

	Engine& Engine::getInstance() {
		static Engine engine;
		return engine;
	}

	void Engine::pushCommand(Command command) {
		commandQueue.push(command);
	}

	void Engine::flushCommands() {
		//.
	}

}
