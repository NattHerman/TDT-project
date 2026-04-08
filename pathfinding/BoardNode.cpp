#include <cmath>
#include <iostream>

#include "BoardNode.h"

namespace Hex {
namespace Path {

std::shared_ptr<BoardNode> BoardNodeGenerator::getNode(vec2<int> tile) {
    vec2<int> tileIndex = tile + vec2{1, 1};
    int nodeIndex = tileIndex.x + tileIndex.y * (boardPtr->getSize().x + 2); // + 2 to account for edges
    if (!nodes.at(nodeIndex)) {
        nodes.at(nodeIndex) = std::make_shared<BoardNode>(shared_from_this(), tile);
    }

    return nodes.at(nodeIndex);
}


double BoardNode::distance(std::shared_ptr<Node> other) {
    static constexpr double _angle = M_PI * 2.0 / 12; 
    static const vec2<double> xAxis{std::cos(_angle), std::sin(_angle)};
    static const vec2<double> yAxis{std::cos(_angle), -std::sin(_angle)};

    std::shared_ptr<BoardNode> otherBoardNode = std::static_pointer_cast<BoardNode>(other);

    // Position of tiles in rectangular coordinates, not hexagonal.
    vec2<double> position = xAxis * tile.x + yAxis * tile.y;
    vec2<double> otherPosition = xAxis * otherBoardNode->tile.x + yAxis * otherBoardNode->tile.y;

    return (position - otherPosition).length();
}

std::vector<std::shared_ptr<Node>> BoardNode::getNeighbours() {
    std::vector<vec2<int>> neighbourTiles = boardPtr->getNeighbouringOfType(tile, generator->tileType);
    std::vector<std::shared_ptr<Node>> neigbourNodes;
    neigbourNodes.reserve(6);

    for (vec2<int> neighbourTile : neighbourTiles) {
        std::shared_ptr<BoardNode> neigbourNode = generator->getNode(neighbourTile); 
        neigbourNodes.emplace_back(neigbourNode);
    }

    return neigbourNodes;
}

} // namespace Hex
} // namespace Hex
