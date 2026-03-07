#pragma once

#include <vector>
#include <AnimationWindow.h>
#include "vec2.h"

namespace Hex {

enum class TileType {
    Empty = 0,
    StonePlayerOne,
    StonePlayerTwo,
    EdgePlayerOne,
    EdgePlayerTwo,
    EdgeBoth,
};

class Board {
    vec2<int> size;
    // Edges facing +- x-direction are player one's
    // Edges facing +- y-direction are player two's
    std::vector<std::vector<TileType>> board;

    std::vector<vec2<int>> stonesPlayerOne;
    std::vector<vec2<int>> stonesPlayerTwo;

    bool tileIsWithinBounds(vec2<int> tile);
    bool tileIsPlayer1Edge(vec2<int> tile);
    bool tileIsPlayer2Edge(vec2<int> tile);
    bool tileIsFree(vec2<int> tile);

public:
    TileType getTile(vec2<int> tile);

    bool playerOnePlace(vec2<int> tile);
    bool playerTwoPlace(vec2<int> tile);

    Board(vec2<int> size); // initialize board, stonesPlayerOne and stonesPlayerTwo sizes
};

} // Hex namespace
