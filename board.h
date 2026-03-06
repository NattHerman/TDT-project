#pragma once

#include <vector>
#include <AnimationWindow.h>
#include "vec2.h"

namespace Hex {

class Board {
    vec2<int> size;

    std::vector<vec2<int>> piecesPlayerOne;
    std::vector<vec2<int>> piecesPlayerTwo;

    void playerOnePlace(vec2<int> position);
    void playerTwoPlace(vec2<int> position);

    Board(vec2<int> size); // initialize piecesPlayerOne and -two sizes
};

} // Hex namespace
