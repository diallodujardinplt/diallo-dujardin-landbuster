#include "ServiceException.hpp"

using namespace std;

namespace server {

	ServiceException::ServiceException(int status, string msg) {
		this->_status = status;
		this->msg = msg;
	}

	int ServiceException::status() const {
		return _status;
	}

	const char* ServiceException::what() const {
		return msg.c_str();
	}

}
