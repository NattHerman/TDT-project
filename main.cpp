#include "testing.h"
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

void newGame(std::shared_ptr<Hex::UI::HexGrid> grid, std::shared_ptr<Hex::Game> game, std::shared_ptr<TDT4102::AnimationWindow> windowPtr) {
    game->newGame();
    // populateButtonGrid(grid, game, windowPtr);
}

int main() {
    // Where the game state file is stored
    std::filesystem::path savePath{"savedata/state.hex"};

    // Initialize AnimationWindow
    std::shared_ptr<TDT4102::AnimationWindow> windowPtr = std::make_shared<TDT4102::AnimationWindow>();
    windowPtr->setBackgroundColor(TDT4102::Color{0x3d404f});

    // Initialize Hex Game object
    std::shared_ptr<Hex::Game> game = std::make_shared<Hex::Game>(Hex::vec2<int>{11, 11}, savePath, windowPtr);

    // Root Node contains all nodes to be drawn, as children
    std::shared_ptr<Hex::UI::UINode> rootNode = std::make_shared<Hex::UI::UINode>(std::string("root"));

    // Grid stores and aligns all hex buttons
    std::shared_ptr<Hex::UI::HexGrid> grid = std::make_shared<Hex::UI::HexGrid>(std::string("Hexagonal Grid"));
    grid->position.y = windowPtr->height() / 2;
    rootNode->addChild(grid);

    // Turn Indicator displays whose turn it is
    std::shared_ptr<Hex::UI::TurnIndicator> turnIndicator = Hex::UI::TurnIndicator::create(game, windowPtr);
    rootNode->addChild(turnIndicator);

    // Game over label appears when game is over (duh)
    std::shared_ptr<Hex::UI::Label> gameOverLabel = std::make_shared<Hex::UI::Label>("Player x won!", 160, windowPtr);
    gameOverLabel->setColor(TDT4102::Color{0x48814d}); // greenish color
    gameOverLabel->visible = false;
    gameOverLabel->position.y = 75;
    rootNode->addChild(gameOverLabel);

    std::shared_ptr<Hex::UI::Button> forfeitButton = std::make_shared<Hex::UI::Button>("Forfeit", 90, windowPtr);
    forfeitButton->setColor(TDT4102::Color{0x48814d});
    forfeitButton->setHighlighColor(TDT4102::Color{0x68a26c});
    forfeitButton->setCallback(std::bind(&Hex::Game::forfeit, game));
    rootNode->addChild(forfeitButton);

    std::shared_ptr<Hex::UI::Button> newgameButton = std::make_shared<Hex::UI::Button>("New Game", 140, windowPtr);
    newgameButton->setColor(TDT4102::Color{0x48814d});
    newgameButton->setHighlighColor(TDT4102::Color{0x68a26c});
    newgameButton->setCallback(std::bind(&newGame, grid, game, windowPtr));
    rootNode->addChild(newgameButton);

    // Fills the grid with hexagonal buttons
    populateButtonGrid(grid, game, windowPtr);

    while (!windowPtr->should_close()) {
        Hex::vec2<int> screenSize = {windowPtr->width(), windowPtr->height()};

        bool gameIsOngoing = game->getState() == Hex::GameState::Ongoing;
        bool player1Won = game->getState() == Hex::GameState::Player1Won;
        
        // Make GameOver thing visible when game ends
        gameOverLabel->visible = !gameIsOngoing;
        newgameButton->visible = !gameIsOngoing;
        forfeitButton->visible = gameIsOngoing;
        if (!gameIsOngoing) {
            gameOverLabel->position.x = screenSize.x / 2;
            gameOverLabel->text = "Player " + std::string(player1Won ? "1" : "2") + " won!";
            gameOverLabel->setColor(TDT4102::Color(player1Won ? 0xa86052 : 0x5676b1)); // stupid magic numbers
        }

        // Align buttons
        forfeitButton->position = {(screenSize.x * 3) / 4, 100}; // im a wizard, so many magic numbers
        if (game->getTurn() == Hex::Turn::Player1)
        { forfeitButton->position.x /= 3; }
        newgameButton->position = {(screenSize.x * 3) / 4, screenSize.y - 75};

        rootNode->update();
        grid->position = screenSize / 2 - grid->getBoundingBox().getCenter();
        // drawBoundingBoxes(rootNode, windowPtr);
        rootNode->draw();
        windowPtr->next_frame();
    }

    return 0;
}
