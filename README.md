# Documentation

## Description

## The Game: Hex
The goal in Hex ([wikipedia page](https://en.wikipedia.org/wiki/Hex_(board_game))) is to connect your edges with stones. Each player takes a turn placing stones of their color, the stones cannot be moved after placement.

## Detecting a win
In order to check if a player has won, we must search for a connecting path of stones going from one side to the opposite. This is done after every move, using an implementation of the [A* pathfinding algorithm](https://www.youtube.com/watch?v=-L-WgKMFuhE).

## UI system
The UI elements are stored in a hierarchical node tree structure. Each node has a single parent and can have multiple children, when a node is updated or drawn it also has the responsibility to draw and update all of its children. The position of a node is stored relative to its parent, to get the "global" position of a node the relative positions of the node's parents are recursively used. "Global" here means a point in the same reference frame as the root node's position, a root node has no parents.

This way of managing UI elements is inspired by the concept of Nodes used in the Godot Engine.

The UI nodes also store an axis-aligned rectangle representing its bounding box. This bounding box covers the entire visible or relevant surface of a node, making it easy to find the center of a node, or check if a point is near the node.

## Saving the game


## vec2 and rect

## main.cpp

## More info on the game of Hex

## Wanted to add

## Resources (no particular order)
- Slides from TDT4102
- W3Schools
- Geeks For Geeks
- ChatGPT and Claude
- cppreference
- Stack Overflow

# Refleksjonsnotat