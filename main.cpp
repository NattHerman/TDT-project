#include <iostream>
#include <AnimationWindow.h>
#include "board.h"

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
    std::cout << "c * 2 = " << (c * 2) << std::endl;
    std::cout << "c * 2 = " << (c * 2.0) << std::endl;
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

    return 0;
}

