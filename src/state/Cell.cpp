#include "Cell.hpp"

using namespace std;

namespace state {

	Cell::Cell() {
	}

	Json::Value Cell::toJSON() const {
    	Json::Value jcell;
    	jcell["x"] = position.x;
    	jcell["y"] = position.y;
    	jcell["land"] = land->getId();
    	return jcell;
    }

}
