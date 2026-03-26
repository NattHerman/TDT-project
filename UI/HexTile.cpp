#include <cmath>

#include "HexTile.h"

void Hex::UI::HexTile::updateVertices() {
    constexpr int vertexCount = 6; // because we're dealing with hexagons

    constexpr double angleIncrement = 2.0*M_PI / float(vertexCount);
    double angle = 0.0;
    for (int i = 0; i < vertexCount; ++i) {
        double isRightHalf = std::cos(angle) > 0;

        vec2<int> vertex{
            radius * std::cos(angle) + (isRightHalf ? longness : 0),
            radius * std::sin(angle)
        };

        if (vertices.size() <= i) {
            vertices.emplace_back(vertex);
        } else {
            vertices.at(i) = std::move(vertex);
        }

        angle += angleIncrement;
    }
}


void Hex::UI::HexTile::draw() {
    // Draw top quad
    vec2<int> globalPos = getGlobalPosition();

    windowPtr->draw_quad(
        vertices.at(0) + globalPos, vertices.at(1) + globalPos, vertices.at(2) + globalPos, vertices.at(3) + globalPos,
        color
    );

    // Draw bottom quad
    windowPtr->draw_quad(
        vertices.at(3) + globalPos, vertices.at(4) + globalPos, vertices.at(5) + globalPos, vertices.at(0) + globalPos,
        color
    );

    drawChildren();
}