#pragma once

#include "Color.h"

#include "Game.h"
#include "UI/UINode.h"
#include "UI/HexTile.h"

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

class HexagonalButton : public UI::HexTile {
    ButtonVisualState visualState = ButtonVisualState::Default;

    std::shared_ptr<Game> gamePtr;

    bool getButtonIsSelected() const; // True when cursor is hovering over button.
    ButtonVisualState getTileState() const;

    // Function that is called when button is pressed, with tile as input.
    std::function<void(vec2<int>, bool)> callBack;
    bool firstFrameOfClick = false;

public:
    void update() override;

    void setCallback(std::function<void(vec2<int>, bool)> callBack);
    vec2<int> getTile() const { return tile; };
    int getRadius() const { return radius; };

    HexagonalButton(
        const vec2<int> &tile,
        const std::shared_ptr<TDT4102::AnimationWindow> &windowPtr,
        const std::shared_ptr<Game> &gamePtr
    ):  HexTile{tile, "HexButton{" + std::to_string(tile.x) + ", " + std::to_string(tile.y) + "}", windowPtr},
        gamePtr{gamePtr} {};
};

} // namespace Hex