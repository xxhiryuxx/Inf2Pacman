#pragma once

const int WIDTH = 25;
const int HEIGHT = 15;
const int TILE_SIZE = 50;
const int SCREEN_WIDTH = WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = HEIGHT * TILE_SIZE;

enum Cell { WALL, EMPTY, COIN };
