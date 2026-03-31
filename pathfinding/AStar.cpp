#include <iostream>
#include <cmath>

#include "AStar.h"
#include "BoardNode.h"

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
            neighbour->from = currentNode;
            if (!openContainsCurrent) {
                open.emplace_back(neighbour);
            }
        }
    }
}

std::vector<std::shared_ptr<Node>> AStar::findPath(std::shared_ptr<TDT4102::AnimationWindow> winPtr) {
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

        // Debugging, draw path
        // static constexpr double angle = 2.0*M_PI / 12.0;

        // static const vec2<double> xDirection{std::cos(angle), std::sin(angle)};
        // static const vec2<double> yDirection{std::cos(angle), -std::sin(angle)};

        // vec2<int> currentTile = std::static_pointer_cast<BoardNode>(current)->tile;
        // std::cout << currentTile << std::endl;
        // vec2<double> alignedPosition = (xDirection * currentTile.x + yDirection * currentTile.y) * 25*2 + vec2<double>{79, 384};
        // winPtr->draw_circle(alignedPosition, 30, TDT4102::Color::light_pink);
    }

    if (!foundPath) { return {}; }

    // create path from targetNode to startNode, following trail of froms.
    std::shared_ptr<Node> pathNode = targetNode;
    std::vector<std::shared_ptr<Node>> path{pathNode};
    while (pathNode->from) {
        pathNode = pathNode->from;
        path.push_back(pathNode);
    }

    // Reverse path, so that it goes from start to target.
    std::reverse(path.begin(), path.end());

    return path;
}

} // namespace Path
} // namespace Hex