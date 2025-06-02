#include "Game.h"
#include "raylib.h"
#include "Renderer.h"
#include "MazeCell.h"
void Game::run() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-Man Raylib");
    Renderer::init();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        switch (state) {
            case STATE_START_MENU:
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
                if (getPlayerName()) {
                    state = STATE_PLAYING;
                }
                break;

            case STATE_PLAYING:
                if (IsKeyPressed(KEY_P)) {
                    state = STATE_PAUSED;
                    break;
                }
                if (!gameOver && board.coinsLeft > 0) {
                    movePacman();
                    moveGhosts();
                    checkCollision();
                    spawnFruit();
                    Renderer::drawGame(board, pacman, ghosts, leaderboard);
                } else {
                    state = STATE_GAME_OVER;
                }
                break;

            case STATE_PAUSED:
                Renderer::drawPauseMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
                if (IsKeyPressed(KEY_P)) {
                    state = STATE_PLAYING;
                    waitForKeyRelease(KEY_P);
                } else if (IsKeyPressed(KEY_Q)) {
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
                Renderer::drawLeaderboard(SCREEN_WIDTH, SCREEN_HEIGHT, "Leaderboard.txt");
                if (IsKeyPressed(KEY_L)) {
                    state = STATE_START_MENU;
                    waitForKeyRelease(KEY_L);
                }
                break;

            case STATE_GAME_OVER: {
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
    Renderer::unload();
    CloseWindow();
}

Game::Game()
    : board(),
    pacman(1, 1),
    ghosts({Ghost(WIDTH - 2, HEIGHT - 2), Ghost(1, HEIGHT - 2), Ghost(WIDTH - 2, 1), Ghost(WIDTH / 2, HEIGHT / 2)}),
    leaderboard("Leaderboard.txt"),
    gameOver(false),
    state(STATE_START_MENU)
{
    board.generateRandomMap();
    if (board.field[pacman.y][pacman.x] == COIN) {
        board.field[pacman.y][pacman.x] = EMPTY;
        board.coinsLeft--;
    }
}



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

void Game::movePacman() {
    static double lastMoveTime = 0.0;
    if (GetTime() - lastMoveTime < 0.12) return;
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
    if (board.fruitPresent && board.fruitX == nx && board.fruitY == ny) {
        pacman.score += 100;
        board.fruitPresent = false;
    }
}


void Game::moveGhosts() {
    static double lastGhostMove = 0.0;
    if (GetTime() - lastGhostMove < 0.32) return;
    lastGhostMove = GetTime();
    for (auto &g : ghosts) {
        int tries = 0;
        bool moved = false;
        while (tries < 4 && !moved) {
            int dir = rand() % 4;
            int dx = 0, dy = 0;
            if (dir == 0) dx = 1;
            else if (dir == 1) dx = -1;
            else if (dir == 2) dy = 1;
            else if (dir == 3) dy = -1;

            int nx = g.x + dx;
            int ny = g.y + dy;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && board.field[ny][nx] != WALL) {
                g.x = nx;
                g.y = ny;
                moved = true;
            }
            tries++;
        }
    }
}

void Game::checkCollision() {
    for (auto &g : ghosts) {
        if (g.x == pacman.x && g.y == pacman.y) {
            gameOver = true;
            return;
        }
    }
}

void Game::spawnFruit() {
    if (board.fruitPresent || (rand() % 20) != 0) return;

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
        board.fruitX = emptyCells[idx].first;
        board.fruitY = emptyCells[idx].second;
        board.fruitPresent = true;
    }
}

void Game::waitForKeyRelease(int key) {
    while (IsKeyDown(key) && !WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();
    }
}
