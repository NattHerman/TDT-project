#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "vec2.h"

namespace Hex {
namespace Path {

class Node {
protected:
    double pathCost = 0.0;
    double heuristicCost = 0.0;

    std::shared_ptr<Node> from = nullptr;
    virtual double distance(std::shared_ptr<Node> other);

    virtual void updateHeuristic();

public:
    virtual std::vector<std::shared_ptr<Node>> getNeighbours() = 0;
    double updateCost() {
        if (from) pathCost = distance(from) + from->pathCost;
        return pathCost + heuristicCost;
    }
    double getCost() const { return pathCost + heuristicCost; }

    Node() { updateHeuristic(); };
    Node(std::shared_ptr<Node> from): from{from} {  updateHeuristic(); }
};

class AStar {
    std::vector<std::shared_ptr<Node>> open;
    std::unordered_map<std::shared_ptr<Node>, bool> closed;

    int getLowestCostIndex();
    void closeNode(int nodeToBeClosedIndex);
    void updateOpenNodes(std::shared_ptr<Node> currentNode);

    std::vector<std::shared_ptr<Node>> findPath(std::shared_ptr<Node> startNode, std::shared_ptr<Node> targetNode);
};

} // namespace Hex
} // namespace Hex
