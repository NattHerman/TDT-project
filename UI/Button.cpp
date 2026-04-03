#include <cmath>

#include "Button.h"

void Hex::UI::Button::setCallback(std::function<void(void)> callback) {
    this->callbackFunction = callback;
}

bool Hex::UI::Button::detectIsSelected() {
    vec2<double> mousePos = windowPtr->get_mouse_coordinates();

    vec2<int> globalPos = getGlobalPosition();
    double mouseDistanceLeftSquared = vec2<double>{mousePos.x - globalPos.x - longness/2, mousePos.y - globalPos.y}.length_squared();
    double mouseDistanceRightSquared = vec2<double>{mousePos.x - globalPos.x + longness/2, mousePos.y - globalPos.y}.length_squared();

    // you know, the longness rectangle, the bit between the ends < > of the hexagon.
    // (or rather an approximation, not 100% accurate but the difference is probably covered by the end circles)
    rect<double> longnessRect{
        {position.x, position.y - radius},
        {longness, 2*radius}
    };

    double radiusSquared = radius*radius;

    bool mouseInLeftCircle = mouseDistanceLeftSquared < radiusSquared;
    bool mouseInRightCircle = mouseDistanceRightSquared < radiusSquared;
    bool mouseWithinRect = longnessRect.inside(mousePos);

    return mouseWithinRect || mouseInLeftCircle || mouseInRightCircle;
}

void Hex::UI::Button::update() {
    if (!visible) return;

    selected = detectIsSelected();
    bool leftMouseDown = windowPtr->is_left_mouse_button_down();

    if (callbackFunction && selected && leftMouseDown && firstFrameOfClick) {
        callbackFunction();
    }

    firstFrameOfClick = !leftMouseDown;

    updateChildren();
}

void Hex::UI::Button::draw() {
    if (!visible) return;

    color = selected ? higlightColor : mainColor;

    drawHexagon();
    drawText();
    drawChildren();
}