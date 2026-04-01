#include "testing.h"


void drawBoundingBoxes(std::shared_ptr<Hex::UI::UINode> node, std::shared_ptr<TDT4102::AnimationWindow> windowPtr) {
    Hex::rect<int> box = node->getBoundingBox();
    windowPtr->draw_rectangle(box.position + node->getGlobalPosition(), box.size.x, box.size.y, TDT4102::Color::transparent, TDT4102::Color::light_pink);

    for (std::shared_ptr<Hex::UI::UINode> &child : node->getChildren()) {
        drawBoundingBoxes(child, windowPtr);
    }
}

void populateButtonGrid(std::shared_ptr<Hex::UI::HexGrid> grid, std::shared_ptr<Hex::Game> game, std::shared_ptr<TDT4102::AnimationWindow> windowPtr) {
    Hex::vec2<int> boardSize = game->getBoard()->getSize();
    for (int x = 0; x < boardSize.x; ++x) {
        for (int y = 0; y < boardSize.y; ++y) {
            grid->addChild(createButton({x, y}, windowPtr, game->getBoard(), game));
        }
    }

    // Create home player home rows
    for (int y = 1; y < boardSize.y; ++y) {
        grid->addChild(createButton({-1, y}, windowPtr, game->getBoard(), game));
    }

    for (int y = 0; y < boardSize.y - 1; ++y) {
        grid->addChild(createButton({boardSize.x, y}, windowPtr, game->getBoard(), game));
    }

    for (int x = 1; x < boardSize.x; ++x) {
        grid->addChild(createButton({x, -1}, windowPtr, game->getBoard(), game));
    }

    for (int x = 0; x < boardSize.x - 1; ++x) {
        grid->addChild(createButton({x, boardSize.y}, windowPtr, game->getBoard(), game));
    }
}

int main() {
    std::shared_ptr<TDT4102::AnimationWindow> windowPtr = std::make_shared<TDT4102::AnimationWindow>();
    windowPtr->setBackgroundColor(TDT4102::Color{0x3d404f});

    std::shared_ptr<Hex::Game> game = std::make_shared<Hex::Game>(Hex::vec2<int>{5, 5}, windowPtr);

    std::shared_ptr<Hex::UI::UINode> rootNode = std::make_shared<Hex::UI::UINode>(std::string("root"));

    std::shared_ptr<Hex::UI::HexGrid> grid = std::make_shared<Hex::UI::HexGrid>(std::string("Hexagonal Grid"));
    grid->position.y = windowPtr->height() / 2;
    rootNode->addChild(grid);

    std::shared_ptr<Hex::UI::TurnIndicator> turnIndicator = Hex::UI::TurnIndicator::create(game, windowPtr);
    rootNode->addChild(turnIndicator);

    std::shared_ptr<Hex::UI::Label> gameOverLabel = std::make_shared<Hex::UI::Label>("Game over", 140, windowPtr);
    gameOverLabel->setColor(TDT4102::Color{0x48814d});
    gameOverLabel->visible = false;
    gameOverLabel->position.y = 75;
    rootNode->addChild(gameOverLabel);

    populateButtonGrid(grid, game, windowPtr);

    while (!windowPtr->should_close()) {
        Hex::vec2<int> screenSize = {windowPtr->width(), windowPtr->height()};

        if (game->getState() != Hex::GameState::Ongoing) {
            gameOverLabel->visible = true;
            gameOverLabel->position.x = screenSize.x /2;
        }

        rootNode->update();
        grid->position = screenSize / 2 - grid->getBoundingBox().getCenter();
        // drawBoundingBoxes(rootNode, windowPtr);
        rootNode->draw();
        windowPtr->next_frame();
    }

    return 0;
}
