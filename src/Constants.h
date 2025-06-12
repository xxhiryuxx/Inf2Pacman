#pragma once

// Constants for the Gameboard and Window Size
const int WIDTH = 25;
const int HEIGHT = 15;
const int TILE_SIZE = 50;
const int SCREEN_WIDTH = WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = HEIGHT * TILE_SIZE;

// Cell types for the game board
enum Cell { WALL, EMPTY, COIN , FRUIT};
