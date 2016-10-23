#include "Renderer.hpp"

using namespace std;

namespace render {

	Renderer::Renderer() {
		waterRect.setSize(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
		waterRect.setFillColor(sf::Color(0, 0, 128));
		portRect.setSize(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
		portRect.setFillColor(sf::Color(128, 64, 0));
	}

	Renderer::~Renderer() {
	}

	Renderer& Renderer::getInstance() {
		static Renderer renderer;
		return renderer;
	}

	void Renderer::init() {
		loadTextures();
		font.loadFromFile("res/font.ttf");
	}

	void Renderer::loadTextures() {
		landTextures[state::LAND_DESERT].loadFromFile("res/textures/desert.jpg");
		landTextures[state::LAND_FOREST].loadFromFile("res/textures/forest.jpg");
		landTextures[state::LAND_MOUNTAIN].loadFromFile("res/textures/mountain.jpg");
		landTextures[state::LAND_COASTAL].loadFromFile("res/textures/coastal.jpg");
	}

	void Renderer::renderCell(sf::RenderWindow& window, state::Game& game, shared_ptr<state::Land> land, const vector<sf::Vector2u>& geometry, state::Cell& cell) {
		unsigned int x = cell.position.x, y = cell.position.y;
		sf::Vector2f pos(CELL_WIDTH*x, CELL_HEIGHT*y);

		bool borderTop = (cell.position.y>0)?(game.getCell(x, y-1).land != land):false;
		bool borderBottom = (cell.position.y<GRID_HEIGHT-1)?(game.getCell(x, y+1).land != land):false;
		bool borderLeft = (cell.position.x>0)?(game.getCell(x-1, y).land != land):false;
		bool borderRight = (cell.position.x<GRID_WIDTH-1)?(game.getCell(x+1, y).land != land):false;

		sf::Sprite sprite;

		if(land->getType() != state::LAND_WATER) {
			// Land

			if(land->getType() != state::LAND_MEADOW) {
				sprite.setPosition(pos);
				sprite.setTexture(landTextures[land->getType()]);
				sprite.setTextureRect(sf::IntRect((CELL_WIDTH*(x-(*(geometry.begin())).x))%600, (CELL_HEIGHT*(y-(*(geometry.begin())).y))%400, CELL_WIDTH, CELL_HEIGHT));
				if(land->getOwner()) sprite.setColor(land->getOwner()->getColor());
				window.draw(sprite);
			}
			else {
				sf::RectangleShape meadowRect(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
				sf::Color color = land->getOwner()?land->getOwner()->getColor():sf::Color(255, 255, 255);
				color.r *= 200;
				color.g *= 255;
				color.b *= 200;
				meadowRect.setFillColor(color);
				meadowRect.setPosition(pos);
				window.draw(meadowRect);
			}

			renderLandBorders(window, game, cell, x, y, pos, borderTop, borderBottom, borderLeft, borderRight);

		}

		else {
			// Water

			bool portCell = false;

			if(borderTop && game.getCell(x, y-1).land->hasPorts() && x%4 == 0)
				portCell = true;

			if(borderBottom && game.getCell(x, y+1).land->hasPorts() && x%4 == 0)
				portCell = true;

			if(borderLeft && game.getCell(x-1, y).land->hasPorts() && y%4 == 0)
				portCell = true;

			if(borderRight && game.getCell(x+1, y).land->hasPorts() && y%4 == 0)
				portCell = true;

			if(portCell) {
				portRect.setPosition(pos);
				window.draw(portRect);
			}
			else {
				waterRect.setPosition(pos);
				window.draw(waterRect);
			}
			
		}	
	}

	void Renderer::renderLandBorders(sf::RenderWindow& window, state::Game& game, state::Cell& cell, unsigned int x, unsigned int y, sf::Vector2f pos, bool borderTop, bool borderBottom, bool borderLeft, bool borderRight) {

		sf::Color landBorder(0, 0, 0);
		sf::Color coastBorder(255, 200, 0);

		if(borderTop) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x, CELL_HEIGHT*cell.position.y-1));
			border.setSize(sf::Vector2f(CELL_WIDTH, BORDER_WIDTH));
			border.setFillColor((game.getCell(x, y-1).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

		if(borderBottom) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x, CELL_HEIGHT*cell.position.y+CELL_HEIGHT-1));
			border.setSize(sf::Vector2f(CELL_WIDTH, BORDER_WIDTH));
			border.setFillColor((game.getCell(x, y+1).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

		if(borderLeft) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x-1, CELL_HEIGHT*cell.position.y));
			border.setSize(sf::Vector2f(BORDER_WIDTH, CELL_HEIGHT));
			border.setFillColor((game.getCell(x-1, y).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

		if(borderRight) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x+CELL_WIDTH-1, CELL_HEIGHT*cell.position.y));
			border.setSize(sf::Vector2f(BORDER_WIDTH, CELL_HEIGHT));
			border.setFillColor((game.getCell(x+1, y).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

	}

	void Renderer::renderSoldiersNumber(sf::RenderWindow& window, state::Game& game, shared_ptr<state::Land> land, const vector<sf::Vector2u>& geometry) {

		float moyX = 0.0, moyY = 0.0;
		for (vector<sf::Vector2u>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {
			state::Cell& cell = game.getCell(cell_it->x, cell_it->y);
			moyX += cell.position.x;
			moyY += cell.position.y;
		}
		moyX /= ((float) geometry.size());
		moyY /= ((float) geometry.size());
		unsigned int moyXu = moyX, moyYu = moyY;
		bool inLand = false;
		for (vector<sf::Vector2u>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {
			state::Cell& cell = game.getCell(cell_it->x, cell_it->y);
			if (cell.position.x == moyXu && cell.position.y == moyYu) {
				inLand = true;
			}
		}
		if (!inLand) {
			moyXu = (*(geometry.begin())).x;
			moyYu = (*(geometry.begin())).y;
		}
		moyXu *= CELL_WIDTH;
		moyYu *= CELL_HEIGHT;
		sf::Text text;
		text.setPosition(sf::Vector2f(moyXu, moyYu));
		text.setFont(font);
		ostringstream oss;
		oss << land->getSoldiersNumber();
		text.setString(oss.str().c_str());
		text.setCharacterSize(30);
		text.setColor(sf::Color(255, 130, 0));
		text.setStyle(sf::Text::Bold);
		window.draw(text);

	}

	void Renderer::renderLand(sf::RenderWindow& window, state::Game& game, shared_ptr<state::Land> land) {

		if (land->getType() == state::LAND_NONE) return;
		const vector<sf::Vector2u>& geometry = land->getGeometry();
		if (geometry.begin() == geometry.end()) return;

		for (vector<sf::Vector2u>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {

			state::Cell& cell = game.getCell((*cell_it).x, (*cell_it).y);
			renderCell(window, game, land, geometry, cell);
			renderSoldiersNumber(window, game, land, geometry);
			
		}

	}

	void Renderer::render(sf::RenderWindow& window) {

		state::Game& game = state::Game::getInstance();

		vector< shared_ptr<state::Land> > lands = game.getLands();
		if (lands.begin() == lands.end()) return;
		for (vector< shared_ptr<state::Land> >::const_iterator land_it = lands.begin(); land_it != lands.end(); ++land_it) {
			
			std::shared_ptr<state::Land> land = *land_it;
			renderLand(window, game, land);
			
		}

	}

}
