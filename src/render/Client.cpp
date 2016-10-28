#include "Client.hpp"

using namespace std;

namespace render {

	Client::Client() {}

	Client::~Client() {}

	Client& Client::getInstance() {
		static Client client;
		return client;
	}

	shared_ptr<state::Player> Client::getPlayer() const {
		return player;
	}

	void Client::setPlayer(shared_ptr<state::Player> player) {
		this->player = player;
	}

	shared_ptr<state::Land> Client::getSelectedInfoLand() const {
		return selectedInfoLand;
	}

	void Client::setSelectedInfoLand(shared_ptr<state::Land> land) {
		selectedInfoLand = land;
	}

	shared_ptr<state::Land> Client::getSelectedLand() const {
		return selectedLand;
	}

	void Client::setSelectedLand(shared_ptr<state::Land> land) {
		selectedLand = land;
	}

	void Client::run() {

		sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_WIDTH + 200, GRID_HEIGHT * CELL_HEIGHT, 32), "Land Buster");

		state::Game& game = state::Game::getInstance();

		render::Renderer& renderer = render::Renderer::getInstance();
		renderer.init();

		sf::Clock framerateClock;

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			mousePos.x /= CELL_WIDTH;
			mousePos.y /= CELL_HEIGHT;
			if(mousePos.x >= 0 && mousePos.x < GRID_WIDTH && mousePos.y >= 0 && mousePos.y < GRID_HEIGHT) {
				shared_ptr<state::Land> target = game.getCell(mousePos.x, mousePos.y).land;

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					setSelectedLand(target);

				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

					setSelectedInfoLand(target);

				}
			}

			sf::Time framerateElapsed = framerateClock.getElapsedTime();
			if (framerateElapsed.asMilliseconds() > 30) {
				window.clear();
				
				renderer.render(window);

				window.display();

				framerateClock.restart();
			}
			
		}

	}

	void Client::sendCommand(engine::Command command) {
		engine::Engine::getInstance().pushCommand(command);
	}

}
