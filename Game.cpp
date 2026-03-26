#include "Game.h"
#include "Audio.h"

#include <random>
#include <iostream>

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
        throw std::runtime_error("Directory '" + std::string(directory) + "' does not exist.");
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
    bool moveSuccess = false;

    if (getTurn() == Turn::Player1) {
        moveSuccess = board->playerOnePlace(move);
    } else if (getTurn() == Turn::Player2) {
        moveSuccess = board->playerTwoPlace(move);
    }

    if (moveSuccess) {
        turnCounter++;
        // state = searchForWin();

        // Play stone-placing sfx // REMEMBER TO LOAD BEFORE DOING STUFF
        // if (audioManager) {
        //     std::filesystem::path stoneAudioPath{"audiodata/stone"};
        //     audioManager.playSound(Hex::pickRandomAudio(stoneAudioPath));
        // }
    }


    return moveSuccess;
}
