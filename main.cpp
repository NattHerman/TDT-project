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

    std::shared_ptr<Hex::UI::Button> undoButton = std::make_shared<Hex::UI::Button>("Undo", 70, windowPtr);
    undoButton->setColor(TDT4102::Color{0x48814d});
    undoButton->setHighlighColor(TDT4102::Color{0x68a26c});
    undoButton->setCallback(std::bind(&Hex::Game::undo, game));
    rootNode->addChild(undoButton);

    std::shared_ptr<int> moveIndex = std::make_shared<int>(0);
    std::shared_ptr<Hex::UI::Button> forwardButton = std::make_shared<Hex::UI::Button>("Forward", 120, windowPtr);
    forwardButton->setColor(TDT4102::Color{0x48814d});
    forwardButton->setHighlighColor(TDT4102::Color{0x68a26c});
    forwardButton->setCallback(std::bind(&forward, moveIndex, game));
    rootNode->addChild(forwardButton);

    std::shared_ptr<Hex::UI::Button> backwardButton = std::make_shared<Hex::UI::Button>("Backward", 120, windowPtr);
    backwardButton->setColor(TDT4102::Color{0x48814d});
    backwardButton->setHighlighColor(TDT4102::Color{0x68a26c});
    backwardButton->setCallback(std::bind(&backward, moveIndex, game));
    rootNode->addChild(backwardButton);

    // Fills the grid with hexagonal buttons
    populateButtonGrid(grid, game, windowPtr);

    while (!windowPtr->should_close()) {
        Hex::vec2<int> screenSize = {windowPtr->width(), windowPtr->height()};

        bool gameIsOngoing = game->getState() == Hex::GameState::Ongoing;
        bool player1Won = game->getState() == Hex::GameState::Player1Won;
        
        // Some things are only visible when game is over, or when game is ongoing
        gameOverLabel->visible = !gameIsOngoing;
        newgameButton->visible = !gameIsOngoing;
        forwardButton->visible = !gameIsOngoing;
        backwardButton->visible = !gameIsOngoing;
        forfeitButton->visible = gameIsOngoing;
        undoButton->visible = gameIsOngoing;
        if (!gameIsOngoing) {
            gameOverLabel->position.x = screenSize.x / 2;
            gameOverLabel->text = "Player " + std::string(player1Won ? "1" : "2") + " won!";
            TDT4102::Color winnerColor{player1Won ? 0xa86052 : 0x5676b1};
            gameOverLabel->setColor(winnerColor); // stupid magic numbers
        } else {
            *moveIndex = 0;
        }

        // Align buttons
        forfeitButton->position = {(screenSize.x * 3) / 4, 100}; // im a wizard, so many magic numbers
        if (game->getTurn() == Hex::Turn::Player1)
        { forfeitButton->position.x /= 3; }
        newgameButton->position = {(screenSize.x * 3) / 4, screenSize.y - 75};
        undoButton->position = {screenSize.x / 4, screenSize.y - 75};
        forwardButton->position = {screenSize.x / 4, screenSize.y - 100};
        backwardButton->position = {screenSize.x / 4, screenSize.y - 50};

        rootNode->update();
        grid->position = screenSize / 2 - grid->getBoundingBox().getCenter();
        // drawBoundingBoxes(rootNode, windowPtr);
        rootNode->draw();
        // Display winning path
        if (!gameIsOngoing && game->getCurrentDisplayedMoveIndex() == game->getMoves().size() - 1) {
            TDT4102::Color pathColor{player1Won ? 0xdc9c8e : 0xa8bdde}; // magic wand: --- ~*  ~* 
            drawPoints(tilesToPoints(game->getWinningTilePath(), grid), pathColor, windowPtr);
        }
        windowPtr->next_frame();
    }

    return 0;
}
