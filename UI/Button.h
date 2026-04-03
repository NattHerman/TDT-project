#pragma once

#include "Label.h"

namespace Hex {
namespace UI {

class Button : public Label {
    bool detectIsSelected();
    bool firstFrameOfClick = false;

protected:
    std::function<void(void)> callbackFunction;
    TDT4102::Color higlightColor = TDT4102::Color::light_gray;
    TDT4102::Color mainColor = TDT4102::Color::gray;

    bool selected = false;

    void update() override;
    void draw() override;

public:
    void setCallback(std::function<void(void)> callback);
    void setHighlighColor(TDT4102::Color color) { higlightColor = color; };
    void setColor(TDT4102::Color color) override { mainColor = color; };

    Button(std::string text, int longness, std::shared_ptr<TDT4102::AnimationWindow> windowPtr): Label{text, longness, windowPtr} {}
};

} // namespace UI
} // namespace Hex
