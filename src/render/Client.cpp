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
		cout << "land ";
		if(land) cout << land->getId();
		else cout << "null";
		cout << endl;
		selectedInfoLand = land;
	}

	shared_ptr<state::Land> Client::getSelectedLand() const {
		return selectedLand;
	}

	void Client::setSelectedLand(shared_ptr<state::Land> land) {
		cout << "select land ";
		if(land) cout << land->getId();
		else cout << "null";
		cout << endl;
		selectedLand = land;
	}

	void Client::run() {

		sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_WIDTH + 200, GRID_HEIGHT * CELL_HEIGHT, 32), "Land Buster");

		state::Game& game = state::Game::getInstance();

		render::Renderer& renderer = render::Renderer::getInstance();
		renderer.init();

		sf::Clock framerateClock, clickClock;

		while (window.isOpen()) {
			player = game.getPlayers()[game.getCurrentPlayer()];

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			mousePos.x /= CELL_WIDTH;
			mousePos.y /= CELL_HEIGHT;
			if(mousePos.x > CELL_WIDTH/2 && mousePos.x < GRID_WIDTH && mousePos.y > CELL_HEIGHT/2 && mousePos.y < GRID_HEIGHT) {
				shared_ptr<state::Land> target = game.getCell(mousePos.x, mousePos.y).land;

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickClock.getElapsedTime().asMilliseconds() >= 500) {

					cout << "click on land " << target->getId() << endl;

					if (!player->getHeadquarters()) {
						sendCommand(engine::Command(engine::COMMAND_CHOOSE_HEADQUARTERS, player->getId(), target->getId(), -1));
					}
					else if (game.getCurrentStep()==state::STEP_REINFORCEMENT) {
						sendCommand(engine::Command(engine::COMMAND_CHOOSE_REINFORCEMENT, player->getId(), target->getId(), -1));
					}
					else if (game.getCurrentStep()==state::STEP_ACTION) {
						if(!selectedLand) {
							setSelectedLand(target);
						}
						else {
							if(selectedLand == target) {
								engine::Command cmd(engine::COMMAND_BUILD_PORT, player->getId(), target->getId(), -1);
								if(engine::Engine::getInstance().isAllowed(cmd)) sendCommand(cmd);
								setSelectedLand(nullptr);
							}
							else {
								engine::Command cmd(engine::COMMAND_ATTACK, player->getId(), selectedLand->getId(), target->getId());
								if(engine::Engine::getInstance().isAllowed(cmd)) sendCommand(cmd);
								setSelectedLand(nullptr);
							}
						}
					}
					else if (game.getCurrentStep()==state::STEP_MOVING) {
						if(!selectedLand) {
							setSelectedLand(target);
						}
						else {
							engine::Command cmd(engine::COMMAND_MOVE, player->getId(), selectedLand->getId(), target->getId());
							if(engine::Engine::getInstance().isAllowed(cmd)) sendCommand(cmd);
							setSelectedLand(nullptr);
						}
					}
					else {
						setSelectedLand(target);
					}

					clickClock.restart();

				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

					setSelectedInfoLand(target);

				}
			}
			else if(mousePos.x >= CELL_WIDTH * GRID_WIDTH + 30 && mousePos.x < CELL_WIDTH * GRID_WIDTH + 30 + 150 && mousePos.y >= CELL_HEIGHT * GRID_HEIGHT - 60 && mousePos.y < CELL_HEIGHT * GRID_HEIGHT - 60 + 40) {
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickClock.getElapsedTime().asMilliseconds() >= 500) {
					sendCommand(engine::Command(engine::COMMAND_SKIP_ROUND, player->getId(), -1, -1));
					clickClock.restart();
				}
			}

			sf::Time framerateElapsed = framerateClock.getElapsedTime();
			if (framerateElapsed.asMilliseconds() > 30) {
				engine::Engine::getInstance().flushCommands();

				window.clear();
				
				renderer.render(window);

				window.display();

				framerateClock.restart();
			}
			
		}

	}

	void Client::sendCommand(engine::Command command) {
		cout << command.toString() << endl;
		engine::Engine::getInstance().pushCommand(command);
	}

}
