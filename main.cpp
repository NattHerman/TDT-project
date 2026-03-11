#include <iostream>
#include <AnimationWindow.h>
#include "board.h"
#include "game.h"

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
    std::cout << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "c * 3 = " << (c * 3) << std::endl;
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
        std::cout << game.getBoard() << std::endl;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    
    /*
    TDT4102::AnimationWindow window;
    TDT4102::Image image("test.jpg");

    int xPosition = 0;
    while(!window.should_close()) {
        xPosition = xPosition + 1;
        if(xPosition > 200*2) {
            xPosition = 0;
        }
        TDT4102::Point position {xPosition, 100};
        window.draw_rectangle(position, 100, 100);
        window.draw_image(position, image, 300, 300, 45.0, {150, 150});

        window.next_frame();
    }
    */

    test_game();

    return 0;
}

