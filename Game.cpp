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

bool Hex::Game::takeTurn(const vec2<int> &move) {
    if (state != GameState::Ongoing) { return false; }

    bool moveSuccess = false;

    if (getTurn() == Turn::Player1) {
        moveSuccess = board->playerOnePlace(move);
    } else if (getTurn() == Turn::Player2) {
        moveSuccess = board->playerTwoPlace(move);
    }

    // DEBUG RETURNED NEIGHBOURS
    // std::cout << "Move: " << move << std::endl;
    // for (vec2<int> tile : board->getNeighbouringOfType(move, getTurn() == Turn::Player1 ? TileType::StonePlayerOne : TileType::StonePlayerTwo)) {
    //     std::cout << "returned: " << tile << std::endl;
    // }

    if (moveSuccess) {
        state = searchForWin();

        // Play stone-placing sfx // REMEMBER TO LOAD BEFORE DOING STUFF
        // if (audioManager) {
        //     std::filesystem::path stoneAudioPath{"audiodata/stone"};
        //     audioManager.playSound(Hex::pickRandomAudio(stoneAudioPath));
        // }
        turnCounter++;

        saveGame("savedata/state.hex");
    }


    return moveSuccess;
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
}

void Hex::Game::newGame(vec2<int> size) {
    turnCounter = 1;
    state = GameState::Ongoing;

    board = std::make_shared<Board>(size);
}

void Hex::Game::saveGame(std::filesystem::path path) {
    std::ofstream outputstream{path};

    // Line 1: size of board.
    vec2<int> boardsize = board->getSize();
    outputstream << boardsize.x << " " << boardsize.y << "\n";

    // Line 2: game state.
    outputstream << int(state) << "\n";

    // x lines where each line is a row with y elements: board state
    for (std::vector<TileType> &row : board->getBoard()) {
        for (TileType tile : row) {
            outputstream << int(tile) << " ";
        }
        outputstream << "\n";
    }
}

void Hex::Game::loadGame(std::filesystem::path path) {

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
