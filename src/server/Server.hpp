#ifndef SERVER__SERVER__HPP
#define SERVER__SERVER__HPP

#include "../common.hpp"
#include "Server.h"

namespace server {

	class Request {
	public:
	    struct MHD_PostProcessor *pp = nullptr;
	    std::string data;
	    ~Request() {
	        if (pp) MHD_destroy_post_processor (pp);
	    }
	};

	static int
	post_iterator(void *cls,
	        enum MHD_ValueKind kind,
	        const char *key,
	        const char *filename,
	        const char *content_type,
	        const char *transfer_encoding,
	        const char *data, uint64_t off, size_t size) ;

	static void
	request_completed (void *cls, struct MHD_Connection *connection,
	                   void **con_cls, enum MHD_RequestTerminationCode toe);

	static int
	main_handler (void *cls,      
	          struct MHD_Connection *connection,
	          const char *url, // 
	          const char *method,
	          const char *version,
	          const char *upload_data, size_t *upload_data_size, void **ptr) ;

}

#endif