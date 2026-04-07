# Documentation

## Description

## The Game: Hex
The goal in Hex ([wikipedia page](https://en.wikipedia.org/wiki/Hex_(board_game))) is to connect your edges with stones. Each player takes a turn placing stones of their color, the stones cannot be moved after placement.

Since the player who gets the first move has an advantage a new rule is often introduced. The Swap rule lets the second player choose to take the first players move, this incentivises the first player to choose a fair opening move. This rule has not been implemented here, so if you choose to play with this rule then you have to do it manually.

## Detecting a win
In order to check if a player has won, we must search for a connecting path of stones going from one side to the opposite. This is done after every move, using an implementation of the [A* pathfinding algorithm](https://www.youtube.com/watch?v=-L-WgKMFuhE).

## UI system
The UI elements are stored in a **hierarchical node tree structure**. Each node has a **single parent** and can have **multiple children**, when a node is updated or drawn it also has the responsibility to draw and update all of its children. The position of a node is stored relative to its parent, to get the "global" position of a node the relative positions of the node's parents are recursively used. "Global" here means a point in the same reference frame as the root node's position, a root node has no parents.

This way of managing UI elements was inspired by the concept of Nodes used in the Godot Engine.

The UI nodes also store an axis-aligned rectangle representing its **bounding box**. This rectangle covers the entire visible or relevant surface of a node, making it easy to find the **center of a node**, or check if a **point is near the node**.

## Saving the game
The game is saved by storing the current state of the game in a text file.

The first line contains the **size** of the board, the current **turn** and the **status** (whether someone has won) of the game.

The following lines store the state of each tile in the board. It would be more efficient to store the moves in the order they were played and it wouldnt really be more complicated, but I didnt do it that way.

## vec2 and rect

## main.cpp

## More info on the game of Hex

If you want to play Hex against players online, then [playhex.org](https://playhex.org/) is the place to go. It also has a nice [tutorial](https://playhex.org/tutorial).

## Wanted to add

## Resources used
- Slides from TDT4102
- W3Schools
- Geeks For Geeks
- ChatGPT and Claude
- cppreference
- Stack Overflow

# Refleksjonsnotat
I am glad that I didnt delay starting on this project, starting early let me pivot away from an idea that would maybe have been too ambitious. The first thing I implemented was the Board and Game class, they were written completely independent of any of the graphics logic. Then I got a working prototype using hexagonal buttons. The provided graphics library didnt include hexagonal buttons, unbelievable. I got distracted for a day trying to implement sounds when placing a stone, but I couldnt get it to work. Trying to align UI elements was getting frustrating, so I implemented the hierarcical UI Node system with bounding boxes. Making the Hexagonal buttons inherit from UINode made aligning the board to the center of the screen easy. One feature I waited a long time before implementing was detecting when a player had won. I was afraid implementing A* was going to be tricky or maybe janky, but the result was flawless. After I finaly implemented saving and loading, the software actually started feeling nice to use. Exeption handling was more of an afterthought to fit the requirements.

From the beginning I was afraid that this project wouldnt be flashy enough, but I focused on making the sofware feel intuitive to use and create a good user interface. And I am happy with how the sofware feels to use in the end.
