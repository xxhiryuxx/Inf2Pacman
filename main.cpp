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

        // Korrekte Initialisierung mit Wand-Check für Geister
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

        // Pacman-Startposition validieren
        pacman = {7, 5};
        field[pacman.y][pacman.x] = EMPTY;
        coinsLeft--;

        // Geister-Startpositionen validieren
        std::vector<Entity> ghostSpawns = {{6,4}, {8,4}, {6,6}, {8,6}};
        for (auto g : ghostSpawns) {
            if (field[g.y][g.x] != WALL) {
                ghosts.push_back(g);
            }
        }
    }

    void movePacman() {
        static int pacmanDelay = 0;
        pacmanDelay++;
        if (pacmanDelay % 2 != 0) return;

        int dx = 0, dy = 0;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dy = -1;
        else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dy = 1;
        else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dx = -1;
        else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx = 1;
        
        int nx = pacman.x + dx;
        int ny = pacman.y + dy;
        
        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && field[ny][nx] != WALL) {
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
    }

    void moveGhosts() {
        static int ghostDelay = 0;
        ghostDelay++;
        if (ghostDelay % 4 != 0) return;

        for (auto &g : ghosts) {
            int dir = rand() % 4;
            int dx = (dir == 0) ? 1 : (dir == 1) ? -1 : 0;
            int dy = (dir == 2) ? 1 : (dir == 3) ? -1 : 0;
            
            int nx = g.x + dx;
            int ny = g.y + dy;
            
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && field[ny][nx] != WALL) {
                g.x = nx;
                g.y = ny;
            }
        }
    }

    void checkCollision() {
        for (auto &g : ghosts) {
            if (g.x == pacman.x && g.y == pacman.y) {
                gameOver = true;
                return;
            }
        }
    }

    void spawnFruit() {
        if (fruitPresent || (rand() % 20) != 0) return;

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
        BeginDrawing();
        ClearBackground(BLACK);

        // Zeichne Grid
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                Rectangle rect = {
                    static_cast<float>(x * TILE_SIZE),
                    static_cast<float>(y * TILE_SIZE),
                    static_cast<float>(TILE_SIZE),
                    static_cast<float>(TILE_SIZE)
                };

                if (field[y][x] == WALL) {
                    DrawRectangleRec(rect, BLUE);
                } else if (field[y][x] == COIN) {
                    DrawCircle(rect.x + TILE_SIZE/2, rect.y + TILE_SIZE/2, 5, YELLOW);
                }
            }
        }

        // Zeichne Frucht
        if (fruitPresent) {
            DrawCircle(fruitX * TILE_SIZE + TILE_SIZE/2, 
                      fruitY * TILE_SIZE + TILE_SIZE/2, 
                      10, RED);
        }

        // Zeichne Geister
        for (auto &g : ghosts) {
            DrawCircle(g.x * TILE_SIZE + TILE_SIZE/2,
                      g.y * TILE_SIZE + TILE_SIZE/2,
                      20, PURPLE);
        }

        // Zeichne Pacman
        DrawCircle(pacman.x * TILE_SIZE + TILE_SIZE/2,
                  pacman.y * TILE_SIZE + TILE_SIZE/2,
                  20, YELLOW);

        // Score-Anzeige
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);

        EndDrawing();
    }

    void run() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-Man");
        SetTargetFPS(30);

        while (!WindowShouldClose() && !gameOver && coinsLeft > 0) {
            movePacman();
            moveGhosts();
            checkCollision();
            spawnFruit();
            draw();
        }

        // Spielende-Screen
        BeginDrawing();
        ClearBackground(BLACK);
        const char* text = gameOver ? "Game Over!" : "Gewonnen!";
        int textWidth = MeasureText(text, 40);
        DrawText(text, SCREEN_WIDTH/2 - textWidth/2, SCREEN_HEIGHT/2 - 20, 40, gameOver ? RED : GREEN);
        EndDrawing();

        // Warte 3 Sekunden
        double startTime = GetTime();
        while (GetTime() - startTime < 3.0 && !WindowShouldClose()) {
            PollInputEvents();
        }

        CloseWindow();
    }
};

int main() {
    srand(time(0));
    Game game;
    game.run();
    return 0;
}
