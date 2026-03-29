#pragma once

#include <memory>

#include "Label.h"
#include "Game.h"

namespace Hex {
namespace UI {

class TurnIndicator : public UINode {
    std::shared_ptr<TDT4102::AnimationWindow> windowPtr;
    std::shared_ptr<Game> gamePtr;

    TurnIndicator(std::shared_ptr<Game> gamePtr, std::shared_ptr<TDT4102::AnimationWindow> windowPtr): gamePtr{gamePtr}, windowPtr{windowPtr} {}

public:
    void draw() override;
    void update() override;

    std::shared_ptr<Label> getChild(int index) const { return std::static_pointer_cast<Label>(children.at(index)); }

    static std::shared_ptr<TurnIndicator> create(std::shared_ptr<Game> gamePtr, std::shared_ptr<TDT4102::AnimationWindow> windowPtr) {
        std::shared_ptr<TurnIndicator> newTurnIndicator = std::shared_ptr<TurnIndicator>(new TurnIndicator(gamePtr, windowPtr));
        newTurnIndicator->addChild(std::make_shared<Label>("Player 1", 100, windowPtr));
        newTurnIndicator->addChild(std::make_shared<Label>("Player 2", 100, windowPtr));

        return newTurnIndicator;
    }

    friend std::shared_ptr<TurnIndicator>;
};

} // namespace UI
} // namespace Hex
