#pragma once

#include "Board.h"
#include "Color.h"

namespace Hex {

class HexagonalButton {
    vec2<int> tile; // Which tile on the board this button corresponds to.
    int radius = 25;
    bool isSelected = false;

    TDT4102::Color d;

public:
    void draw(TDT4102::AnimationWindow &window);
    vec2<int> getScreenPosition(const TDT4102::AnimationWindow &window) const;

    HexagonalButton(const vec2<int> &tile): tile{tile} {};

    friend class GUI;
};

} // namespace Hex