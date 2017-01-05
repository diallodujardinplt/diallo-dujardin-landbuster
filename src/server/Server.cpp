#include "Server.hpp"

using namespace std;

namespace server {

	Server::Server() {

	}

	void Server::run() {

	}

	void Server::pushCommand (shared_ptr<engine::Command> command) {
		history.push_back(command);
	}

    vector< shared_ptr<engine::Command> > Server::getCommands (unsigned int from) const {
    	vector< shared_ptr<engine::Command> > result;
    	if (from < history.size())
    		for (unsigned int i = from; i < history.size(); i++)
    			result.push_back(history[i]);
    	return result;
    }

}
