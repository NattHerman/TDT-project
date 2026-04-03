#pragma once

#include "UINode.h"
#include "HexTile.h"

namespace Hex {
namespace UI {

class Label : public HexTile {
public:
    std::string text;

    virtual void setColor(TDT4102::Color newColor) { color = newColor; }

    virtual void drawText();
    virtual void draw() override;

    Label(std::string text, int longness, std::shared_ptr<TDT4102::AnimationWindow> windowPtr)
    : HexTile{{0, 0}, windowPtr, longness}, text{text} {}
};

} // namespace UI
} // namespace Hex
