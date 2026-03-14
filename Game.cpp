#include "Game.h"

Hex::Turn Hex::Game::getTurn() const {
    bool turnIsOdd = turnCounter % 2 == 1;

    return turnIsOdd ? Turn::Player1 : Turn::Player2;
}

bool Hex::Game::takeTurn(const vec2<int> &move) {
    bool moveSuccess = false;

    if (getTurn() == Turn::Player1) {
        moveSuccess = board->playerOnePlace(move);
    } else if (getTurn() == Turn::Player2) {
        moveSuccess = board->playerTwoPlace(move);
    }

    if (moveSuccess) {
        turnCounter++;
        // state = searchForWin();
    }


    return moveSuccess;
}
