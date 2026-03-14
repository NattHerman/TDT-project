#pragma once

#include "Board.h"

namespace Hex {

enum class GameState {
    Ongoing = 0,
    Player1Won,
    Player2Won,
};

enum class Turn {
    Player1 = 1,
    Player2,
};

class Game {
    int turnCounter = 1;
    GameState state = GameState::Ongoing;
    std::shared_ptr<Board> board;

    GameState searchForWin(); // Look for a path of stones going from one side to the other.

public:
    Turn getTurn() const;
    std::shared_ptr<Board>  getBoard() const { return board; };
    GameState getState() const { return state; };
    bool takeTurn(const vec2<int> &move);

    Game(const vec2<int> &boardSize): board{std::make_shared<Board>(boardSize)} {};
};

} // namespace Hex
