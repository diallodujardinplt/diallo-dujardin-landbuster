// Generated by dia2code
#ifndef RENDER__RENDERER__H
#define RENDER__RENDERER__H

#include <map>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace render {
  class Renderer;
};
namespace state {
  class Game;
  class Land;
  class Cell;
};
namespace render {
  class Client;
}

#include "state/Land.h"
#include "Client.h"
#include "state/Game.h"

namespace render {

  /// class Renderer - 
  class Renderer {
    // Associations
    // Attributes
  private:
    std::map<state::LandType, sf::Texture> landTextures;
    sf::Font font;
    sf::RectangleShape waterRect;
    sf::RectangleShape portRect;
    sf::Texture headquartersMaskTexture;
    sf::Texture heroTexture;
    sf::Texture fortTexture;
    // Operations
  public:
    ~Renderer ();
    static Renderer& getInstance ();
    void render (sf::RenderWindow& window);
    void init ();
    void renderFort (sf::RenderWindow& window, state::Game& game, std::shared_ptr<state::Land> land, const std::vector<sf::Vector2u>& geometry);
  private:
    Renderer ();
    void loadTextures ();
    void renderCell (sf::RenderWindow& window, state::Game& game, std::shared_ptr<state::Land> land, const std::vector<sf::Vector2u>& geometry, state::Cell& cell);
    void renderLandBorders (sf::RenderWindow& window, state::Game& game, state::Cell& cell, unsigned int x, unsigned int y, sf::Vector2f pos, bool borderTop, bool borderBottom, bool borderLeft, bool borderRight);
    void renderSoldiersNumber (sf::RenderWindow& window, state::Game& game, std::shared_ptr<state::Land> land, const std::vector<sf::Vector2u>& geometry);
    void renderLand (sf::RenderWindow& window, state::Game& game, std::shared_ptr<state::Land> land);
    void renderItem (sf::RenderWindow& window, state::Game& game, std::shared_ptr<state::Land> land, const std::vector<sf::Vector2u>& geometry);
    void renderHero (sf::RenderWindow& window, state::Game& game, std::shared_ptr<state::Land> land, const std::vector<sf::Vector2u>& geometry);
    sf::Vector2u getMeanPos (state::Game& game, std::shared_ptr<state::Land> land, const std::vector<sf::Vector2u>& geometry, unsigned int offset);
    void renderGame (sf::RenderWindow& window);
    void renderUI (sf::RenderWindow& window);
  };

};

#endif
