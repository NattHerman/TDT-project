#pragma once

#include <vector>
#include <iostream>
#include <filesystem>

#include "Audio.h"
#include "AnimationWindow.h"

namespace Hex{
    
class AudioManager {
    std::vector<TDT4102::Audio> sounds;
    int index = 0;
    int maxCount = 8;

    std::shared_ptr<TDT4102::AnimationWindow> windowPtr;
    
public:
    void playSound(TDT4102::Audio sound) {
        if (!windowPtr) {
            throw std::runtime_error("Cannot play sound when windowPtr is null.");
        }

        sounds.at(index) = std::move(sound);
        windowPtr->play_audio(sounds.at(index));

        // something something keep sound alive 

        index = (index + 1) % maxCount;
    }

    AudioManager() {
        sounds.resize(maxCount);
    }

    AudioManager(std::shared_ptr<TDT4102::AnimationWindow> windowPtr): windowPtr{windowPtr} {
        sounds.resize(maxCount);
    }

    operator bool() const {
        return bool(windowPtr);
    }
};

} // namespace Hex
