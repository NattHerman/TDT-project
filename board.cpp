#include "board.h"

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

