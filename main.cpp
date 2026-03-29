#include <iostream>

#include <AnimationWindow.h>

#include "Board.h"
#include "Game.h"
#include "HexagonalButton.h"
#include "UI/UINode.h"
#include "UI/HexGrid.h"
#include "UI/Label.h"
#include "UI/TurnIndicator.h"

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

std::shared_ptr<Hex::HexagonalButton> createButton(
    Hex::vec2<int> tile,
    const std::shared_ptr<TDT4102::AnimationWindow> &windowPtr,
    const std::shared_ptr<Hex::Board> &boardPtr,
    std::shared_ptr<Hex::Game> gamePtr
) {
    std::shared_ptr<Hex::HexagonalButton> button = std::make_shared<Hex::HexagonalButton>(tile, windowPtr, gamePtr->getBoard());
    std::function<void(Hex::vec2<int>)> func = std::bind(&Hex::Game::takeTurn, gamePtr, button->getTile());
    button->setCallback(func);

    return button;
}

void test_hexGrid() {
    std::shared_ptr<TDT4102::AnimationWindow> windowPtr = std::make_shared<TDT4102::AnimationWindow>();
    windowPtr->setBackgroundColor(TDT4102::Color{0x3d404f});

    std::shared_ptr<Hex::Game> game = std::make_shared<Hex::Game>(Hex::vec2<int>{11,11}, windowPtr);

    std::shared_ptr<Hex::UI::UINode> rootNode = std::make_shared<Hex::UI::UINode>(std::string("root"));

    std::shared_ptr<Hex::UI::HexGrid> grid = std::make_shared<Hex::UI::HexGrid>(std::string("Hexagonal Grid"));
    grid->position.y = windowPtr->height() / 2;
    rootNode->addChild(grid);

    std::shared_ptr<Hex::UI::TurnIndicator> turnIndicator = Hex::UI::TurnIndicator::create(game, windowPtr);
    rootNode->addChild(turnIndicator);

    Hex::vec2<int> boardSize = game->getBoard()->getSize();
    for (int x = 0; x < boardSize.x; ++x) {
        for (int y = 0; y < boardSize.y; ++y) {
            grid->addChild(createButton({x, y}, windowPtr, game->getBoard(), game));
        }
    }

    // Create home player home rows
    for (int y = 0; y < boardSize.y; ++y) {
        grid->addChild(createButton({-1, y}, windowPtr, game->getBoard(), game));
    }

    for (int y = 0; y < boardSize.y; ++y) {
        grid->addChild(createButton({boardSize.x, y}, windowPtr, game->getBoard(), game));
    }

    for (int x = 0; x < boardSize.x; ++x) {
        grid->addChild(createButton({x, -1}, windowPtr, game->getBoard(), game));
    }

    for (int x = 0; x < boardSize.x; ++x) {
        grid->addChild(createButton({x, boardSize.y}, windowPtr, game->getBoard(), game));
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

    // std::shared_ptr<TDT4102::AnimationWindow> windowPtr = std::make_shared<TDT4102::AnimationWindow>();
    // windowPtr->setBackgroundColor(TDT4102::Color{0x3d404f});

    // Hex::Game game{{11,11}, windowPtr};

    // std::shared_ptr<Hex::UI::HexGrid> rootNode = std::make_shared<Hex::UI::HexGrid>(std::string("Hexagonal Grid"));
    // rootNode->position.y = windowPtr->height() / 2;

    // Hex::UI::Label label{"Player 1", 75, windowPtr};

    // rootNode->addChild(std::make_shared<Hex::UI::Label>(label));

    // while (!windowPtr->should_close()) {
    //     Hex::vec2<int> screenSize = {windowPtr->width(), windowPtr->height()};

    //     rootNode->update();
    //     rootNode->position = screenSize / 2 - rootNode->getBoundingBox().getCenter();
    //     // drawBoundingBoxes(rootNode, windowPtr);
    //     rootNode->draw();

    //     int height = 30;
    //     windowPtr->draw_rectangle({0, 10}, 10, height, TDT4102::Color::dark_green);
    //     windowPtr->draw_rectangle({5, 0}, 10, 10, TDT4102::Color::green);
    //     windowPtr->draw_text({0, 0}, "Test text Test text Test text", TDT4102::Color::black, height + height/3);

    //     windowPtr->next_frame();
    // }

    test_hexGrid();

    return 0;
}
