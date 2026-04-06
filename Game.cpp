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

        saveGame(savePath);
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

    saveGame(savePath);
}

void Hex::Game::newGame(vec2<int> size) {
    turnCounter = 1;
    state = GameState::Ongoing;

    board = std::make_shared<Board>(size);
}

void Hex::Game::saveGame(std::filesystem::path path) {
    std::ofstream outputstream{path};

    // Line 1: <board size x> <board size y> <game state> <turn count>.
    vec2<int> boardsize = board->getSize();
    outputstream << boardsize.x << " " << boardsize.y << " ";
    outputstream << int(state) << " ";
    outputstream << turnCounter << "\n";

    // (there are two ways i could approach saving the board. 1: save the moves in order and replay them at load, or 2: store the entire board.
    // i chose option 2, although option 1 would store more information that could be used for a replay or something.)

    // x lines where each line is a row with y elements: board state
    for (std::vector<TileType> &row : board->getBoard()) {
        for (TileType tile : row) {
            outputstream << int(tile) << " ";
        }
        outputstream << "\n";
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
    vec2<int> boardsize;
    int gameStateInt = 0;
    int turnCount = 0;

    try {
        line1Stream >> boardsize.x;
        line1Stream >> boardsize.y;
        line1Stream >> gameStateInt;
        line1Stream >> turnCount;

    } catch (std::ios_base::failure) {
        std::cout << "Saved state loading line 1 failed " << path << ", starting fresh game." << std::endl;
        return;
    }

    // Values loaded from next lines.
    std::vector<std::vector<TileType>> boardState;

    int lineNumer = 2;
    try {
        // Initialize vectors for storing new board state
        boardState.reserve(boardsize.x);
        for (int x = 0; x < boardsize.x; ++x) {
            boardState.emplace_back(boardsize.y, TileType::Empty);
        }

        // Load board state from filestream
        for (int x = 0; x < boardsize.x; ++x) {
            std::string line;
            std::getline(inputstream, line);
            std::stringstream lineStream{line};
            lineStream.exceptions(std::ios::failbit);
            
            for (int y = 0; y < boardsize.y; ++y) {
                int tileTypeInt = 0;
                lineStream >> tileTypeInt;
                boardState.at(x).at(y) = TileType(tileTypeInt);
            }
            lineNumer++;
        }
    } catch (std::ios_base::failure) {
        std::cout << "Saved state loading line " << lineNumer << " failed " << path << ", starting fresh game." << std::endl;
        return;
    }

    // Set values after successful loading
    state = GameState(gameStateInt);
    turnCounter = turnCount;
    board = std::make_shared<Board>(boardsize);
    board->setBoard(boardState);

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
