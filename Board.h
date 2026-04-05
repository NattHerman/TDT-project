#pragma once

#include <vector>
#include <AnimationWindow.h>
#include "vec2.h"
#include <ostream>

namespace Hex {

enum class TileType {
    Empty = 0,
    StonePlayerOne,
    StonePlayerTwo,
    EdgePlayerOne,
    EdgePlayerTwo,
    EdgeBoth,
    OutOfBounds,
};

inline std::unordered_map<TileType, std::string> tileToString {
    {TileType::Empty, " -"},
    {TileType::StonePlayerOne, " 1"},
    {TileType::StonePlayerTwo, " 2"},
    {TileType::EdgePlayerOne, "E1"},
    {TileType::EdgePlayerTwo, "E2"},
    {TileType::EdgeBoth, "EB"},
    {TileType::OutOfBounds, " _"},
};

class Board {
    vec2<int> size;
    // Edges facing +- x-direction are player one's
    // Edges facing +- y-direction are player two's
    std::vector<std::vector<TileType>> board;

    bool tileIsWithinBounds(const vec2<int> &tile) const;
    bool tileIsPlayer1Edge(const vec2<int> &tile)  const;
    bool tileIsPlayer2Edge(const vec2<int> &tile)  const;
    bool tileIsFree(const vec2<int> &tile)  const;

public:
    TileType getTile(const vec2<int> &tile) const;
    vec2<int> getSize() const { return size; };
    std::vector<std::vector<TileType>> getBoard() const { return board; };
    void setBoard(std::vector<std::vector<TileType>> newBoard) { board = newBoard; };

    std::vector<vec2<int>> getNeighbouringOfType(vec2<int> tile, TileType type);

    bool playerOnePlace(const vec2<int> &tile);
    bool playerTwoPlace(const vec2<int> &tile);

    Board(const vec2<int> &size); // initialize board size
};

} // Hex namespace

std::ostream& operator<<(std::ostream& lhs, const Hex::TileType &rhs);
std::ostream& operator<<(std::ostream& lhs, const Hex::Board &rhs);
