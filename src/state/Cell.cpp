#include "Cell.hpp"

using namespace std;

namespace state {

	Cell::Cell(sf::Vector2u position, bool borderTop, bool borderBottom, bool borderLeft, bool borderRight) {
		this->position = position;
		this->borderTop = borderTop;
		this->borderBottom = borderBottom;
		this->borderLeft = borderLeft;
		this->borderRight = borderRight;
	}

}
