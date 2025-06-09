// Main game logic implementation
#include "Game.h"
#include "GameBoard.h"
#include "raylib.h"
#include "Renderer.h"
#include "MazeCell.h"
#include "Ghost.h"


// Main game loop and state management
void Game::run() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-Man Raylib");
    Renderer::init();
    SetTargetFPS(120);

    // Main game loop
    while (!WindowShouldClose()) {
        switch (state) {
            case STATE_START_MENU:
                // Draw the start menu and handle input
                Renderer::drawStartMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
                if (IsKeyPressed(KEY_ENTER)) {
                    state = STATE_ENTER_NAME;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    waitForKeyRelease(KEY_ESCAPE);
                    CloseWindow();
                    return;
                }
                if (IsKeyPressed(KEY_L)) {
                    state = STATE_LEADERBOARD;
                }
                break;

            case STATE_ENTER_NAME:
                // Prompt the player to enter their name
                if (getPlayerName()) {
                    state = STATE_PLAYING;
                }
                break;

            case STATE_PLAYING:
                // Main gameplay: move Pacman, ghosts, check collisions, draw game
                if (IsKeyPressed(KEY_P)) {
                    state = STATE_PAUSED;
                    break;
                }
                if (!gameOver && board.coinsLeft > 0) {
                    movePacman();
                    for (auto& ghost : ghosts) {
                        ghost.update(board, pacman);
                    }
                    checkCollision();
                    spawnFruit();
                    Renderer::drawGame(board, pacman, ghosts, leaderboard);
                } else {
                    state = STATE_GAME_OVER;
                }
                break;

            case STATE_PAUSED:
                // Draw pause menu and handle pause input
                Renderer::drawPauseMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
                if (IsKeyPressed(KEY_P)) {
                    state = STATE_PLAYING;
                    waitForKeyRelease(KEY_P);
                } else if (IsKeyPressed(KEY_Q)) {
                    // Warn user and return to main menu
                    BeginDrawing();
                    ClearBackground(DARKGRAY);
                    const char* warn = "Progress will be lost! Returning to main menu...";
                    int w = MeasureText(warn, 32);
                    DrawText(warn, SCREEN_WIDTH/2 - w/2, SCREEN_HEIGHT/2, 32, RED);
                    EndDrawing();
                    WaitTime(1.2);
                    *this = Game();
                    state = STATE_START_MENU;
                } else if (IsKeyPressed(KEY_ESCAPE)) {
                    *this = Game();
                    state = STATE_START_MENU;
                }
                break;

            case STATE_LEADERBOARD:
                // Show the leaderboard
                Renderer::drawLeaderboard(SCREEN_WIDTH, SCREEN_HEIGHT, "../Leaderboard.txt");
                if (IsKeyPressed(KEY_L)) {
                    state = STATE_START_MENU;
                    waitForKeyRelease(KEY_L);
                }
                break;

            case STATE_GAME_OVER: {
                // Handle game over state and highscore
                bool newHighscore = leaderboard.tryUpdateHighscore(pacman.score, playerName);
                Renderer::drawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, pacman.score, gameOver, leaderboard, playerName, newHighscore);
                if (IsKeyPressed(KEY_ENTER)) {
                    *this = Game();
                    state = STATE_START_MENU;
                }
                break;
            }
        }
    }
    // Cleanup resources
    Renderer::unload();
    CloseWindow();
}

// Constructor: initializes game objects and generates the map
Game::Game()
    : board(),
      pacman(1, 1),
      ghosts(),
      leaderboard("../Leaderboard.txt"),
      gameOver(false),
      state(STATE_START_MENU)
{
    board.generateRandomMap(); // Generate the initial map
    auto starts = board.getGhostStartPositions();
    ghosts.clear();
    if (starts.size() >= 4) {
        ghosts.emplace_back(starts[0].first, starts[0].second, RED);
        ghosts.emplace_back(starts[1].first, starts[1].second, BLUE);
        ghosts.emplace_back(starts[2].first, starts[2].second, GREEN);
        ghosts.emplace_back(starts[3].first, starts[3].second, PINK);
    }
    // Remove coin from Pacman's starting position if present
    if (board.field[pacman.y][pacman.x] == COIN) {
        board.field[pacman.y][pacman.x] = EMPTY;
        board.coinsLeft--;
    }
}

// Shuffle the direction arrays for random maze generation
void Game::shuffleDirections(int dx[], int dy[], int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(dx[i], dx[j]);
        std::swap(dy[i], dy[j]);
    }
}

// Depth-first search for maze generation
void Game::dfs(int x, int y, std::vector<std::vector<MazeCell>>& maze) {
    maze[y][x].visited = true;
    maze[y][x].wall = false;
    int dx[4] = {2, -2, 0, 0};
    int dy[4] = {0, 0, 2, -2};
    shuffleDirections(dx, dy, 4);
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx > 0 && nx < (int)maze[0].size() - 1 &&
            ny > 0 && ny < (int)maze.size() - 1 &&
            !maze[ny][nx].visited) {
            maze[y + dy[i] / 2][x + dx[i] / 2].wall = false;
            dfs(nx, ny, maze);
        }
    }
}

// Prompts the player to enter their name
bool Game::getPlayerName() {
    char name[32] = {0};
    int letterCount = 0;
    bool enterPressed = false;
    while (!enterPressed && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        const char* prompt = "Please enter your name and press ENTER:";
        int promptWidth = MeasureText(prompt, 28);
        DrawText(prompt, SCREEN_WIDTH/2 - promptWidth/2, 120, 28, RAYWHITE);
        DrawRectangle(SCREEN_WIDTH/2 - 180, 180, 360, 50, LIGHTGRAY);
        DrawText(name, SCREEN_WIDTH/2 - MeasureText(name, 32)/2, 195, 32, BLACK);
        EndDrawing();
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 31)) {
                name[letterCount] = (char)key;
                letterCount++;
                name[letterCount] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
            enterPressed = true;
        }
    }
    playerName = (letterCount > 0) ? std::string(name) : "Player";
    return enterPressed;
}

// Handles Pacman's movement and coin/fruit collection
void Game::movePacman() {
    static double lastMoveTime = 0.0;
    if (GetTime() - lastMoveTime < 0.12) return; // Control movement speed
    lastMoveTime = GetTime();
    int dx = 0, dy = 0;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dy = -1;
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dy = 1;
    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dx = -1;
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx = 1;
    int nx = pacman.x + dx;
    int ny = pacman.y + dy;
    if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) return;
    if (board.field[ny][nx] == WALL) return;
    pacman.x = nx;
    pacman.y = ny;
    if (board.field[ny][nx] == COIN) {
        board.field[ny][nx] = EMPTY;
        pacman.score += 10;
        board.coinsLeft--;
    }
    if (board.field[ny][nx] == FRUIT) {
        board.field[ny][nx] = EMPTY;
        pacman.score += 100;
        board.coinsLeft--;
    }
}

// Checks for collisions between Pacman and ghosts
void Game::checkCollision() {
    for (auto &g : ghosts) {
        if (g.x == pacman.x && g.y == pacman.y) {
            gameOver = true;
            return;
        }
    }
}

// Spawns a fruit on an empty cell only every 9 seconds
void Game::spawnFruit() {
    static double lastFruitTime = 0.0;
    if (GetTime() - lastFruitTime < 7.0) return;

    bool fruitExists = false;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (board.field[y][x] == FRUIT) {
                fruitExists = true;
                break;
            }
        }
        if (fruitExists) break;
    }
    if (fruitExists) return;

    std::vector<std::pair<int, int>> emptyCells;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (board.field[y][x] == EMPTY && !(x == pacman.x && y == pacman.y)) {
                emptyCells.push_back({x, y});
            }
        }
    }

    if (!emptyCells.empty()) {
        int idx = rand() % emptyCells.size();
        int x = emptyCells[idx].first;
        int y = emptyCells[idx].second;
        board.field[y][x] = FRUIT;
        board.coinsLeft++;
        lastFruitTime = GetTime();
    }
}

// Waits until the specified key is released
void Game::waitForKeyRelease(int key) {
    while (IsKeyDown(key) && !WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();
    }
}
