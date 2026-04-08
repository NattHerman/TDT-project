#include "Game.h"
#include "Audio.h"

#include <random>
#include <iostream>
#include <fstream>

Hex::Turn Hex::Game::getTurn() const {
    bool turnIsOdd = turnCounter % 2 == 1;

    return turnIsOdd ? Turn::Player1 : Turn::Player2;
}

int Hex::rand(int from, int to) {
    // Generate a random seed using the operating system
    static std::random_device device;
    unsigned long randomSeed = device();
    // Initialise the random engine
    static std::default_random_engine engine{randomSeed};
    // Now you can generate numbers!
    std::uniform_int_distribution integerDistribution{from, to};

    return integerDistribution(engine);
}

TDT4102::Audio Hex::pickRandomAudio(std::filesystem::path directory) {
    if (!std::filesystem::exists(directory)) {
        throw std::runtime_error("Directory '" + directory.string() + "' does not exist.");
    }

    std::vector<std::filesystem::path> audioFiles;
    audioFiles.reserve(4);

    // Discover audio files
    for (std::filesystem::directory_entry directoryEntry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path path = directoryEntry.path();
        if (path.extension() == ".wav" || path.extension() == ".mp3") {
            audioFiles.push_back(path);
        }
    }

    int index = rand(0, audioFiles.size() - 1);
    std::cout << audioFiles.at(index) << std::endl;
    return TDT4102::Audio(audioFiles.at(index));
}

// Throws InvalidMoveError if move is invalid
void Hex::Game::takeTurn(const vec2<int> &move, bool save) {
    if (state != GameState::Ongoing) { return; }

    if (getTurn() == Turn::Player1) {
        board->playerOnePlace(move); // Can throw
    } else if (getTurn() == Turn::Player2) {
        board->playerTwoPlace(move); // Can throw
    }

    // DEBUG RETURNED NEIGHBOURS
    // std::cout << "Move: " << move << std::endl;
    // for (vec2<int> tile : board->getNeighbouringOfType(move, getTurn() == Turn::Player1 ? TileType::StonePlayerOne : TileType::StonePlayerTwo)) {
    //     std::cout << "returned: " << tile << std::endl;
    // }

    state = searchForWin();

    // Play stone-placing sfx // REMEMBER TO LOAD BEFORE DOING STUFF
    // if (audioManager) {
    //     std::filesystem::path stoneAudioPath{"audiodata/stone"};
    //     audioManager.playSound(Hex::pickRandomAudio(stoneAudioPath));
    // }
    turnCounter++;
    moves.emplace_back(move);

    if (save) {
        saveGame(savePath);
    }
}

// Resets the board, and plays moves from start.
void Hex::Game::playMoves(std::vector<vec2<int>> moves) {
    board->resetBoard();
    state = GameState::Ongoing;
    turnCounter = 1;
    for (vec2<int> move : moves) {
        takeTurn(move, false); // Can throw
    }
}

void Hex::Game::forfeit() {
    switch (getTurn()) {
    case Turn::Player1:
        state = GameState::Player2Won; break;
    case Turn::Player2:
        state = GameState::Player1Won; break;
    default:
        break;
    }

    saveGame(savePath);
}

void Hex::Game::newGame(vec2<int> size) {
    turnCounter = 1;
    state = GameState::Ongoing;
    moves.clear();

    board = std::make_shared<Board>(size);
}

void Hex::Game::saveGame(std::filesystem::path path) {
    std::ofstream outputstream{path};

    // Line 1: <board size x> <board size y> <game state> <turn count>.
    vec2<int> boardsize = board->getSize();
    outputstream << boardsize.x << " " << boardsize.y << " ";
    outputstream << int(state) << " ";
    outputstream << turnCounter << "\n";

    // Line 2 and onward:
    // <move 1 x> <move 1 y>
    // <move 2 x> <move 2 y>
    // ...
    // <move n x> <move n y>, where n = turnCounter - 1 = moves.size()
    for (vec2<int> move : moves) {
        outputstream << move.x << " " << move.y << "\n";
    }
}

void Hex::Game::loadGame(std::filesystem::path path) {
    std::cout << "Loading game.." << std::endl;
    if (!std::filesystem::exists(path)) {
        std::cout << "Saved state doesnt exist at " << path << ", starting fresh game." << std::endl;
        return;
    }

    std::ifstream inputstream{path};

    // Line 1: <board size x> <board size y> <game state> <turn count>.
    std::string line1;
    std::getline(inputstream, line1);
    std::stringstream line1Stream{line1};
    line1Stream.exceptions(std::ios::failbit); // help from claude, figure out how to make >> operator throw on failure.

    // Values loaded from line 1.
    vec2<int> loadedBoardsize;
    int loadedGameStateInt = 0;
    int loadedTurnCount = 0;

    try {
        line1Stream >> loadedBoardsize.x;
        line1Stream >> loadedBoardsize.y;
        line1Stream >> loadedGameStateInt;
        line1Stream >> loadedTurnCount;

    } catch (std::ios_base::failure) {
        std::cout << "Saved state loading line 1 failed " << path << ", starting fresh game." << std::endl;
        return;
    }

    // Values loaded from next lines.
    std::vector<vec2<int>> loadedMoves;

    int lineNumer = 2; // for the error
    try {
        loadedMoves.reserve(loadedBoardsize.x * loadedBoardsize.y);

        for (int i = 0; i < loadedTurnCount - 1; ++i) {
            std::string line;
            std::getline(inputstream, line);
            std::stringstream lineStream{line};
            lineStream.exceptions(std::ios::failbit);
            
            vec2<int> move;
            lineStream >> move.x;
            lineStream >> move.y;
            loadedMoves.emplace_back(move);
            
            lineNumer++;
        }
    } catch (std::ios_base::failure) {
        std::cout << "Saved state loading line " << lineNumer << " failed " << path << ", starting fresh game." << std::endl;
        return;
    }

    // Set values after successful loading
    state = GameState(loadedGameStateInt);
    turnCounter = loadedTurnCount;
    try {
        playMoves(loadedMoves);
    } catch (InvalidMoveError& error) {
        std::cout << "Saved move " << error.move << ", turn " << turnCounter << " is invalid " << path << ", starting fresh game." << std::endl;
        return;
    }

    std::cout << "Game loaded." << std::endl;
}

Hex::GameState Hex::Game::searchForWin() {
    vec2<int> startTile;
    vec2<int> endTile;

    switch (getTurn()) {
    case Turn::Player2:
        startTile = {1, -1};
        endTile = {0, board->getSize().y};
        break;
    case Turn::Player1:
        startTile = {-1, 1};
        endTile = {board->getSize().x, 0};
        break;
    }

    std::shared_ptr<Path::BoardNodeGenerator> boardNodeGenerator = std::make_shared<Path::BoardNodeGenerator>(getBoard(), getTurn() == Turn::Player1 ? TileType::StonePlayerOne : TileType::StonePlayerTwo);

    std::shared_ptr<Path::BoardNode> startNode = boardNodeGenerator->getNode(startTile);
    std::shared_ptr<Path::BoardNode> endNode = boardNodeGenerator->getNode(endTile);

    Path::AStar pathfinder{startNode, endNode};
    std::vector<std::shared_ptr<Path::Node>> path = pathfinder.findPath(audioManager.getWindowPtr());

    if (path.size() > 0) {
        return getTurn() == Turn::Player1 ? GameState::Player1Won : GameState::Player2Won;
    }

    return GameState::Ongoing;
}
