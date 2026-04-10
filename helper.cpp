#include "testing.h"
#include "helper.h"
#include "UI/Button.h"

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
            grid->addChild(createButton({x, y}, windowPtr, game));
        }
    }

    // Create home player home rows
    for (int y = 1; y < boardSize.y; ++y) {
        grid->addChild(createButton({-1, y}, windowPtr, game));
    }

    for (int y = 0; y < boardSize.y - 1; ++y) {
        grid->addChild(createButton({boardSize.x, y}, windowPtr, game));
    }

    for (int x = 1; x < boardSize.x; ++x) {
        grid->addChild(createButton({x, -1}, windowPtr, game));
    }

    for (int x = 0; x < boardSize.x - 1; ++x) {
        grid->addChild(createButton({x, boardSize.y}, windowPtr, game));
    }
}

void newGame(std::shared_ptr<Hex::UI::HexGrid> grid, std::shared_ptr<Hex::Game> game, std::shared_ptr<TDT4102::AnimationWindow> windowPtr) {
    game->newGame();
    // populateButtonGrid(grid, game, windowPtr);
}

void forward(std::shared_ptr<int> index, std::shared_ptr<Hex::Game> game) {
    *index = std::min(*index + 1, int(game->getMoves().size()) - 1);
    game->displayMove(*index);
}

void backward(std::shared_ptr<int> index, std::shared_ptr<Hex::Game> game) {
    *index = std::max(*index - 1, 0);
    game->displayMove(*index);
}

void drawPoints(const std::vector<Hex::vec2<int>> &points, TDT4102::Color color, std::shared_ptr<TDT4102::AnimationWindow> windowPtr) {
    for (Hex::vec2<int> point : points) {
        windowPtr->draw_circle(point, 7, color);
    }
}

std::vector<Hex::vec2<int>> tilesToPoints(const std::vector<Hex::vec2<int>> &tiles, std::shared_ptr<Hex::UI::HexGrid> grid) {
    std::vector<Hex::vec2<int>> points;
    points.reserve(tiles.size());
    for (Hex::vec2<int> tile : tiles) {
        if (tile.x < 0 || tile.x > 10 || tile.y < 0 || tile.y > 10) continue;
        points.emplace_back(grid->calculateTilePosition(tile, 25) + grid->getGlobalPosition());
    }

    return std::move(points);
}