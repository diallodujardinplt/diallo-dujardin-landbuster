#ifndef RENDER__CLIENT__HPP
#define RENDER__CLIENT__HPP

#include "../common.hpp"
#include "Client.h"

namespace render {
	void clientThreadFunction(std::shared_ptr<Client> client);
}

#endif
