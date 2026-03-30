#pragma once

#include <vector>

#include "AStar.h"
#include "Board.h"

namespace Hex {
namespace Path {

class BoardNode;

class BoardNodeGenerator : public std::enable_shared_from_this<BoardNodeGenerator> {
    std::shared_ptr<Board> boardPtr;

    Hex::TileType tileType;

    std::vector<std::shared_ptr<BoardNode>> nodes;
public:
    std::shared_ptr<BoardNode> getNode(vec2<int> tile);

    BoardNodeGenerator(std::shared_ptr<Board> boardPtr): boardPtr{boardPtr} {
        vec2<int> boardSize = boardPtr->getSize();
        nodes.resize(boardSize.x * boardSize.y);
    }

    friend BoardNode;
};

class BoardNode : public Node {
protected:
    vec2<int> tile;

    std::shared_ptr<Board> boardPtr;
    std::shared_ptr<BoardNodeGenerator> generator;

    double distance(std::shared_ptr<Node> other) override;

    BoardNode(std::shared_ptr<BoardNodeGenerator> generator, std::shared_ptr<Node> from): Node{from}, boardPtr{generator->boardPtr}, generator{generator} {}

public:
    std::vector<std::shared_ptr<Node>> getNeighbours() override;

    BoardNode(std::shared_ptr<BoardNodeGenerator> generator): boardPtr{generator->boardPtr}, generator{generator} {}
};

} // namespace Hex
} // namespace Hex
