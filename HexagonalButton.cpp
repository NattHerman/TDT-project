#include <cmath>
#include <iostream>

#include "HexagonalButton.h"

void Hex::HexagonalButton::updateVertices() {
    constexpr int vertexCount = 6; // because we're dealing with hexagons

    constexpr double angleIncrement = 2.0*M_PI / float(vertexCount);
    double angle = 0.0;
    for (int i = 0; i < vertexCount; ++i) {
        vec2<int> vertex{
            radius * std::cos(angle),
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

void Hex::HexagonalButton::update() {
    // Color stuff. VisualState -> Color
    visualState = getTileState();
    color = buttonVisualStateToColor.at(visualState);

    // Detect button press.
    if (callBack && windowPtr->is_left_mouse_button_down() && visualState == ButtonVisualState::Selected && firstFrameOfClick) {
        callBack(tile);
    }
    
    // Detect first frame of click.
    firstFrameOfClick = !windowPtr->is_left_mouse_button_down();

    updateChildren();
}


void Hex::HexagonalButton::draw() {
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

    vec2<int> globalPos = getGlobalPosition();
    double mouseDistanceSquared = pow(mousePos.x - globalPos.x, 2) + pow(mousePos.y - globalPos.y, 2);

    // std::cout << "mouse: " << mousePos << "tile: " << position << "\n";
    // std::cout << "dist:" << sqrt(mouseDistanceSquared) << "\n";
    // std::cout << "is selected: " << (mouseDistanceSquared < radius*radius ? "true" : "false") << std::endl;

    return mouseDistanceSquared < radius*radius;
}

void Hex::HexagonalButton::setCallback(std::function<void(vec2<int>)> callBack) {
    this->callBack = callBack;
}
