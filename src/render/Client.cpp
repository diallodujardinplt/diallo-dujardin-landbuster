#include "Client.hpp"

using namespace std;

namespace render {

	Client::Client() {}

	void Client::run() {

		sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_WIDTH + 200, GRID_HEIGHT * CELL_HEIGHT, 32), "Land Buster");

		render::Renderer& renderer = render::Renderer::getInstance();
		renderer.init();

		sf::Clock framerateClock;

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
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

}
