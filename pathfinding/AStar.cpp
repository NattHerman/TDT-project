#include "AStar.h"

namespace Hex {
namespace Path {

int AStar::getLowestCostIndex() {
    int lowestCostIndex = 0;
    std::shared_ptr<Node> lowestCostNode = open.at(lowestCostIndex);
    double lowestCost = lowestCostNode->getCost();

    for (int i = 0; i < open.size(); ++i) {
        std::shared_ptr<Node> node = open.at(i);
        if (node->getCost() < lowestCost) {
            lowestCostNode = node;
            lowestCostIndex = i;
        }
    }

    return lowestCostIndex;
}

void AStar::closeNode(int nodeToBeClosedIndex) {
    // Close node
    closed.insert({open.at(nodeToBeClosedIndex), true});

    // Remove node from open:
    // Replace with node from back. 
    open.at(nodeToBeClosedIndex) = open.back();
    // Remove node at back.
    open.pop_back();
}

void AStar::updateOpenNodes(std::shared_ptr<Node> currentNode) {
    std::vector<std::shared_ptr<Node>> neighbours = currentNode->getNeighbours();

    for (std::shared_ptr<Node> neighbour : neighbours) {
        if (closed.contains(neighbour)) continue;

        bool openContainsCurrent = false;
        for (std::shared_ptr<Node> openNode : open) {
            if (neighbour == openNode) {
                openContainsCurrent = true;
                break;
            }
        }

        double oldPath = neighbour->pathCost;
        double newPath = currentNode->pathCost + currentNode->distance(neighbour);
        bool newPathToNeighbourIsShorter = newPath < oldPath;

        if (!openContainsCurrent || newPathToNeighbourIsShorter) {
            neighbour->heuristicCost = neighbour->distance(targetNode);
            neighbour->pathCost = newPath;
            if (!openContainsCurrent) {
                open.emplace_back(neighbour);
            }
        }
    }
}

std::vector<std::shared_ptr<Node>> AStar::findPath() {
    std::shared_ptr<Node> current = startNode;
    open.push_back(current);

    bool foundPath = false;

    while (open.size() > 0) {
        int lowestCostIndex = getLowestCostIndex();
        current = open.at(lowestCostIndex);
        closeNode(lowestCostIndex);

        foundPath = current == targetNode;
        if (foundPath) break;

        updateOpenNodes(current);
    }

    if (!foundPath) { return {}; }

    // create path from targetNode
    return {}; // 
}

} // namespace Path
} // namespace Hex