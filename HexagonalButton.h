#pragma once

#include "Board.h"
#include "Color.h"

namespace Hex {

enum class ButtonVisualState {
    Default,
    Selected,
    Player1,
    Player2,
};

class HexagonalButton {
    vec2<int> tile; // Which tile on the board this button corresponds to.
    int radius = 25;

    // Variables that change when updated
    ButtonVisualState visualState = ButtonVisualState::Default;
    TDT4102::Color color = TDT4102::Color::black;
    std::vector<TDT4102::Point> vertices;
    vec2<int> screenPosition;

    // Pointers to objects shared between all tile buttons
    std::shared_ptr<TDT4102::AnimationWindow> windowPtr;
    std::shared_ptr<Board> boardPtr;

    vec2<int> getScreenPosition() const;
    ButtonVisualState getTileState() const;

public:
    void update();
    void draw();

    // Constructor
    HexagonalButton(
        const vec2<int> &tile,
        const std::shared_ptr<TDT4102::AnimationWindow> &windowPtr,
        const std::shared_ptr<Board> &boardPtr
    ): tile{tile}, windowPtr{windowPtr}, boardPtr{boardPtr} {
        vertices.reserve(6); // 6 vertices in a hexagon
    };
};

} // namespace Hex