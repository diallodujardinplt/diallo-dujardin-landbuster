#include "Renderer.h"

using namespace std;

namespace render {

	void Renderer::render(sf::RenderWindow& window) {

		Game& game = state::Game::getInstance();

		for (vector<shared_ptr<Land>>::const_iterator land = game.getLands().begin(); land != game.getLands().end(); ++land) {
			for (vector<sf::Vector2u>::const_iterator cell = land.getGeometry().begin(); cell != land.getGeometry().end(); ++cell) {

				
				
			}
		}

	}

}
