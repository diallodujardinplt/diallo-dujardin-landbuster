#include "Renderer.hpp"

using namespace std;

namespace render {

	Renderer::Renderer() {
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
		landTextures[state::LAND_DESERT].loadFromFile("res/textures/sm_desert.jpg");
		landTextures[state::LAND_FOREST].loadFromFile("res/textures/sm_forest.jpg");
		landTextures[state::LAND_MOUNTAIN].loadFromFile("res/textures/sm_mountain.jpg");
		landTextures[state::LAND_COASTAL].loadFromFile("res/textures/sm_coastal.jpg");
		landTextures[state::LAND_MEADOW].create(9, 9);
	}

	void Renderer::render(sf::RenderWindow& window) {

		state::Game& game = state::Game::getInstance();

		sf::Sprite sprite;

		vector< shared_ptr<state::Land> > lands = game.getLands();
		if (lands.begin() == lands.end()) return;
		for (vector< shared_ptr<state::Land> >::const_iterator land_it = lands.begin(); land_it != lands.end(); ++land_it) {
			
			std::shared_ptr<state::Land> land = *land_it;
			if (land->getType() == state::LAND_NONE) return;
			const vector<state::Cell>& geometry = land->getGeometry();
			if (geometry.begin() == geometry.end()) return;

			// Draw cells with borders
			for (vector<state::Cell>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {

				state::Cell cell = *cell_it;
				sprite.setPosition(9*cell.position.x, 9*cell.position.y);
				sprite.setTexture(landTextures[land->getType()]);
				if(land->getOwner()) sprite.setColor(land->getOwner()->getColor());
				window.draw(sprite);

				if(cell.borderTop) {
					sf::RectangleShape border;
					border.setPosition(sf::Vector2f(9*cell.position.x, 9*cell.position.y-1));
					border.setSize(sf::Vector2f(9, 3));
					border.setFillColor(sf::Color(0,0,0));
					window.draw(border);
				}

				if(cell.borderBottom) {
					sf::RectangleShape border;
					border.setPosition(sf::Vector2f(9*cell.position.x, 9*cell.position.y+9-1));
					border.setSize(sf::Vector2f(9, 3));
					border.setFillColor(sf::Color(0,0,0));
					window.draw(border);
				}

				if(cell.borderLeft) {
					sf::RectangleShape border;
					border.setPosition(sf::Vector2f(9*cell.position.x-1, 9*cell.position.y));
					border.setSize(sf::Vector2f(3, 9));
					border.setFillColor(sf::Color(0,0,0));
					window.draw(border);
				}

				if(cell.borderRight) {
					sf::RectangleShape border;
					border.setPosition(sf::Vector2f(9*cell.position.x+9-1, 9*cell.position.y));
					border.setSize(sf::Vector2f(3, 9));
					border.setFillColor(sf::Color(0,0,0));
					window.draw(border);
				}

			}

			// Write soldiers number
			float moyX = 0.0, moyY = 0.0;
			for (vector<state::Cell>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {
				state::Cell cell = *cell_it;
				moyX += cell.position.x;
				moyY += cell.position.y;
			}
			moyX /= ((float) geometry.size());
			moyY /= ((float) geometry.size());
			unsigned int moyXu = moyX, moyYu = moyY;
			bool inLand = false;
			for (vector<state::Cell>::const_iterator cell_it = geometry.begin(); cell_it != geometry.end(); ++cell_it) {
				state::Cell cell = *cell_it;
				if (cell.position.x == moyX && cell.position.y == moyY) {
					inLand = true;
				}
			}
			if (!inLand) {
				moyXu = (*(geometry.begin())).position.x;
				moyYu = (*(geometry.begin())).position.y;
			}
			moyXu *= 9;
			moyYu *= 9;
			sf::Text text;
			text.setPosition(sf::Vector2f(moyXu, moyYu));
			text.setFont(font);
			ostringstream oss;
			oss << land->getSoldiersNumber();
			text.setString(oss.str().c_str());
			text.setCharacterSize(20);
			text.setColor(sf::Color(0, 0, 0));
			text.setStyle(sf::Text::Bold);
			window.draw(text);
			
		}

	}

}
