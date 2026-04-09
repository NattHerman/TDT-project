#pragma once

#include "UINode.h"
#include "HexTile.h"

namespace Hex {
namespace UI {

// Aligns children to a hexagonal grid, where all children are HexTile
class HexGrid : public UINode {
    void alignChildrenPositions();

public:
    // Only children of type HexTile can be added.
    void addChild(const std::shared_ptr<HexTile> &child);
    std::shared_ptr<HexTile> getChild(int index) const { return std::static_pointer_cast<HexTile>(children.at(index)); }

    vec2<int> calculateTilePosition(Hex::vec2<int> tile, int radius);

    void update() override;

    HexGrid() = default;
    HexGrid(std::string name): UINode{name} {}
};

} // namespace UI
} // namespace Hex