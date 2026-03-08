#include "board.h"
#include <cmath>

std::ostream& operator<<(std::ostream& lhs, Hex::TileType rhs) {
    lhs << Hex::tileToString.at(rhs);
    return lhs;
}

std::ostream& operator<<(std::ostream& lhs, Hex::Board rhs) {
    int displacement = 0;
    int displacementSize = 2;
    for (std::vector<Hex::TileType> col : rhs.getBoard()) {
        lhs << std::string(displacement * displacementSize, ' ');
        for (Hex::TileType tile : col) {
            lhs << tile << " ";
        }
        lhs << "\n";
        displacement++;
    }
    
    return lhs;
}

bool Hex::Board::tileIsPlayer1Edge(vec2<int> tile) {
    bool onXEdge = tile.x == -1 || tile.x == size.x;

    return onXEdge;
}

bool Hex::Board::tileIsPlayer2Edge(vec2<int> tile) {
    bool onYEdge = tile.y == -1 || tile.y == size.y;

    return onYEdge;
}

bool Hex::Board::tileIsWithinBounds(vec2<int> tile) {
    bool withinX = 0 <= tile.x && tile.x < size.x;
    bool withinY = 0 <= tile.y && tile.y < size.y;

    return withinX && withinY;
}

bool Hex::Board::tileIsFree(vec2<int> tile) {
    if (!tileIsWithinBounds(tile)) {
        return false;
    }
    // Tile is within bounds, on the board.
    return getTile(tile) == TileType::Empty;
}

Hex::TileType Hex::Board::getTile(vec2<int> tile) {
    if (tileIsWithinBounds(tile)) {
        return board.at(tile.x).at(tile.y);
    }

    bool isP1Edge = tileIsPlayer1Edge(tile);
    bool isP2Edge = tileIsPlayer2Edge(tile);

    if (isP1Edge && isP2Edge) {
        return TileType::EdgeBoth;
    } else if (isP1Edge) {
        return TileType::EdgePlayerOne;
    } else if (isP2Edge) {
        return TileType::EdgePlayerTwo;
    }

    return TileType::OutOfBounds;
}

bool Hex::Board::playerOnePlace(vec2<int> tile) {
    if (!tileIsFree(tile)) {
        return false;
    }

    stonesPlayerOne.emplace_back(tile);
    board.at(tile.x).at(tile.y) = TileType::StonePlayerOne;
    return true;
}

bool Hex::Board::playerTwoPlace(vec2<int> tile) {
    if (!tileIsFree(tile)) {
        return false;
    }

    stonesPlayerTwo.emplace_back(tile);
    board.at(tile.x).at(tile.y) = TileType::StonePlayerTwo;
    return true;
}

Hex::Board::Board(vec2<int> size): size{size} {
    // Allocate board memory, and populate board vectors
    board.reserve(size.x);
    for (int x = 0; x < size.x; ++x) {
        board.emplace_back(size.y, TileType::Empty); // Claude suggestion, more efficient
    }

    int maxPossibleMoves = size.x * size.y / 2;
    stonesPlayerOne.reserve(maxPossibleMoves);
    stonesPlayerTwo.reserve(maxPossibleMoves);
}
