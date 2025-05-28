#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <cstring>

const int WIDTH = 25;
const int HEIGHT = 15;
const int TILE_SIZE = 50;
const int SCREEN_WIDTH = WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = HEIGHT * TILE_SIZE;

enum Cell { WALL, EMPTY, COIN };

struct Entity {
    int x, y;
};

class Leaderboard {
private:
    std::string filename;
    std::vector<std::pair<int, std::string>> entries; // pair<Score, Name>

public:
    Leaderboard(const std::string& file) : filename(file) {
        std::ifstream fileIn(filename);
        if (fileIn.is_open()) {
            std::string name;
            int score;
            while (fileIn >> name >> score) {
                entries.push_back({score, name});
            }
            fileIn.close();
            std::sort(entries.begin(), entries.end(),
                [](const auto& a, const auto& b) { return a.first > b.first; });
            if (entries.size() > 10) entries.resize(10);
        }
    }

    int getHighscore() const {
        if (entries.empty()) return 0;
        return entries.front().first;
    }

    std::string getHighscoreName() const {
        if (entries.empty()) return "None";
        return entries.front().second;
    }

    bool tryUpdateHighscore(int score, const std::string& playerName) {
        bool updated = false;
        auto it = std::find_if(entries.begin(), entries.end(),
            [&](const auto& entry) { return entry.second == playerName; });

        if (it != entries.end()) {
            if (score > it->first) {
                it->first = score;
                updated = true;
            }
        } else {
            entries.push_back({score, playerName});
            updated = true;
        }

        if (updated) {
            std::sort(entries.begin(), entries.end(),
                [](const auto& a, const auto& b) { return a.first > b.first; });
            if (entries.size() > 10) entries.resize(10);
            save();
        }
        return updated;
    }

    void save() {
        std::ofstream fileOut(filename);
        if (fileOut.is_open()) {
            for (const auto& entry : entries) {
                fileOut << entry.second << " " << entry.first << "\n";
            }
            fileOut.close();
        }
    }
};

struct MazeCell {
    bool visited;
    bool wall;
    MazeCell() : visited(false), wall(true) {}
};

enum GameState {
    STATE_START_MENU,
    STATE_ENTER_NAME,    // <--- NEU
    STATE_LEADERBOARD,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
};

class Game {
public:
    std::vector<std::vector<Cell>> field;
    Entity pacman;
    std::vector<Entity> ghosts;
    int score;
    bool gameOver;
    int coinsLeft;
    bool fruitPresent;
    int fruitX, fruitY;

    Leaderboard leaderboard;
    std::string playerName;

    GameState state;

    Game()
        : field(HEIGHT, std::vector<Cell>(WIDTH, WALL)),
        score(0),
        gameOver(false),
        coinsLeft(0),
        fruitPresent(false),
        leaderboard("Leaderboard.txt"),
        state(STATE_START_MENU)
    {
        generateRandomMap();
        pacman = {1, 1};
        ghosts = {
            {WIDTH - 2, HEIGHT - 2},
            {1, HEIGHT - 2},
            {WIDTH - 2, 1},
            {WIDTH / 2, HEIGHT / 2}
        };
        if (field[pacman.y][pacman.x] == COIN) {
            field[pacman.y][pacman.x] = EMPTY;
            coinsLeft--;
        }
    }

    void drawStartMenu() {
        BeginDrawing();
        ClearBackground(BLACK);
        const char* titleText = "PAC-MAN";
        int titleWidth = MeasureText(titleText, 72);
        DrawText(titleText, SCREEN_WIDTH/2 - titleWidth/2, 120, 72, YELLOW);

        const char* subText = "Press [ENTER] to start";
        int subWidth = MeasureText(subText, 32);
        DrawText(subText, SCREEN_WIDTH/2 - subWidth/2, 250, 32, WHITE);

        const char* escText = "Exit with [ESC]";
        int escWidth = MeasureText(escText, 24);
        DrawText(escText, SCREEN_WIDTH/2 - escWidth/2, 300, 24, GRAY);

        const char* lbText = "View Leaderboard with [L]";
        int lbWidth = MeasureText(lbText, 24);
        DrawText(lbText, SCREEN_WIDTH/2 - lbWidth/2, 330, 24, GRAY);

        EndDrawing();
    }

    void drawPauseMenu() {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        const char* pauseText = "PAUSE";
        int pauseWidth = MeasureText(pauseText, 64);
        DrawText(pauseText, SCREEN_WIDTH/2 - pauseWidth/2, 180, 64, YELLOW);

        const char* continueText = "Continue with [P]";
        int contWidth = MeasureText(continueText, 32);
        DrawText(continueText, SCREEN_WIDTH/2 - contWidth/2, 250, 32, WHITE);

        const char* exitText = "Exit with [ESC]";
        int exitWidth = MeasureText(exitText, 25);
        int exitY = 300;
        DrawText(exitText, SCREEN_WIDTH/2 - exitWidth/2, exitY, 25, GRAY);

        const char* warnText = "Back to Main Menu with [Q] (Progress will be lost!)";
        int warnWidth = MeasureText(warnText, 25);
        int warnY = exitY + 40;
        DrawText(warnText, SCREEN_WIDTH/2 - warnWidth/2, warnY, 25, RED);

        EndDrawing();
    }

    void drawLeaderboard() {
        BeginDrawing();
        ClearBackground(BLACK);
        const char* lbTitle = "Leaderboard";
        int titleWidth = MeasureText(lbTitle, 48);
        DrawText(lbTitle, SCREEN_WIDTH/2 - titleWidth/2, 60, 48, YELLOW);

        std::ifstream file("Leaderboard.txt");
        if (file.is_open()) {
            std::string name;
            int score;
            int line = 0;
            while (file >> name >> score && line < 10) {
                std::string entry = std::to_string(line + 1) + ". " + name + " - " + std::to_string(score);
                DrawText(entry.c_str(), SCREEN_WIDTH/2 - 150, 130 + line * 30, 24, WHITE);
                line++;
            }
            file.close();
        } else {
            DrawText("Unable to load leaderboard.", SCREEN_WIDTH/2 - 150, 130, 24, RED);
        }

        const char* backText = "Press [L] to return";
        int backWidth = MeasureText(backText, 20);
        DrawText(backText, SCREEN_WIDTH/2 - backWidth/2, SCREEN_HEIGHT - 50, 20, GRAY);

        EndDrawing();
    }

    void shuffleDirections(int dx[], int dy[], int n) {
        for (int i = n - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            std::swap(dx[i], dx[j]);
            std::swap(dy[i], dy[j]);
        }
    }

    void dfs(int x, int y, std::vector<std::vector<MazeCell>>& maze) {
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

    void generateRandomMap() {
        const int mazeHeight = HEIGHT % 2 == 0 ? HEIGHT + 1 : HEIGHT;
        const int mazeWidth = WIDTH % 2 == 0 ? WIDTH + 1 : WIDTH;

        std::vector<std::vector<MazeCell>> maze(mazeHeight, std::vector<MazeCell>(mazeWidth));

        dfs(1, 1, maze);

        coinsLeft = 0;
        field.resize(HEIGHT, std::vector<Cell>(WIDTH, WALL));
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (y < mazeHeight && x < mazeWidth && !maze[y][x].wall) {
                    field[y][x] = COIN;
                    coinsLeft++;
                } else {
                    field[y][x] = WALL;
                }
            }
        }

        int extraConnections = (WIDTH * HEIGHT) / 6;

        for (int i = 0; i < extraConnections; ++i) {
            int x = 1 + rand() % (WIDTH - 2);
            int y = 1 + rand() % (HEIGHT - 2);

            if (field[y][x] == WALL) {
                if (field[y][x - 1] != WALL && field[y][x + 1] != WALL) {
                    field[y][x] = COIN;
                    coinsLeft++;
                }
                else if (field[y - 1][x] != WALL && field[y + 1][x] != WALL) {
                    field[y][x] = COIN;
                    coinsLeft++;
                }
            }
        }
    }

    bool getPlayerName() {
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

    void movePacman() {
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

                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && field[ny][nx] != WALL) {
                    g.x = nx;
                    g.y = ny;
                    moved = true;
                }
                tries++;
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
            int idx = rand() % emptyCells.size();
            fruitX = emptyCells[idx].first;
            fruitY = emptyCells[idx].second;
            fruitPresent = true;
        }
    }

    void draw() {
        BeginDrawing();
        ClearBackground(BLACK);

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
                } else {
                    DrawRectangleRec(rect, BLACK);
                    if (field[y][x] == COIN) {
                        DrawCircle(rect.x + TILE_SIZE/2, rect.y + TILE_SIZE/2, 6, YELLOW);
                    }
                }
            }
        }

        if (fruitPresent) {
            DrawCircle(fruitX * TILE_SIZE + TILE_SIZE/2,
                        fruitY * TILE_SIZE + TILE_SIZE/2,
                        12, RED);
        }

        for (auto &g : ghosts) {
            DrawCircle(g.x * TILE_SIZE + TILE_SIZE/2,
                        g.y * TILE_SIZE + TILE_SIZE/2,
                        TILE_SIZE/2 - 4, PURPLE);
        }

        DrawCircle(pacman.x * TILE_SIZE + TILE_SIZE/2,
                    pacman.y * TILE_SIZE + TILE_SIZE/2,
                    TILE_SIZE/2 - 4, YELLOW);

        std::string scoreText = "Score: " + std::to_string(score);
        DrawText(scoreText.c_str(), 10, 10, 24, WHITE);

        std::string coinsText = "Coins left: " + std::to_string(coinsLeft);
        DrawText(coinsText.c_str(), 10, 40, 20, WHITE);

        std::string highscoreText = "Highscore: " + std::to_string(leaderboard.getHighscore()) + " (" + leaderboard.getHighscoreName() + ")";
        DrawText(highscoreText.c_str(), 10, 70, 20, WHITE);

        EndDrawing();
    }

    void waitForKeyRelease(int key) {
        while (IsKeyDown(key) && !WindowShouldClose()) {
            BeginDrawing();
            EndDrawing();
        }
    }

    void run() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-Man Raylib");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            switch (state) {
                case STATE_START_MENU:
                    drawStartMenu();
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
                    if (!gameOver && coinsLeft > 0) {
                        movePacman();
                        moveGhosts();
                        checkCollision();
                        spawnFruit();
                        draw();
                    } else {
                        state = STATE_GAME_OVER;
                    }
                    break;

                case STATE_PAUSED:
                    drawPauseMenu();
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
                    drawLeaderboard();
                    if (IsKeyPressed(KEY_L)) {
                        state = STATE_START_MENU;
                        waitForKeyRelease(KEY_L);
                    }
                    break;

                case STATE_GAME_OVER: {
                    BeginDrawing();
                    ClearBackground(BLACK);

                    const char* text = gameOver ? "Game Over!" : "You Won!";
                    int textWidth = MeasureText(text, 48);
                    DrawText(text, SCREEN_WIDTH/2 - textWidth/2, SCREEN_HEIGHT/2 - 60, 48, gameOver ? RED : GREEN);

                    std::string scoreText = "Score: " + std::to_string(score);
                    int scoreWidth = MeasureText(scoreText.c_str(), 32);
                    DrawText(scoreText.c_str(), SCREEN_WIDTH/2 - scoreWidth/2, SCREEN_HEIGHT/2, 32, WHITE);

                    if (leaderboard.tryUpdateHighscore(score, playerName)) {
                        std::string hsText = "New Highscore!";
                        int hsWidth = MeasureText(hsText.c_str(), 32);
                        DrawText(hsText.c_str(), SCREEN_WIDTH/2 - hsWidth/2, SCREEN_HEIGHT/2 + 50, 32, YELLOW);
                    } else {
                        std::string hsText = "Highscore: " + std::to_string(leaderboard.getHighscore()) + " (" + leaderboard.getHighscoreName() + ")";
                        int hsWidth = MeasureText(hsText.c_str(), 24);
                        DrawText(hsText.c_str(), SCREEN_WIDTH/2 - hsWidth/2, SCREEN_HEIGHT/2 + 50, 24, WHITE);
                    }

                    const char* menuText = "Press [ENTER] for Main Menu";
                    int menuWidth = MeasureText(menuText, 24);
                    DrawText(menuText, SCREEN_WIDTH/2 - menuWidth/2, SCREEN_HEIGHT/2 + 100, 24, GRAY);

                    EndDrawing();

                    if (IsKeyPressed(KEY_ENTER)) {
                        *this = Game();
                        state = STATE_START_MENU;
                    }
                    break;
                }
            }
        }
        CloseWindow();
    }
};

int main() {
    srand((unsigned int)time(nullptr));
    Game game;
    game.run();
    return 0;
}
