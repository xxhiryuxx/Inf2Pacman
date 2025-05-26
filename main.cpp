#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int WIDTH = 15;
const int HEIGHT = 10;
const int TILE_SIZE = 40;
const int SCREEN_WIDTH = WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = HEIGHT * TILE_SIZE;

enum Cell { WALL, EMPTY, COIN };

struct Entity {
    int x, y;
};

struct Game {
    std::vector<std::vector<Cell>> field;
    Entity pacman;
    std::vector<Entity> ghosts;
    int score;
    bool gameOver;
    int coinsLeft;
    bool fruitPresent;
    int fruitX, fruitY;

    Game() : field(HEIGHT, std::vector<Cell>(WIDTH, COIN)), score(0), gameOver(false), coinsLeft(0), fruitPresent(false) {
        // Symmetrisches Labyrinth mit zentralem Ghost-Spawn (7,4) und (7,5)
        std::vector<std::vector<int>> layout = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
            {1,0,1,0,1,0,0,0,0,0,1,0,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,0,1,0,0,0,0,0,1,0,1,0,1},
            {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (layout[y][x] == 1) {
                    field[y][x] = WALL;
                } else {
                    field[y][x] = COIN;
                    coinsLeft++;
                }
            }
        }

        pacman = {1, 1};
        ghosts = {{7,4}, {7,5}, {6,4}, {8,5}}; // Zentrale Spawn-Positionen für Geister
        field[pacman.y][pacman.x] = EMPTY;
        coinsLeft--;
    }

    void movePacman() {
        int dx = 0, dy = 0;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            dy = -1;
        } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            dy = 1;
        } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            dx = -1;
        } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            dx = 1;
        }
        int nx = pacman.x + dx, ny = pacman.y + dy;
        if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) return;
        if (field[ny][nx] == WALL) return;
        pacman.x = nx; pacman.y = ny;

        if (field[ny][nx] == COIN) {
            field[ny][nx] = EMPTY;
            score += 10;
            coinsLeft--;
        }
        if (fruitPresent && fruitX == nx && fruitY == ny) {
            score += 100;
            fruitPresent = false;
        }
    }

    void moveGhosts() {
        if (rand() % 10 < 7) {  // 70% chance to move
            for (auto &g : ghosts) {
                int dir = rand() % 4;
                int dx = 0, dy = 0;
                if (dir == 0) dx = 1;
                if (dir == 1) dx = -1;
                if (dir == 2) dy = 1;
                if (dir == 3) dy = -1;
                int nx = g.x + dx, ny = g.y + dy;
                if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;
                if (field[ny][nx] == WALL) continue;
                g.x = nx; g.y = ny;
            }
        }
    }

    void checkCollision() {
        for (auto &g : ghosts)
            if (g.x == pacman.x && g.y == pacman.y)
                gameOver = true;
    }

    void spawnFruit() {
        if (fruitPresent) return;
        if (rand() % 20 != 0) return;

        std::vector<std::pair<int, int>> emptyCells;

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (field[y][x] == EMPTY && !(x == pacman.x && y == pacman.y)) {
                    emptyCells.push_back({x, y});
                }
            }
        }

        if (!emptyCells.empty()) {
            auto [x, y] = emptyCells[rand() % emptyCells.size()];
            fruitX = x;
            fruitY = y;
            fruitPresent = true;
        }
    }

    void draw() {
        ClearBackground(BLACK);
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                int px = x * TILE_SIZE;
                int py = y * TILE_SIZE;

                // Always draw base tile
                DrawRectangle(px, py, TILE_SIZE, TILE_SIZE, BLACK);

                if (field[y][x] == WALL)
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE, DARKGRAY);
                else if (field[y][x] == COIN)
                    DrawCircle(px + TILE_SIZE / 2, py + TILE_SIZE / 2, 5, GOLD);

                if (fruitPresent && x == fruitX && y == fruitY)
                    DrawCircle(px + TILE_SIZE / 2, py + TILE_SIZE / 2, 8, RED);
            }
        }

        for (auto &g : ghosts) {
            DrawCircle(g.x * TILE_SIZE + TILE_SIZE / 2, g.y * TILE_SIZE + TILE_SIZE / 2, 14, PURPLE);
        }

        DrawCircle(pacman.x * TILE_SIZE + TILE_SIZE / 2, pacman.y * TILE_SIZE + TILE_SIZE / 2, 14, YELLOW);

        DrawText(TextFormat("Score: %d", score), 10, SCREEN_HEIGHT - 30, 20, WHITE);
    }

    void run() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-Man mit Raylib");
        SetTargetFPS(10);

        while (!WindowShouldClose() && !gameOver && coinsLeft > 0) {
            BeginDrawing();
            movePacman();
            moveGhosts();
            checkCollision();
            spawnFruit();
            draw();
            EndDrawing();
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (gameOver)
            DrawText("Game Over! Du wurdest gefangen.", 100, SCREEN_HEIGHT / 2, 30, RED);
        else
            DrawText("Glückwunsch! Alle Münzen gesammelt.", 80, SCREEN_HEIGHT / 2, 30, GREEN);
        EndDrawing();

        // 3 Sekunden warten ohne weitere Zeichenbefehle
        double startTime = GetTime();
        while (!WindowShouldClose() && GetTime() - startTime < 5.0) {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }
        CloseWindow();
        TraceLog(LOG_INFO, "Closing window.");
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.run();
    return 0;
}
