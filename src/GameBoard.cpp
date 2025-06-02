#include "GameBoard.h"
#include "raylib.h"
#include <cstdlib>

// Generates a static Pac-Man-like maze with a central ghost house (bunker)
void GameBoard::generateRandomMap() {
    field.resize(HEIGHT, std::vector<Cell>(WIDTH, WALL));
    coinsLeft = 0;

    // Statically defined Pac-Man-like maze
    std::vector<std::string> layout = {
        "####################",
        "#........##........#",
        "#.####.#.##.#.####.#",
        "#.#  #.#.##.#.#  #.#",
        "#.####.#.##.#.####.#",
        "#..................#",
        "#.####.######.####.#",
        "#.####.######.####.#",
        "#..................#",
        "####.## ###### ##.###",
        "#..................#",
        "#.####.######.####.#",
        "#.####.######.####.#",
        "#..................#",
        "###.##.######.##.###",
        "#....#....##....#..#",
        "#.##########.######.",
        "####################"
    };

    for (int y = 0; y < HEIGHT && y < static_cast<int>(layout.size()); ++y) {
        for (int x = 0; x < WIDTH && x < static_cast<int>(layout[y].size()); ++x) {
            char ch = layout[y][x];
            switch (ch) {
                case '#':
                    field[y][x] = WALL;
                    break;
                case '.':
                    field[y][x] = COIN;
                    ++coinsLeft;
                    break;
                default:
                    field[y][x] = EMPTY;
                    break;
            }
        }
    }

    // Insert central ghost bunker ("ghost house") as empty 3x3 area in the center
    int centerY = HEIGHT / 2;
    int centerX = WIDTH / 2;
    for (int y = centerY - 1; y <= centerY + 1; ++y) {
        for (int x = centerX - 1; x <= centerX + 1; ++x) {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                field[y][x] = EMPTY;
            }
        }
    }   
}

const std::vector<std::pair<int, int>>& GameBoard::getGhostStartPositions() const {
    static std::vector<std::pair<int, int>> ghostStarts = {
        {WIDTH / 2 - 1, HEIGHT / 2},     // Linker Geist
        {WIDTH / 2, HEIGHT / 2},         // Mittlerer Geist
        {WIDTH / 2 + 1, HEIGHT / 2},     // Rechter Geist
        {WIDTH / 2, HEIGHT / 2 - 1}      // Oberer Geist
    };
    return ghostStarts;
}