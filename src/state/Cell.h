// Generated by dia2code
#ifndef STATE__CELL__H
#define STATE__CELL__H


namespace state {

  /// class Cell - 
  class Cell {
    // Attributes
  public:
    sf::Vector2u position;
    bool borderTop;
    bool borderBottom;
    bool borderLeft;
    bool borderRight;
    // Operations
  public:
    Cell (sf::Vector2u position, bool borderTop, bool borderBottom, bool borderLeft, bool borderRight);
  };

};

#endif
