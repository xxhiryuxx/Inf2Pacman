#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int WIDTH = 15;
const int HEIGHT = 10;
const int TILE_SIZE = 50;
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
        std::vector<std::vector<int>> layout = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,1,0,0,0,0,0,1,0,0,1},
            {1,0,1,1,0,1,0,1,1,1,0,1,0,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,0,1,1,1,1,1,0,1,1,0,1},
            {1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
            {1,1,1,1,0,1,0,1,0,1,0,1,1,1,1},
            {1,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
            {1,0,1,1,1,1,0,1,1,1,1,1,0,0,1},
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

        pacman = {7, 5};  // Startposition in der Mitte
        ghosts = {{6,4}, {8,4}, {6,6}, {8,6}};  // Geister um die Mitte herum
        field[pacman.y][pacman.x] = EMPTY;
        coinsLeft--;
    }

    void movePacman() {
        static int pacmanDelay = 0;
        pacmanDelay++;
        if (pacmanDelay % 2 != 0) return; // Pacman bewegt sich nur jedes zweite Frame

        int dx = 0, dy = 0;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dy = -1;
        else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dy = 1;
        else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dx = -1;
        else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx = 1;
        
        int nx = pacman.x + dx, ny = pacman.y + dy;
        if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) return;
        if (field[ny][nx] == WALL) return;
        
        pacman.x = nx;
        pacman.y = ny;

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
        static int ghostDelay = 0;
        ghostDelay++;
        if (ghostDelay % 4 != 0) return; // Geister bewegen sich nur jedes vierte Frame

        for (auto &g : ghosts) {
            int dir = rand() % 4;
            int dx = (dir == 0) ? 1 : (dir == 1) ? -1 : 0;
            int dy = (dir == 2) ? 1 : (dir == 3) ? -1 : 0;
            
            int nx = g.x + dx, ny = g.y + dy;
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                if (field[ny][nx] != WALL) {
                    g.x = nx;
                    g.y = ny;
                }
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

                DrawRectangle(px, py, TILE_SIZE, TILE_SIZE, BLACK);

                if (field[y][x] == WALL)
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE, BLUE); // Blaue Wände
                else if (field[y][x] == COIN)
                    DrawCircle(px + TILE_SIZE / 2, py + TILE_SIZE / 2, 7, GOLD);

                if (fruitPresent && x == fruitX && y == fruitY) {
                    // Kirsche als Frucht
                    int fx = px + TILE_SIZE / 2;
                    int fy = py + TILE_SIZE / 2;
                    DrawCircle(fx - 8, fy + 4, 8, RED);
                    DrawCircle(fx + 8, fy + 4, 8, RED);
                    DrawLine(fx - 8, fy + 4, fx - 2, fy - 10, DARKGREEN);
                    DrawLine(fx + 8, fy + 4, fx + 2, fy - 10, DARKGREEN);
                    DrawCircle(fx, fy - 10, 3, GREEN);
                }
            }
        }

        // Geister mit Augen und Fransen
        for (auto &g : ghosts) {
            int gx = g.x * TILE_SIZE + TILE_SIZE / 2;
            int gy = g.y * TILE_SIZE + TILE_SIZE / 2;
            int radius = 20;

            // Körper
            DrawCircle(gx, gy, radius, PURPLE);

            // Fransen unten
            for (int i = -radius; i < radius; i += 8) {
                DrawTriangle(
                    (Vector2){(float)(gx + i), (float)(gy + radius)},
                    (Vector2){(float)(gx + i + 8), (float)(gy + radius)},
                    (Vector2){(float)(gx + i + 4), (float)(gy + radius - 8)},
                    PURPLE
                );
            }

            // Augen
            DrawCircle(gx - 7, gy - 6, 6, WHITE);
            DrawCircle(gx + 7, gy - 6, 6, WHITE);
            DrawCircle(gx - 7, gy - 6, 2, BLUE);
            DrawCircle(gx + 7, gy - 6, 2, BLUE);
        }

        // Pac-Man als Sektor mit Auge
        int px = pacman.x * TILE_SIZE + TILE_SIZE / 2;
        int py = pacman.y * TILE_SIZE + TILE_SIZE / 2;
        int pradius = 20;
        DrawCircleSector((Vector2){(float)px, (float)py}, pradius, 30, 330, 32, YELLOW);
        DrawCircle(px + pradius / 4, py - pradius / 2, 3, BLACK);

        DrawText(TextFormat("Score: %d", score), 10, SCREEN_HEIGHT - 30, 24, WHITE);
    }

    void run() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-Man mit Raylib");
        SetTargetFPS(30);

        while (!WindowShouldClose() && !gameOver && coinsLeft > 0) {
            if (IsKeyPressed(KEY_ESCAPE)) break;

            movePacman();
            moveGhosts();
            checkCollision();
            spawnFruit();

            BeginDrawing();
            draw();
            EndDrawing();
        }

        if (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            if (gameOver)
                DrawText("Game Over!", SCREEN_WIDTH/2 - MeasureText("Game Over!", 40)/2, SCREEN_HEIGHT/2 - 20, 40, RED);
            else
                DrawText("Gewonnen!", SCREEN_WIDTH/2 - MeasureText("Gewonnen!", 40)/2, SCREEN_HEIGHT/2 - 20, 40, GREEN);
            EndDrawing();

            double startTime = GetTime();
            while (GetTime() - startTime < 3.0 && !WindowShouldClose()) {
                // Damit das Fenster nicht "einfriert"
                PollInputEvents();
            }
        }

        CloseWindow();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.run();
    return 0;
}
