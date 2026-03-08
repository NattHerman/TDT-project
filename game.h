#pragma once

#include "board.h"

namespace Hex {

enum class GameState {
    Ongoing,
    Player1Won,
    Player2Won,
};

class Game {
    int turn = 1;
    GameState state = GameState::Ongoing;

    Board board;

    GameState searchForWin();

public:
    Game(const vec2<int> &boardSize);
};

} // namespace Hex
