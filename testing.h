#pragma once

#include <iostream>

#include <AnimationWindow.h>

#include "Board.h"
#include "Game.h"
#include "HexagonalButton.h"
#include "UI/UINode.h"
#include "UI/HexGrid.h"
#include "UI/HexTile.h"
#include "UI/Label.h"
#include "UI/TurnIndicator.h"

void drawBoundingBoxes(std::shared_ptr<Hex::UI::UINode> node, std::shared_ptr<TDT4102::AnimationWindow> windowPtr);
void test_vec2_operators();
void test_board();
void test_game();
void test_hexbutton();

std::shared_ptr<Hex::HexagonalButton> createButton(
    Hex::vec2<int> tile,
    const std::shared_ptr<TDT4102::AnimationWindow> &windowPtr,
    const std::shared_ptr<Hex::Board> &boardPtr,
    std::shared_ptr<Hex::Game> gamePtr
);

void test_hexGrid();
void test_printChildren();
void test_engulf();