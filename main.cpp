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
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1 || (y == 3 && x == 3)) {
                    field[y][x] = WALL;
                } else {
                    field[y][x] = COIN;
                    coinsLeft++;
                }
            }
        }
        pacman = {1, 1};
        ghosts = {{WIDTH - 2, HEIGHT - 2}, {1, HEIGHT - 2}, {WIDTH - 2, 1}, {WIDTH / 2, HEIGHT / 2}};
        field[pacman.y][pacman.x] = EMPTY;
        coinsLeft--;
    }

    void movePacman() {
        int dx = 0, dy = 0;
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) dy = -1;
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) dy = 1;
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) dx = -1;
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) dx = 1;
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

    void checkCollision() {
        for (auto &g : ghosts)
            if (g.x == pacman.x && g.y == pacman.y)
                gameOver = true;
    }

    void spawnFruit() {
        if (fruitPresent) return;
        if (rand() % 20 == 0) {
            int x, y;
            do {
                x = rand() % WIDTH;
                y = rand() % HEIGHT;
            } while (field[y][x] != EMPTY || (x == pacman.x && y == pacman.y));
            fruitX = x; fruitY = y;
            fruitPresent = true;
        }
    }

    void draw() {
        ClearBackground(BLACK);
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                int px = x * TILE_SIZE;
                int py = y * TILE_SIZE;

                if (field[y][x] == WALL)
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE, DARKGRAY);
                else if (field[y][x] == COIN)
                    DrawCircle(px + TILE_SIZE / 2, py + TILE_SIZE / 2, 5, GOLD);
                else
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE, BLACK);

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
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-Man with Raylib");
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
        BeginDrawing();
        ClearBackground(BLACK);
        if (gameOver)
            DrawText("Game Over! You were caught.", 100, SCREEN_HEIGHT / 2, 30, RED);
        else
            DrawText("Congratulations! All coins collected.", 80, SCREEN_HEIGHT / 2, 30, GREEN);
        EndDrawing();

        // einfach nur 3 Sekunden warten ohne weitere Zeichenbefehle
        double startTime = GetTime();
        while (!WindowShouldClose() && GetTime() - startTime < 3.0) {
            // nichts tun – warten
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
