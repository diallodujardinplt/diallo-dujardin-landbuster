#ifndef ENGINE__ENGINE__HPP
#define ENGINE__ENGINE__HPP

#include "../common.hpp"
#include "state.h"
#include "Engine.h"

namespace engine {
	void engineThreadFunction(std::shared_ptr<Engine> engine);
}

#endif