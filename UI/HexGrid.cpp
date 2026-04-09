#include "HexGrid.h"
#include <cmath>

Hex::vec2<int> Hex::UI::HexGrid::calculateTilePosition(Hex::vec2<int> tile, int radius) {
    static constexpr double angle = 2.0*M_PI / 12.0;

    static const vec2<double> xDirection{std::cos(angle), std::sin(angle)};
    static const vec2<double> yDirection{std::cos(angle), -std::sin(angle)};

    vec2<double> alignedPosition = (xDirection * tile.x + yDirection * tile.y) * radius*2;

    return static_cast<vec2<int>>(alignedPosition);
}

void Hex::UI::HexGrid::alignChildrenPositions() {
    for (int i = 0; i < children.size(); ++i) {
        std::shared_ptr<HexTile> child = getChild(i);

        child->setPosition(calculateTilePosition(child->getTile(), child->getRadius()));
    }
}

void Hex::UI::HexGrid::addChild(const std::shared_ptr<HexTile> &child) {
    // Cast to std::shared_ptr<UINode>
    children.emplace_back(child);
    child->parent = shared_from_this();
}

void Hex::UI::HexGrid::update() {
    alignChildrenPositions();
    
    updateChildren();
}
