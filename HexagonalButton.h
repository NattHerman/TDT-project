#pragma once

#include "Board.h"
namespace Hex {

class HexagonalButton {
    vec2<int> tile; // Which tile on the board this button corresponds to.
    int radius = 25;
    bool isSelected = false;

public:
    void draw();
    vec2<int> getScreenPosition();

    friend class GUI;
};

} // namespace Hex