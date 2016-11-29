#include "Client.hpp"

using namespace std;

namespace render {

	Client::Client(shared_ptr<state::Game> game, shared_ptr<engine::Engine> engine) {
		this->game = game;
		this->engine = engine;
		debugMode = false;
	}

	Client::Client(Client& client) {
		attributedPlayers = client.attributedPlayers;
		currentPlayerId = client.currentPlayerId;
		selectedInfoLand = client.selectedInfoLand;
		selectedLand = client.selectedLand;
		debugMode = client.debugMode;
		game = client.game;
		engine = client.engine;
	}

	Client::~Client() {}

	bool Client::getDebugMode() const {
		return debugMode;
	}

	vector<unsigned int> Client::getAttributedPlayers() const {
		return attributedPlayers;
	}

	void Client::addAttributedPlayer(unsigned int playerId) {
		attributedPlayers.push_back(playerId);
	}

	unsigned int Client::getCurrentPlayerId() const {
		return currentPlayerId;
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

	void Client::operator()() {

		sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_WIDTH + 200, GRID_HEIGHT * CELL_HEIGHT, 32), "Land Buster");

		render::Renderer renderer(this);
		renderer.init();

		sf::Clock framerateClock, clickClock;

		while (window.isOpen()) {
			game->mutex.lock();

			shared_ptr<state::Player> player = game->getPlayers()[game->getCurrentPlayer()];
			currentPlayerId = game->getCurrentPlayer();

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					game->mutex.unlock();
					engine->mutex.lock();
					engine->commandQuit = true;
					engine->mutex.unlock();
					game->mutex.lock();
					window.close();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && clickClock.getElapsedTime().asMilliseconds() >= 500) {
					debugMode = !debugMode;
					clickClock.restart();
				}
			}

			if(find(attributedPlayers.begin(), attributedPlayers.end(), game->getCurrentPlayer()) != attributedPlayers.end()) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				if(mousePos.x > CELL_WIDTH/2 && mousePos.x < GRID_WIDTH * CELL_WIDTH && mousePos.y > CELL_HEIGHT/2 && mousePos.y < GRID_HEIGHT * CELL_HEIGHT) {
					shared_ptr<state::Land> target = game->getCell(mousePos.x / CELL_WIDTH, mousePos.y / CELL_HEIGHT).land;

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickClock.getElapsedTime().asMilliseconds() >= 500) {

						cout << "click on land " << target->getId() << endl;

						if (!player->getHeadquarters()) {
							sendCommand(make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_HEADQUARTERS, player->getId(), target->getId()));
						}
						else if (game->getCurrentStep()==state::STEP_REINFORCEMENT) {
							sendCommand(make_shared<engine::ChoiceCommand>(engine::COMMAND_CHOOSE_REINFORCEMENT, player->getId(), target->getId()));
						}
						else if (game->getCurrentStep()==state::STEP_ACTION) {
							if(!selectedLand) {
								setSelectedLand(target);
							}
							else {
								if(selectedLand == target) {
									shared_ptr<engine::ChoiceCommand> cmd = make_shared<engine::ChoiceCommand>(engine::COMMAND_BUILD_PORT, player->getId(), target->getId());
									if(engine->isAllowed(cmd)) {
										sendCommand(cmd);
										setSelectedLand(nullptr);
									}
								}
								else {
									vector<engine::Interaction> attacks;
									attacks.push_back(engine::Interaction(selectedLand->getId(), target->getId()));
									shared_ptr<engine::AttackCommand> cmd = make_shared<engine::AttackCommand>(player->getId(), attacks);
									if(engine->isAllowed(cmd)) {
										sendCommand(cmd);
										setSelectedLand(nullptr);
									}
								}
							}
						}
						else if (game->getCurrentStep()==state::STEP_MOVING) {
							if(!selectedLand) {
								setSelectedLand(target);
							}
							else {
								shared_ptr<engine::MoveCommand> cmd = make_shared<engine::MoveCommand>(player->getId(), engine::Interaction(selectedLand->getId(), target->getId()), 0.5, false);
								if(engine->isAllowed(cmd)) {
									sendCommand(cmd);
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
						sendCommand(make_shared<engine::ActionCommand>(engine::COMMAND_SKIP_ROUND, player->getId()));
						clickClock.restart();
					}
				}
				else if(mousePos.x >= CELL_WIDTH * GRID_WIDTH + 30 && mousePos.x < CELL_WIDTH * GRID_WIDTH + 30 + 150 && mousePos.y >= CELL_HEIGHT * GRID_HEIGHT - 30 && mousePos.y < CELL_HEIGHT * GRID_HEIGHT - 30 + 30) {
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickClock.getElapsedTime().asMilliseconds() >= 500) {
						sendCommand(make_shared<engine::ActionCommand>(engine::COMMAND_ABANDON, player->getId()));
						clickClock.restart();
					}
				}
			}

			game->mutex.unlock();

			sf::Time framerateElapsed = framerateClock.getElapsedTime();
			if (framerateElapsed.asMilliseconds() > 30) {

				window.clear();

				game->mutex.lock();
				renderer.render(window, *game, game->getPlayers()[currentPlayerId]);
				game->mutex.unlock();

				window.display();

				framerateClock.restart();
			}
			
		}

	}

	void Client::sendCommand(shared_ptr<engine::Command> command) {
		cout << command->toString() << endl;
		engine->mutex.lock();
		engine->pushCommand(command);
		engine->mutex.unlock();
	}

}
