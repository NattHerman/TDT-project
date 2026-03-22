#pragma once

#include "UINode.h"

namespace Hex {
namespace UI {

class HexGrid; // forward declaration — just tells the compiler it exists (Claude fix circular import)

class HexTile : public UINode {
protected:
    vec2<int> tile;
    int radius = 25;

    void setPosition(vec2<int> newPosition) { position = newPosition; }

public:
    vec2<int> getTile() const { return tile; }
    int getRadius() const { return radius; }

    HexTile(vec2<int> tile): tile{tile} {}
    HexTile(vec2<int> tile, std::string name): UINode{name}, tile{tile} {}

    friend HexGrid;
};

} // namespace UI
} // namespace Hex