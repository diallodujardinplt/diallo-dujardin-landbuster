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
				color.r = ((float) color.r) * 200.0/255.0;
				color.g = ((float) color.g) * 255.0/255.0;
				color.b = ((float) color.b) * 200.0/255.0;
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

		if(borderTop && y>0) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x, CELL_HEIGHT*cell.position.y-1));
			border.setSize(sf::Vector2f(CELL_WIDTH, BORDER_WIDTH));
			border.setFillColor((game.getCell(x, y-1).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

		if(borderBottom && y<GRID_HEIGHT-1) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x, CELL_HEIGHT*cell.position.y+CELL_HEIGHT-1));
			border.setSize(sf::Vector2f(CELL_WIDTH, BORDER_WIDTH));
			border.setFillColor((game.getCell(x, y+1).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

		if(borderLeft && x>0) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x-1, CELL_HEIGHT*cell.position.y));
			border.setSize(sf::Vector2f(BORDER_WIDTH, CELL_HEIGHT));
			border.setFillColor((game.getCell(x-1, y).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

		if(borderRight && x<GRID_WIDTH-1) {
			sf::RectangleShape border;
			border.setPosition(sf::Vector2f(CELL_WIDTH*cell.position.x+CELL_WIDTH-1, CELL_HEIGHT*cell.position.y));
			border.setSize(sf::Vector2f(BORDER_WIDTH, CELL_HEIGHT));
			border.setFillColor((game.getCell(x+1, y).land->getType() == state::LAND_WATER)?coastBorder:landBorder);
			window.draw(border);
		}

	}

	sf::Vector2u Renderer::getMeanPos(state::Game& game, shared_ptr<state::Land> land, const vector<sf::Vector2u>& geometry, unsigned int offset) {

		sf::Vector2f mean(0.0, 0.0);
		for (vector<sf::Vector2u>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {
			state::Cell& cell = game.getCell(cell_it->x, cell_it->y);
			mean.x += cell.position.x;
			mean.y += cell.position.y;
		}
		mean.x /= ((float) geometry.size());
		mean.y /= ((float) geometry.size());

		sf::Vector2u umean(mean.x, mean.y + offset);
		bool inLand = false;

		for (vector<sf::Vector2u>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {
			state::Cell& cell = game.getCell(cell_it->x, cell_it->y);
			if (cell.position.x == umean.x && cell.position.y == umean.y)
				inLand = true;
		}

		if (!inLand) {
			if (offset < geometry.size()) umean = geometry[offset];
			else umean = geometry[geometry.size() -1];
		}

		return umean;

	}

	void Renderer::renderSoldiersNumber(sf::RenderWindow& window, state::Game& game, shared_ptr<state::Land> land, const vector<sf::Vector2u>& geometry) {

		sf::Vector2u mean = getMeanPos(game, land, geometry, 0);
		sf::Vector2f pos(CELL_WIDTH * mean.x, CELL_HEIGHT * mean.y);
		sf::Text text;
		text.setPosition(pos);
		text.setFont(font);
		ostringstream oss;
		oss << land->getSoldiersNumber();
		text.setString(oss.str().c_str());
		text.setCharacterSize(20);
		text.setColor(sf::Color(255, 130, 0));
		window.draw(text);

	}

	void Renderer::renderItem(sf::RenderWindow& window, state::Game& game, shared_ptr<state::Land> land, const vector<sf::Vector2u>& geometry) {

		if (land->getItem() == state::ITEM_NONE) return;

		sf::Vector2u mean = getMeanPos(game, land, geometry, 3);
		sf::Vector2f pos(CELL_WIDTH * mean.x, CELL_HEIGHT * mean.y);
		sf::Text text;
		text.setPosition(pos);
		text.setFont(font);
		
		if (land->getItem() == state::ITEM_FAT_BONUS) text.setString("???");
		else text.setString("?");

		text.setCharacterSize(30);
		text.setColor(sf::Color(255, 160, 10));
		window.draw(text);

	}

	void Renderer::renderLand(sf::RenderWindow& window, state::Game& game, shared_ptr<state::Land> land) {

		if (land->getType() == state::LAND_NONE) return;
		const vector<sf::Vector2u>& geometry = land->getGeometry();
		if (geometry.begin() == geometry.end()) return;

		for (vector<sf::Vector2u>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {

			state::Cell& cell = game.getCell((*cell_it).x, (*cell_it).y);
			renderCell(window, game, land, geometry, cell);
			
		}

		if (land->getType() != state::LAND_WATER) {
			renderSoldiersNumber(window, game, land, geometry);
			renderItem(window, game, land, geometry);
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
