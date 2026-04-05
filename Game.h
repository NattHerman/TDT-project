#pragma once

#include <filesystem>

#include "Board.h"
#include "AudioManager.h"
#include "pathfinding/BoardNode.h"

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

    AudioManager audioManager;

    GameState searchForWin(); // Look for a path of stones going from one side to the other.

public:
    Turn getTurn() const;
    std::shared_ptr<Board>  getBoard() const { return board; };
    GameState getState() const { return state; };
    bool takeTurn(const vec2<int> &move);
    void forfeit();

    void newGame(vec2<int> size = {11, 11});
    void saveGame(std::filesystem::path path);
    void loadGame(std::filesystem::path path);

    Game(const vec2<int> &boardSize)
    : board{std::make_shared<Board>(boardSize)} {};
    Game(const vec2<int> &boardSize, std::shared_ptr<TDT4102::AnimationWindow> windowPtr)
    : board{std::make_shared<Board>(boardSize)}, audioManager{windowPtr} {};
};

// Helper functions
TDT4102::Audio pickRandomAudio(std::filesystem::path directory);
int rand(int from, int to);

} // namespace Hex
