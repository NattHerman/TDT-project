#pragma once

#include "testing.h"
#include "helper.h"
#include "UI/Button.h"

void drawBoundingBoxes(std::shared_ptr<Hex::UI::UINode> node, std::shared_ptr<TDT4102::AnimationWindow> windowPtr);

void populateButtonGrid(std::shared_ptr<Hex::UI::HexGrid> grid, std::shared_ptr<Hex::Game> game, std::shared_ptr<TDT4102::AnimationWindow> windowPtr);

void newGame(std::shared_ptr<Hex::UI::HexGrid> grid, std::shared_ptr<Hex::Game> game, std::shared_ptr<TDT4102::AnimationWindow> windowPtr);

void forward(std::shared_ptr<int> index, std::shared_ptr<Hex::Game> game);

void backward(std::shared_ptr<int> index, std::shared_ptr<Hex::Game> game);

void drawPoints(const std::vector<Hex::vec2<int>> &points, TDT4102::Color color, std::shared_ptr<TDT4102::AnimationWindow> windowPtr);

std::vector<Hex::vec2<int>> tilesToPoints(const std::vector<Hex::vec2<int>> &tiles, std::shared_ptr<Hex::UI::HexGrid> grid);

std::shared_ptr<Hex::HexagonalButton> createButton(
    Hex::vec2<int> tile,
    const std::shared_ptr<TDT4102::AnimationWindow> &windowPtr,
    std::shared_ptr<Hex::Game> gamePtr
);