#include "HexagonalButton.h"
#include <cmath>
#include <iostream>

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

    screenPosition = getScreenPosition();

    constexpr double angleIncrement = 2.0*M_PI / float(vertexCount);
    double angle = 0.0;
    for (int i = 0; i < vertexCount; ++i) {
        vec2<double> vertex{
            radius * std::cos(angle) + screenPosition.x,
            radius * std::sin(angle) + screenPosition.y
        };

        if (vertices.size() <= i) {
            vertices.emplace_back(vertex);
        } else {
            vertices.at(i) = std::move(vertex);
        }

        angle += angleIncrement;
    }

    // Color stuff. VisualState -> Color
    visualState = getTileState();
    color = buttonVisualStateToColor.at(visualState);
}


void Hex::HexagonalButton::draw() {
    if (vertices.size() < 6) {
        throw std::runtime_error("update() must be called before draw().");
    }
  
    // Draw top quad
    windowPtr->draw_quad(
        vertices.at(0), vertices.at(1), vertices.at(2), vertices.at(3),
        color
    );

    // Draw bottom quad
    windowPtr->draw_quad(
        vertices.at(3), vertices.at(4), vertices.at(5), vertices.at(0),
        color
    );
}

Hex::ButtonVisualState Hex::HexagonalButton::getTileState() const {
    TileType boardTileType = boardPtr->getTile(tile);

    if (boardTileType == TileType::StonePlayerOne) {
        return ButtonVisualState::Player1;
    } if (boardTileType == TileType::StonePlayerTwo) {
        return ButtonVisualState::Player2;
    }

    if (getButtonIsSelected()) {
        return ButtonVisualState::Selected;
    }

    return ButtonVisualState::Default;
}

bool Hex::HexagonalButton::getButtonIsSelected() const {
    vec2<double> mousePos = windowPtr->get_mouse_coordinates();

    double mouseDistanceSquared = pow(mousePos.x - screenPosition.x, 2) + pow(mousePos.y - screenPosition.y, 2);

    // std::cout << "mouse: " << mousePos << "tile: " << screenPosition << "\n";
    // std::cout << "dist:" << sqrt(mouseDistanceSquared) << "\n";
    // std::cout << "is selected: " << (mouseDistanceSquared < radius*radius ? "true" : "false") << std::endl;

    return mouseDistanceSquared < radius*radius;
}
