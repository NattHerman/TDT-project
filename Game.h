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
    std::vector<vec2<int>> moves;
    std::vector<vec2<int>> winningTilePath;

    AudioManager audioManager;

    GameState searchForWin(); // Look for a path of stones going from one side to the other.

public:
    std::filesystem::path savePath;

    Turn getTurn() const;
    std::shared_ptr<Board>  getBoard() const { return board; };
    std::vector<vec2<int>>  getMoves() const { return moves; };
    std::vector<vec2<int>>  getWinningTilePath() const { return winningTilePath; };
    GameState getState() const { return state; };

    void takeTurn(const vec2<int> &move, bool save = true);
    void undo();
    void forfeit();

    void playMoves(std::vector<vec2<int>> moves);
    void displayMove(int index);

    void newGame(vec2<int> size = {11, 11});
    void saveGame(std::filesystem::path path);
    void loadGame(std::filesystem::path path);

    Game(const vec2<int> &boardSize, std::filesystem::path savePath)
    : board{std::make_shared<Board>(boardSize)}, savePath{savePath} {
        loadGame(savePath);
        moves.reserve(boardSize.x*boardSize.y);
    }
    Game(const vec2<int> &boardSize, std::filesystem::path savePath, std::shared_ptr<TDT4102::AnimationWindow> windowPtr)
    : board{std::make_shared<Board>(boardSize)}, savePath{savePath}, audioManager{windowPtr} { loadGame(savePath); };
};

// Helper functions
TDT4102::Audio pickRandomAudio(std::filesystem::path directory);
int rand(int from, int to);

} // namespace Hex
