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
	}

	void Renderer::loadTextures() {
		// TODO
	}

	void Renderer::render(sf::RenderWindow& window) {

		state::Game& game = state::Game::getInstance();

		if (game.getLands().begin() == game.getLands().end()) return;
		for (vector<shared_ptr<state::Land>>::const_iterator land = game.getLands().begin(); land != game.getLands().end(); ++land) {
			
			if ((*land)->getGeometry().begin() == (*land)->getGeometry().end()) return;
			for (vector<sf::Vector2u>::const_iterator cell = (*land)->getGeometry().begin(); cell != (*land)->getGeometry().end(); ++cell) {

				
				
			}
			
		}

	}

}
