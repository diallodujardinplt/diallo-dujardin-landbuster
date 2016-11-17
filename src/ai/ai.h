#ifndef AI__H
#define AI__H


namespace ai {

  class AI {
    // Attributes
  private:
    std::shared_ptr<state::Player> player
    // Operations
  public:
    AI(player:std::shared_ptr<state::Player>){}
    ~AI(){}
	run(game:state::Game&, playerId: unsigned int){}
  };

  class DumbAI : public ai::AI {
    // Attributes
  private:
    std::shared_ptr<state::Player> player
    // Operations
  public:
    DumbAI (player:std::shared_ptr<state::Player>);
    run(game:state::Game&, playerId: unsigned int);

};

#endif