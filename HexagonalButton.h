#pragma once

#include "Board.h"
#include "Color.h"

namespace Hex {

class HexagonalButton {
    vec2<int> tile; // Which tile on the board this button corresponds to.
    int radius = 25;
    bool isSelected = false;

    std::shared_ptr<TDT4102::AnimationWindow> windowPtr;

    TDT4102::Color d;

public:
    void draw();
    vec2<int> getScreenPosition() const;

    HexagonalButton(const vec2<int> &tile, const std::shared_ptr<TDT4102::AnimationWindow> &windowPtr): tile{tile}, windowPtr{windowPtr} {};

    friend class GUI;
};

} // namespace Hex