#pragma once

#include "Label.h"

namespace Hex {
namespace UI {

class Slider : public Label {


public:
    Slider(int minValue, int maxValue, int width, std::shared_ptr<TDT4102::AnimationWindow> windowPtr, std::string text = "", int longness = 0)
    : Label{text, longness + width, windowPtr} {};
};

} // namespace UI
} // namespace Hex
