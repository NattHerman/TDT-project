#include "HexagonalButton.h"
#include <cmath>

Hex::vec2<int> Hex::HexagonalButton::getScreenPosition() const {
    // vec2<double> boardPos = static_cast<vec2<double>>(parentGUI->boardPosition);
    vec2<double> boardPos = {windowPtr->width() / 10, windowPtr->height() / 2};

    static constexpr double angle = 2.0*M_PI / 12.0;

    static const vec2<double> xDirection{std::cos(angle), std::sin(angle)};
    static const vec2<double> yDirection{std::cos(angle), -std::sin(angle)};

    vec2<double> screenPos = boardPos + (xDirection * tile.x + yDirection * tile.y) * radius*2;

    return static_cast<vec2<int>>(screenPos);
}

void Hex::HexagonalButton::update() {
    constexpr int vertexCount = 6;

    vec2<int> screenPos = getScreenPosition();

    constexpr double angleIncrement = 2.0*M_PI / float(vertexCount);
    double angle = 0.0;
    for (int i = 0; i < vertexCount; ++i) {
        vec2<double> vertex{
            radius * std::cos(angle) + screenPos.x,
            radius * std::sin(angle) + screenPos.y
        };

        if (vertices.size() <= i) {
            vertices.emplace_back(vertex);
        } else {
            vertices.at(i) = std::move(vertex);
        }

        angle += angleIncrement;
    }
}


void Hex::HexagonalButton::draw() {
    if (vertices.size() < 6) {
        throw std::runtime_error("update() must be called before draw().");
    }
  
    // Draw top quad
    windowPtr->draw_quad(
        vertices.at(0), vertices.at(1), vertices.at(2), vertices.at(3),
        TDT4102::Color::cadet_blue
    );

    // Draw bottom quad
    windowPtr->draw_quad(
        vertices.at(3), vertices.at(4), vertices.at(5), vertices.at(0),
        TDT4102::Color::cadet_blue
    );
}
