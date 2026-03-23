#include <iostream>

#include <AnimationWindow.h>

#include "Board.h"
#include "Game.h"
#include "HexagonalButton.h"
#include "UI/UINode.h"
#include "UI/HexGrid.h"

void drawBoundingBoxes(std::shared_ptr<Hex::UI::UINode> node, std::shared_ptr<TDT4102::AnimationWindow> windowPtr);

void test_vec2_operators() {
    Hex::vec2<int> a{1, 2};
    Hex::vec2<int> b{2, -2};
    Hex::vec2<double> c{0.5, 0.5};

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << std::endl;
    std::cout << "a + b = " << (a + b) << std::endl;
    std::cout << "a - b = " << (a - b) << std::endl;
    std::cout << "a * 2 = " << (a * 2) << std::endl;
    std::cout << "a * 2.5 = " << (a * 2.5) << std::endl;
    std::cout << "2.5 * a = " << (2.5 * a) << std::endl;
    std::cout << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "c * 3 = " << (c * 3) << std::endl;
    std::cout << "3 * c = " << (3 * c) << std::endl;
    std::cout << "c * 2 = " << (c * 2.0) << std::endl;
}

void test_board() {
    Hex::Board board{{5, 5}};

    for (int x = -10; x <= board.getSize().x + 9; ++x) {
        board.playerOnePlace({x, 0});
        board.playerTwoPlace({x, 1});
        board.playerOnePlace({x, x});
    }
    
    std::cout << board << std::endl;
}

void test_game() {
    std::vector<Hex::vec2<int>> moves = {
        {2, 0},
        {2, 2},
        {1, 2},
        {1, 3},
        {1, 3},
        {1, 4},
        {0, 4},
    };

    Hex::Game game{{5, 5}};

    for (Hex::vec2<int> move : moves) {
        bool playerOnesTurn = game.getTurn() == Hex::Turn::Player1;
        std::cout << (playerOnesTurn ? "Turn: Player One" : "Turn: Player Two") << std::endl;
        std::cout << "Move: " << move << std::endl;
        
        std::cout << "Move " << (game.takeTurn(move) ? "succeded" : "failed") << std::endl;
        std::cout << *(game.getBoard()) << std::endl;
    }
}

void test_hexbutton() {
    std::vector<Hex::vec2<int>> moves = {
        // {2, 0},
        // {2, 2},
        // {1, 2},
        // {1, 3},
        // {1, 3},
        // {1, 4},
        // {0, 4},
    };

    Hex::Game game{{11,11}};

    for (Hex::vec2<int> move : moves) {
        bool playerOnesTurn = game.getTurn() == Hex::Turn::Player1;
        std::cout << (playerOnesTurn ? "Turn: Player One" : "Turn: Player Two") << std::endl;
        std::cout << "Move: " << move << std::endl;
        
        std::cout << "Move " << (game.takeTurn(move) ? "succeded" : "failed") << std::endl;
        std::cout << *(game.getBoard()) << std::endl;
    }

    std::shared_ptr<TDT4102::AnimationWindow> windowPtr = std::make_shared<TDT4102::AnimationWindow>();
    windowPtr->setBackgroundColor(TDT4102::Color{0x3d404f});

    std::vector<Hex::HexagonalButton> buttons;
    Hex::vec2<int> boardSize = game.getBoard()->getSize();
    for (int x = 0; x < boardSize.x; ++x) {
        for (int y = 0; y < boardSize.y; ++y) {
            Hex::HexagonalButton button{{x, y}, windowPtr, game.getBoard()};
            std::function<void(Hex::vec2<int>)> func = std::bind(&Hex::Game::takeTurn, &game, button.getTile());
            button.setCallback(func);
            buttons.emplace_back(button);
        }
    }

    while (!windowPtr->should_close())
    {
        for (Hex::HexagonalButton &button : buttons) {
            button.update();
            button.draw();
        }
        windowPtr->next_frame();
    }
}

void test_hexGrid() {
    Hex::Game game{{11,11}};
    std::shared_ptr<TDT4102::AnimationWindow> windowPtr = std::make_shared<TDT4102::AnimationWindow>();
    windowPtr->setBackgroundColor(TDT4102::Color{0x3d404f});

    std::shared_ptr<Hex::UI::HexGrid> rootNode = std::make_shared<Hex::UI::HexGrid>(std::string("Hexagonal Grid"));
    rootNode->position.y = windowPtr->height() / 2;

    Hex::vec2<int> boardSize = game.getBoard()->getSize();
    for (int x = 0; x < boardSize.x; ++x) {
        for (int y = 0; y < boardSize.y; ++y) {
            // Initialize button
            std::shared_ptr<Hex::HexagonalButton> button = std::make_shared<Hex::HexagonalButton>(Hex::vec2<int>{x, y}, windowPtr, game.getBoard());
            std::function<void(Hex::vec2<int>)> func = std::bind(&Hex::Game::takeTurn, &game, button->getTile());
            button->setCallback(func);

            rootNode->addChild(button);
        }
    }

    while (!windowPtr->should_close()) {
        Hex::vec2<int> screenSize = {windowPtr->width(), windowPtr->height()};

        rootNode->update();
        rootNode->position = screenSize / 2 - rootNode->getBoundingBox().getCenter();
        // drawBoundingBoxes(rootNode, windowPtr);
        rootNode->draw();
        windowPtr->next_frame();
    }
}

void test_printChildren() {
    std::shared_ptr<Hex::UI::UINode> rootNode = std::make_shared<Hex::UI::UINode>("Root");
    rootNode->addChild(std::make_shared<Hex::UI::UINode>("Child0"));
    rootNode->addChild(std::make_shared<Hex::UI::UINode>("Child1"));
    rootNode->getChildren().at(0)->addChild(std::make_shared<Hex::UI::UINode>("GrandChild"));

    rootNode->printChildren();
}

void test_engulf() {
    Hex::rect<int> rectA{{1, 1}, {2, 1}};
    Hex::rect<int> rectB{{3, -1}, {1, 4}};

    rectA.engulf(rectB);

    std::cout << rectA << std::endl;
}

void drawBoundingBoxes(std::shared_ptr<Hex::UI::UINode> node, std::shared_ptr<TDT4102::AnimationWindow> windowPtr) {
    Hex::rect<int> box = node->getBoundingBox();
    windowPtr->draw_rectangle(box.position + node->getGlobalPosition(), box.size.x, box.size.y, TDT4102::Color::transparent, TDT4102::Color::light_pink);

    for (std::shared_ptr<Hex::UI::UINode> &child : node->getChildren()) {
        drawBoundingBoxes(child, windowPtr);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    test_hexGrid();

    return 0;
}
