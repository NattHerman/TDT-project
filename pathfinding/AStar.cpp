#include "AStar.h"


int Hex::Path::AStar::getLowestCostIndex() {
    int lowestCostIndex = 0;
    std::shared_ptr<Hex::Path::Node> lowestCostNode = open.at(lowestCostIndex);
    double lowestCost = lowestCostNode->getCost();

    for (int i = 0; i < open.size(); ++i) {
        std::shared_ptr<Hex::Path::Node> node = open.at(i);
        if (node->getCost() < lowestCost) {
            lowestCostNode = node;
            lowestCostIndex = i;
        }
    }

    return lowestCostIndex;
}

void Hex::Path::AStar::closeNode(int nodeToBeClosedIndex) {
    // Close node
    closed.insert({open.at(nodeToBeClosedIndex), true});

    // Remove node from open:
    // Replace with node from back. 
    open.at(nodeToBeClosedIndex) = open.back();
    // Remove node at back.
    open.pop_back();
}

void Hex::Path::AStar::updateOpenNodes(std::shared_ptr<Node> currentNode) {
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

        if (!openContainsCurrent) {
            open.emplace_back(neighbour);
        }
    }
}

std::vector<std::shared_ptr<Hex::Path::Node>> Hex::Path::AStar::findPath(std::shared_ptr<Hex::Path::Node> startNode, std::shared_ptr<Hex::Path::Node> targetNode) {
    std::vector<std::shared_ptr<Node>> open;
    std::unordered_map<std::shared_ptr<Node>, bool> closed;

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
}