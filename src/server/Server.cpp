#include "Server.hpp"

using namespace std;

namespace server {

	Server::Server() {
		state = STATE_IDLE;
		joinedPlayers = 0;
	}

	void Server::run() {
		try {
			struct MHD_Daemon *d;
			d = MHD_start_daemon(
				MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
				8080,
				NULL, NULL,
				&main_handler, (void*) this,
				MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL,
				MHD_OPTION_END);
			if (d == NULL)
				return;
			(void) getc(stdin);
			MHD_stop_daemon(d);
		}
		catch(exception& e) {
			cerr << "Exception: " << e.what() << endl;
		}
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

    void Server::createGame(unsigned int numPlayers) {
    	game = make_shared<state::Game>();
		game->init(numPlayers);
		state = STATE_INIT;
		joinedPlayers = 1;
		game->generateMap();
		state = STATE_GAME;
    }

    int Server::queryService(string& out, const string& in, const string& url, const string& method) {
    	Json::Value jdata;
    	istringstream iss(in);
    	iss >> jdata;
    	if (url == "/status/" && method == "GET") {
    		// Service : get_status
    		if (state == STATE_IDLE) return 503;
    		else if (state == STATE_INIT) {
    			if (joinedPlayers < game->getPlayers().size()) return 204;
    			else return 403;
    		}
    		else {
    			Json::Value jret;
    			jret["playerId"] = joinedPlayers;
    			jret["state"] = game->toJSON();
    			ostringstream oss;
    			oss << jret;
    			out = oss.str();
    			return 200;
    		}
    	}
    	else if (url == "/status" && method == "PUT") {
    		// Service : create_game
    		if (state == STATE_IDLE) {
	    		createGame(jdata["numPlayers"].asInt());
	    		return 200;
	    	}
	    	else {
	    		return 403;
	    	}
    	}
    	else if ((url == "/commands/action/" || url == "/commands/choice" || url == "/commands/attack/" || url == "/commands/move/") && method == "PUT") {
    		// Service : push_command (action)
    		if (state != STATE_GAME) return 503;
    		shared_ptr<engine::Command> cmd = engine->commandFromJSON(jdata);
    		if (engine->isAllowed(cmd)) {
    			engine->pushCommand(cmd);
    			engine->flushCommands();
    			Json::Value jret = (int) history.size();
    			history.push_back(cmd);
    			ostringstream oss;
    			oss << jret;
    			out = oss.str();
    			return 200;
    		}
    		else {
    			return 403;
    		}
    	}
    	else if (url == "/commands/" && method == "GET") {
    		unsigned int index = jdata.asInt();
    		if (state != STATE_GAME) return 503;
    		if (index >= history.size()) return 400;
    		Json::Value cmds;
    		for (int i = index; i < history.size(); i++) {
    			cmds[i-index] = history[i]->toJSON();
    		}
    		ostringstream oss;
    		oss << cmds;
    		out = oss.str();
    		return 200;
    	}
    	else return 400;
    }

    // Fonction pour gérer les données imporantes en upload (non implanté ici)
	static int
	post_iterator(void *cls,
	        enum MHD_ValueKind kind,
	        const char *key,
	        const char *filename,
	        const char *content_type,
	        const char *transfer_encoding,
	        const char *data, uint64_t off, size_t size) 
	{
	    return MHD_NO;
	}

	// Détruit les données d'une requête
	static void
	request_completed (void *cls, struct MHD_Connection *connection,
	                   void **con_cls, enum MHD_RequestTerminationCode toe)
	{
	  Request *request = (Request*)*con_cls;
	  if (request) {
	      delete request;
	      *con_cls = nullptr;
	  }
	}

	// Gestionnaire principal
	static int
	main_handler (void *cls,      
	          struct MHD_Connection *connection,
	          const char *url, // 
	          const char *method,
	          const char *version,
	          const char *upload_data, size_t *upload_data_size, void **ptr) 
	{
	    // Données pour une requête (en plusieurs appels à cette fonction)
	    Request *request = (Request*)*ptr;

	    // Premier appel pour cette requête
	    if (!request) { 
	        request = new Request();
	        if (!request) {
	            return MHD_NO;
	        }
	        *ptr = request;
	        if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
	         || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
	            request->pp = MHD_create_post_processor(connection,1024,&post_iterator,request);
	            if (!request->pp) {
	                cerr << "Failed to setup post processor for " << url << endl;
	                return MHD_NO;
	            }
	        }
	        return MHD_YES;
	    }    
	    
	    // Cas où il faut récupérer les données envoyés par l'utilisateur
	    if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
	     || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
	        MHD_post_process(request->pp,upload_data,*upload_data_size);
	        if (*upload_data_size != 0) {
	            request->data = upload_data;
	            *upload_data_size = 0;
	            return MHD_YES;
	        }    
	    }

	    int status;
	    string response;
	    try {

	        Server *manager = (Server*) cls;
	        status = manager->queryService(response,request->data,url,method);
	    }
	    catch(ServiceException& e) {
	        status = e.status();
	        response = e.what();
	        response += "\n";
	    }
	    catch(exception& e) {
	        status = 500;
	        response = e.what();
	        response += "\n";
	    }
	    catch(...) {
	        status = 500;
	        response = "Unknown exception\n";
	    }

	    struct MHD_Response *mhd_response;
	    mhd_response = MHD_create_response_from_buffer(response.size(),(void *)response.c_str(),MHD_RESPMEM_MUST_COPY);
	    if (strcmp(method,MHD_HTTP_METHOD_GET) == 0) {
	        MHD_add_response_header(mhd_response,"Content-Type","application/json; charset=utf-8");
	    }
	    int ret = MHD_queue_response(connection, status, mhd_response);
	    MHD_destroy_response(mhd_response);
	    return ret;
	}

}
