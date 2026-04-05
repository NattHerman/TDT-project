#include "Board.h"
#include <cmath>

std::ostream& operator<<(std::ostream& lhs, const Hex::TileType &rhs) {
    lhs << Hex::tileToString.at(rhs);
    return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const Hex::Board &rhs) {
    constexpr int displacementSize = 2;
    constexpr int drawBounds = 1;
    int displacement = 0;
    for (int y = -drawBounds; y < rhs.getSize().y + drawBounds; ++y) {
        lhs << std::string(displacement * displacementSize, ' ');
        for (int x = -drawBounds; x < rhs.getSize().x + drawBounds; ++x) {
            Hex::TileType tile = rhs.getTile({x, y});
            lhs << tile << " ";
        }
        lhs << "\n";
        displacement++;
    }

    return lhs;
}

bool Hex::Board::tileIsPlayer1Edge(const vec2<int> &tile) const {
    bool onXEdge = tile.x == -1 || tile.x == size.x;

    return onXEdge;
}

bool Hex::Board::tileIsPlayer2Edge(const vec2<int> &tile) const {
    bool onYEdge = tile.y == -1 || tile.y == size.y;

    return onYEdge;
}

bool Hex::Board::tileIsWithinBounds(const vec2<int> &tile) const {
    bool withinX = 0 <= tile.x && tile.x < size.x;
    bool withinY = 0 <= tile.y && tile.y < size.y;

    return withinX && withinY;
}

bool Hex::Board::tileIsFree(const vec2<int> &tile) const {
    if (!tileIsWithinBounds(tile)) {
        return false;
    }
    // Tile is within bounds, on the board.
    return getTile(tile) == TileType::Empty;
}

Hex::TileType Hex::Board::getTile(const vec2<int> &tile) const {
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

bool Hex::Board::playerOnePlace(const vec2<int> &tile) {
    if (!tileIsFree(tile)) {
        return false;
    }

    board.at(tile.x).at(tile.y) = TileType::StonePlayerOne;
    return true;
}

bool Hex::Board::playerTwoPlace(const vec2<int> &tile) {
    if (!tileIsFree(tile)) {
        return false;
    }

    board.at(tile.x).at(tile.y) = TileType::StonePlayerTwo;
    return true;
}

Hex::Board::Board(const vec2<int> &size): size{size} {
    // Allocate board memory, and populate board vectors
    board.reserve(size.x);
    for (int x = 0; x < size.x; ++x) {
        board.emplace_back(size.y, TileType::Empty); // Claude suggestion, more efficient
    }
}

std::vector<Hex::vec2<int>> Hex::Board::getNeighbouringOfType(Hex::vec2<int> tile, Hex::TileType targetType) {
    static const std::vector<Hex::vec2<int>> relativeTiles = {
        { 1,  0},
        {-1,  0},
        { 1, -1},
        { 0,  1},
        { 0, -1},
        {-1,  1},
    };

    std::vector<Hex::vec2<int>> tiles;
    tiles.reserve(6);

    TileType edgeType = TileType::EdgePlayerOne;
    if (targetType == TileType::StonePlayerTwo) {
        edgeType = TileType::EdgePlayerTwo;
    } 

    for (Hex::vec2<int> relativeTile : relativeTiles) {
        Hex::TileType type = getTile(tile + relativeTile);
        if (type == targetType || type == edgeType) {
            tiles.emplace_back(tile + relativeTile);
        }
    }

    return tiles;
}