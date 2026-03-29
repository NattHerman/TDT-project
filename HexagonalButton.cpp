#include <cmath>
#include <iostream>

#include "HexagonalButton.h"

void Hex::HexagonalButton::update() {
    // Color stuff. VisualState -> Color
    visualState = getTileState();
    color = buttonVisualStateToColor.at(visualState);

    if (!windowPtr) {
        throw std::runtime_error("windowPtr is null");
    }


    // Detect button press.
    if (callBack && windowPtr->is_left_mouse_button_down() && visualState == ButtonVisualState::Selected && firstFrameOfClick) {
        callBack(tile); // callback is game.takeTurn 
    }
    
    // Detect first frame of click.
    firstFrameOfClick = !windowPtr->is_left_mouse_button_down();

    updateChildren();
}

Hex::ButtonVisualState Hex::HexagonalButton::getTileState() const {
    TileType boardTileType = boardPtr->getTile(tile);

    if (boardTileType == TileType::StonePlayerOne || boardTileType == TileType::EdgePlayerOne) {
        return ButtonVisualState::Player1;
    } if (boardTileType == TileType::StonePlayerTwo || boardTileType == TileType::EdgePlayerTwo) {
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
