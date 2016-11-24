#include "Client.hpp"

using namespace std;

namespace render {

	Client::Client() {
		debugMode = false;
	}

	Client::~Client() {}

	Client& Client::getInstance() {
		static Client client;
		return client;
	}

	bool Client::getDebugMode() const {
		return debugMode;
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

	void Client::run(state::Game& game, shared_ptr<engine::Engine> engine) {

		sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_WIDTH + 200, GRID_HEIGHT * CELL_HEIGHT, 32), "Land Buster");

		render::Renderer& renderer = render::Renderer::getInstance();
		renderer.init();

		sf::Clock framerateClock, clickClock;

		while (window.isOpen()) {
			player = game.getPlayers()[game.getCurrentPlayer()];

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && clickClock.getElapsedTime().asMilliseconds() >= 500) {
					debugMode = !debugMode;
					clickClock.restart();
				}
			}

			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			if(mousePos.x > CELL_WIDTH/2 && mousePos.x < GRID_WIDTH * CELL_WIDTH && mousePos.y > CELL_HEIGHT/2 && mousePos.y < GRID_HEIGHT * CELL_HEIGHT) {
				shared_ptr<state::Land> target = game.getCell(mousePos.x / CELL_WIDTH, mousePos.y / CELL_HEIGHT).land;

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickClock.getElapsedTime().asMilliseconds() >= 500) {

					cout << "click on land " << target->getId() << endl;

					if (!player->getHeadquarters()) {
						sendCommand(engine, make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_HEADQUARTERS, player->getId(), target->getId()));
					}
					else if (game.getCurrentStep()==state::STEP_REINFORCEMENT) {
						sendCommand(engine, make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_REINFORCEMENT, player->getId(), target->getId()));
					}
					else if (game.getCurrentStep()==state::STEP_ACTION) {
						if(!selectedLand) {
							setSelectedLand(target);
						}
						else {
							if(selectedLand == target) {
								shared_ptr<engine::ChoiceCommand> cmd = make_shared<engine::ChoiceCommand>(engine::COMMAND_BUILD_PORT, player->getId(), target->getId());
								if(engine->isAllowed(cmd)) {
									sendCommand(engine, cmd);
									setSelectedLand(nullptr);
								}
							}
							else {
								vector<engine::Interaction> attacks;
								attacks.push_back(engine::Interaction(selectedLand->getId(), target->getId()));
								shared_ptr<engine::AttackCommand> cmd = make_shared<engine::AttackCommand>(player->getId(), attacks);
								if(engine->isAllowed(cmd)) {
									sendCommand(engine, cmd);
									setSelectedLand(nullptr);
								}
							}
						}
					}
					else if (game.getCurrentStep()==state::STEP_MOVING) {
						if(!selectedLand) {
							setSelectedLand(target);
						}
						else {
							shared_ptr<engine::MoveCommand> cmd = make_shared<engine::MoveCommand>(player->getId(), engine::Interaction(selectedLand->getId(), target->getId()), 0.5, false);
							if(engine->isAllowed(cmd)) {
								sendCommand(engine, cmd);
								setSelectedLand(nullptr);
							}
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
			else if(mousePos.x >= CELL_WIDTH * GRID_WIDTH + 30 && mousePos.x < CELL_WIDTH * GRID_WIDTH + 30 + 150 && mousePos.y >= CELL_HEIGHT * GRID_HEIGHT - 60 && mousePos.y < CELL_HEIGHT * GRID_HEIGHT - 60 + 30) {
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickClock.getElapsedTime().asMilliseconds() >= 500) {
					sendCommand(engine, make_shared<engine::ActionCommand>(engine::COMMAND_SKIP_ROUND, player->getId()));
					clickClock.restart();
				}
			}
			else if(mousePos.x >= CELL_WIDTH * GRID_WIDTH + 30 && mousePos.x < CELL_WIDTH * GRID_WIDTH + 30 + 150 && mousePos.y >= CELL_HEIGHT * GRID_HEIGHT - 30 && mousePos.y < CELL_HEIGHT * GRID_HEIGHT - 30 + 30) {
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickClock.getElapsedTime().asMilliseconds() >= 500) {
					sendCommand(engine, make_shared<engine::ActionCommand>(engine::COMMAND_ABANDON, player->getId()));
					clickClock.restart();
				}
			}

			sf::Time framerateElapsed = framerateClock.getElapsedTime();
			if (framerateElapsed.asMilliseconds() > 30) {
				engine->flushCommands();

				window.clear();
				
				renderer.render(window, game);

				window.display();

				framerateClock.restart();
			}
			
		}

	}

	void Client::sendCommand(shared_ptr<engine::Engine> engine, shared_ptr<engine::Command> command) {
		cout << command->toString() << endl;
		engine->pushCommand(command);
	}

}
