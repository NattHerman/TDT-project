#include "TurnIndicator.h"
#include "HexagonalButton.h"

void Hex::UI::TurnIndicator::update() {
    int halfWindowWidth = windowPtr->width()/2;
    position.x = halfWindowWidth;
    position.y = 50; // Just below the top of the screen. magic number, sorry

    // windowPtr->draw_circle(position, 20);

    // getChild(0)->position.x = halfWindowWidth/2;
    // getChild(1)->position.x = -halfWindowWidth/2;

    updateChildren();
}

void Hex::UI::TurnIndicator::draw() {
    if (gamePtr->getTurn() == Turn::Player1) {
        getChild(0)->setColor(buttonVisualStateToColor.at(ButtonVisualState::Player1));
        getChild(1)->setColor(buttonVisualStateToColor.at(ButtonVisualState::Default));
    } else {
        getChild(0)->setColor(buttonVisualStateToColor.at(ButtonVisualState::Default));
        getChild(1)->setColor(buttonVisualStateToColor.at(ButtonVisualState::Player2));
    }

    drawChildren();
}
