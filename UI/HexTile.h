#pragma once

#include "UINode.h"

namespace Hex {
namespace UI {

class HexGrid; // forward declaration (Claude fix, circular import)

class HexTile : public UINode {
protected:
    vec2<int> tile;
    int radius = 25;
    int longness = 0;

    TDT4102::Color color = TDT4102::Color::black;

    std::shared_ptr<TDT4102::AnimationWindow> windowPtr;
    std::vector<vec2<int>> vertices;
    void updateVertices();

    void setPosition(vec2<int> newPosition) { position = newPosition; }

    void updateBoundingBox() override {
        boundingBox = Hex::rect<int>{{-radius, -radius}, {radius*2 + longness, radius*2}};
    }

public:
    vec2<int> getTile() const { return tile; }
    int getRadius() const { return radius; }

    void draw() override;

    HexTile(vec2<int> tile, std::shared_ptr<TDT4102::AnimationWindow> windowPtr): tile{tile}, windowPtr{windowPtr} {
        vertices.reserve(6); // 6 vertices in a hexagon
        updateVertices();
    }
    HexTile(vec2<int> tile, std::string name, std::shared_ptr<TDT4102::AnimationWindow> windowPtr): UINode{name}, tile{tile}, windowPtr{windowPtr} {
        vertices.reserve(6); // 6 vertices in a hexagon
        updateVertices();
    }

    friend HexGrid;
};

} // namespace UI
} // namespace Hex
