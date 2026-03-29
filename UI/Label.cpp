#include "Label.h"

void Hex::UI::Label::draw() {
    drawHexagon();

    // Draw text
    static const int fontHeight = 30;
    vec2<int> topLeftText = getGlobalPosition() - vec2<int>{longness/2, fontHeight/2 + 5};
    windowPtr->draw_text(topLeftText, text, TDT4102::Color::white, fontHeight);

    drawChildren();
}