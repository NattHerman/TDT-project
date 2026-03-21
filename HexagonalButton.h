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

inline std::unordered_map<ButtonVisualState, TDT4102::Color> buttonVisualStateToColor {
    {ButtonVisualState::Default, TDT4102::Color{0x767676}},
    {ButtonVisualState::Selected, TDT4102::Color{0xa1a1a1}},
    {ButtonVisualState::Player1, TDT4102::Color{0xa86052}},
    {ButtonVisualState::Player2, TDT4102::Color{0x5676b1}},
};

class HexagonalButton {
    const vec2<int> tile; // Which tile on the board this button corresponds to.
    int radius = 25;

    // Variables that change when updated
    ButtonVisualState visualState = ButtonVisualState::Default;
    TDT4102::Color color = TDT4102::Color::black;
    std::vector<TDT4102::Point> vertices;
    vec2<int> screenPosition = {0, 0};

    // Pointers to objects shared between all tile buttons
    std::shared_ptr<TDT4102::AnimationWindow> windowPtr;
    std::shared_ptr<Board> boardPtr;

    vec2<int> getScreenPosition() const;
    bool getButtonIsSelected() const; // True when cursor is hovering over button.
    ButtonVisualState getTileState() const;

    // Function that is called when button is pressed, with tile as input.
    std::function<void(vec2<int>)> callBack;
    bool firstFrameOfClick = false;

public:
    void update();
    void draw();

    void setCallback(std::function<void(vec2<int>)> callBack);
    vec2<int> getTile() const { return tile; };
    int getRadius() const { return radius; };

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