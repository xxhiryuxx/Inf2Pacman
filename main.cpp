#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <random>

const int WIDTH = 25;
const int HEIGHT = 15;
const int TILE_SIZE = 50;
const int SCREEN_WIDTH = WIDTH * TILE_SIZE;
const int SCREEN_HEIGHT = HEIGHT * TILE_SIZE;

enum Cell { WALL, EMPTY, COIN, BUNKER_WALL, BUNKER_OPENING, BUNKER_FLOOR };

struct Entity {
    int x, y;
    Entity(int startX = 0, int startY = 0) : x(startX), y(startY) {}
};

struct Player : public Entity {
    int score;
    Player(int startX = 0, int startY = 0) : Entity(startX, startY), score(0) {}
};

struct GhostEntity {
    int x, y;
    bool leftBunker = false;
};

class GameBoard {
public:
    std::vector<std::vector<Cell>> field;
    int coinsLeft;
    bool fruitPresent;
    int fruitX, fruitY;

    GameBoard() : field(HEIGHT, std::vector<Cell>(WIDTH, WALL)), coinsLeft(0), fruitPresent(false) {}
};

    bool leftBunker = false; // Merkt, ob der Geist schon draußen ist
};

struct Entity {
    int x, y;
    Entity(int startX, int startY) : x(startX), y(startY) {}
};

// Player struct, inherits from Entity and adds score
struct Player : public Entity {
    int score;
    Player(int startX, int startY) : Entity(startX, startY), score(0) {}
};

// Ghost struct, inherits from Entity
struct Ghost : public Entity {
    Ghost(int startX, int startY) : Entity(startX, startY) {}
};

// Handles the game board, coins, and fruit state
class GameBoard {
public:
    std::vector<std::vector<Cell>> field;
    int coinsLeft;
    bool fruitPresent;
    int fruitX, fruitY;

    // Initializes the board with all walls, no coins, no fruit
    GameBoard() : field(HEIGHT, std::vector<Cell>(WIDTH, WALL)), coinsLeft(0), fruitPresent(false) {}

    // Generates a random maze and places coins
    void generateRandomMap();
};

// Handles leaderboard file I/O and highscore logic
class Leaderboard {
private:
    std::string filename;
    std::vector<std::pair<int, std::string>> entries;

public:
    // Loads leaderboard from file and sorts entries
    Leaderboard(const std::string& file) : filename(file) {
        std::ifstream fileIn(filename);
        if (fileIn.is_open()) {
            std::string name;
            int score;
            while (fileIn >> name >> score) {
                entries.push_back({score, name});
            }
            fileIn.close();
            std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
            if (entries.size() > 10) entries.resize(10);
        }
    }

    // Returns the highest score
    int getHighscore() const {
        if (entries.empty()) return 0;
        return entries.front().first;
    }

    // Returns the name of the highscore holder
    std::string getHighscoreName() const {
        if (entries.empty()) return "None";
        return entries.front().second;
    }

    // Tries to update the highscore for a player, returns true if updated
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
            std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
            if (entries.size() > 10) entries.resize(10);
            save();
        }
        return updated;
    }

    // Saves the leaderboard to file
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

// Used for maze generation (DFS)
struct MazeCell {
    bool visited;
    bool wall;
    MazeCell() : visited(false), wall(true) {}
};

// All possible game states
enum GameState {
    STATE_START_MENU,
    STATE_ENTER_NAME,
    STATE_ENTER_NAME,
    STATE_LEADERBOARD,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
};

// Main game class: handles all game logic, state, and rendering
class Game {
public:
    GameBoard board;
    Player pacman;
    std::vector<GhostEntity> ghosts;
    Leaderboard leaderboard;
    std::string playerName;
    bool gameOver;
    GameState state;

    static constexpr int bunkerWidth = 5;
    static constexpr int bunkerHeight = 3;
    static constexpr int bunkerX = WIDTH / 2 - bunkerWidth / 2;
    static constexpr int bunkerY = HEIGHT / 2 - bunkerHeight / 2;
    static constexpr int bunkerOpenX = bunkerX + 2;
    static constexpr int bunkerOpenY = bunkerY;

    Game()
        : board(),
        pacman(1, 1),
        ghosts({Ghost(WIDTH - 2, HEIGHT - 2), Ghost(1, HEIGHT - 2), Ghost(WIDTH - 2, 1), Ghost(WIDTH / 2, HEIGHT / 2)}),
        leaderboard("Leaderboard.txt"),
        gameOver(false),
        state(STATE_START_MENU)
    {
        generateRandomMap();
        ghosts = {
            {bunkerX + 1, bunkerY + 1, false},
            {bunkerX + 2, bunkerY + 1, false},
            {bunkerX + 3, bunkerY + 1, false},
            {bunkerX + 2, bunkerY + 2, false}
        };
        if (board.field[pacman.y][pacman.x] == COIN) {
            board.field[pacman.y][pacman.x] = EMPTY;
            board.coinsLeft--;
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

    // Depth-first search for maze generation
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

    // Generates a random maze and places coins
    void generateRandomMap() {
        const int mazeHeight = HEIGHT % 2 == 0 ? HEIGHT + 1 : HEIGHT;
        const int mazeWidth = WIDTH % 2 == 0 ? WIDTH + 1 : WIDTH;

        std::vector<std::vector<MazeCell>> maze(mazeHeight, std::vector<MazeCell>(mazeWidth));
        dfs(1, 1, maze);

        board.coinsLeft = 0;
        board.field.resize(HEIGHT, std::vector<Cell>(WIDTH, WALL));
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (y < mazeHeight && x < mazeWidth && !maze[y][x].wall) {
                    board.field[y][x] = COIN;
                    board.coinsLeft++;
                } else {
                    board.field[y][x] = WALL;
                }
            }
        }

        int extraConnections = (WIDTH * HEIGHT) / 6;
        for (int i = 0; i < extraConnections; ++i) {
            int x = 1 + rand() % (WIDTH - 2);
            int y = 1 + rand() % (HEIGHT - 2);

            if (board.field[y][x] == WALL) {
                if (board.field[y][x - 1] != WALL && board.field[y][x + 1] != WALL) {
                    board.field[y][x] = COIN;
                    board.coinsLeft++;
                }
                else if (board.field[y - 1][x] != WALL && board.field[y + 1][x] != WALL) {
                    board.field[y][x] = COIN;
                    board.coinsLeft++;
                }
            }
        }

        // BUNKER exakt nach Vorgabe einbauen
        //  Y = bunkerY bis bunkerY+2, X = bunkerX bis bunkerX+4
        // Zeile 0: ##O##
        // Zeile 1: #FFFF#
        // Zeile 2: #####
        for (int y = 0; y < bunkerHeight; ++y) {
            for (int x = 0; x < bunkerWidth; ++x) {
                int fx = bunkerX + x;
                int fy = bunkerY + y;
                if (y == 0) { // obere Zeile
                    if (x == 2)
                        board.field[fy][fx] = BUNKER_OPENING; // Öffnung
                    else
                        board.field[fy][fx] = BUNKER_WALL;
                } else if (y == 1) { // mittlere Zeile
                    if (x == 0 || x == 4)
                        board.field[fy][fx] = BUNKER_WALL;
                    else
                        board.field[fy][fx] = BUNKER_FLOOR;
                } else if (y == 2) { // untere Zeile
                    board.field[fy][fx] = BUNKER_WALL;
                }
            }
        }

        // Entferne ggf. Coins im Bunkerbereich
        for (int y = 0; y < bunkerHeight; ++y) {
            for (int x = 0; x < bunkerWidth; ++x) {
                int fx = bunkerX + x;
                int fy = bunkerY + y;
                if (board.field[fy][fx] == BUNKER_FLOOR || board.field[fy][fx] == BUNKER_OPENING) {
                    if (board.field[fy][fx] == COIN) board.coinsLeft--;
                    board.field[fy][fx] = board.field[fy][fx]; // explizit setzen
                }
            }
        }

        // --- Diese Zeile garantiert den freien Ausgang! ---
        board.field[bunkerY - 1][bunkerX + 2] = EMPTY;
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

    // Handles Pacman movement and coin/fruit collection
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
        // PacMan darf BUNKER_WALL, BUNKER_FLOOR, BUNKER_OPENING nicht betreten!
        if (board.field[ny][nx] == WALL || board.field[ny][nx] == BUNKER_WALL ||
            board.field[ny][nx] == BUNKER_FLOOR || board.field[ny][nx] == BUNKER_OPENING) return;

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

    // Moves all ghosts randomly
    void moveGhosts() {
        static double lastGhostMove = 0.0;
        if (GetTime() - lastGhostMove < 0.32) return;
        lastGhostMove = GetTime();

        // Zufallsgenerator für std::shuffle
        static std::random_device rd;
        static std::mt19937 rng(rd());

        for (auto &g : ghosts) {
            // Im Bunker: gezielt zur Öffnung
            if (!g.leftBunker) {
                // Falls schon auf Öffnung, dann raus
                if (g.x == bunkerX + 2 && g.y == bunkerY + 1) {
                    g.y = bunkerY; // raus auf die Öffnung
                    g.leftBunker = true;
                    continue;
                }
                // Sonst: Bewege dich auf die Öffnung zu
                int dx = (bunkerX + 2) - g.x;
                int dy = (bunkerY + 1) - g.y;
                if (dx != 0) {
                    int step = (dx > 0) ? 1 : -1;
                    if (board.field[g.y][g.x + step] == BUNKER_FLOOR)
                        g.x += step;
                    else if (dy != 0 && board.field[g.y + ((dy > 0) ? 1 : -1)][g.x] == BUNKER_FLOOR)
                        g.y += (dy > 0) ? 1 : -1;
                } else if (dy != 0) {
                    int step = (dy > 0) ? 1 : -1;
                    if (board.field[g.y + step][g.x] == BUNKER_FLOOR || board.field[g.y + step][g.x] == BUNKER_OPENING)
                        g.y += step;
                }
            } else {
                // Außerhalb: 30% gezielt Richtung PacMan, sonst zufällig
                bool chase = (rand() % 100) < 30;
                int bestDx = 0, bestDy = 0;
                int minDist = 10000;
                std::vector<std::pair<int, int>> moves = { {1,0}, {-1,0}, {0,1}, {0,-1} };

                if (chase) {
                    for (auto [dx, dy] : moves) {
                        int nx = g.x + dx, ny = g.y + dy;
                        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                            Cell c = board.field[ny][nx];
                            if (c == EMPTY || c == COIN) {
                                int d = abs(nx - pacman.x) + abs(ny - pacman.y);
                                if (d < minDist) {
                                    minDist = d;
                                    bestDx = dx;
                                    bestDy = dy;
                                }
                            }
                        }
                    }
                }
                if (!chase || (bestDx == 0 && bestDy == 0)) {
                    // Zufällig bewegen
                    std::shuffle(moves.begin(), moves.end(), rng);
                    for (auto [dx, dy] : moves) {
                        int nx = g.x + dx, ny = g.y + dy;
                        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                            Cell c = board.field[ny][nx];
                            if (c == EMPTY || c == COIN) {
                                bestDx = dx;
                                bestDy = dy;
                                break;
                            }
                        }
                    }
                }
                g.x += bestDx;
                g.y += bestDy;
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

    // Randomly spawns a fruit on the board
    void spawnFruit() {
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

    // Draws the start menu
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

    // Draws the pause menu
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

    // Draws the leaderboard screen
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

    // Draws the game board, entities, and HUD
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

                if (board.field[y][x] == WALL) {
                    DrawRectangleRec(rect, BLUE);
                } else if (board.field[y][x] == BUNKER_WALL) {
                    DrawRectangleRec(rect, BLUE);
                } else if (board.field[y][x] == BUNKER_OPENING) {
                    DrawRectangleRec(rect, LIGHTGRAY); // Öffnung: helleres Blau
                } else if (board.field[y][x] == BUNKER_FLOOR) {
                    DrawRectangleRec(rect, DARKGRAY);
                } else {
                    DrawRectangleRec(rect, BLACK);
                    if (board.field[y][x] == COIN) {
                        DrawCircle(rect.x + TILE_SIZE/2, rect.y + TILE_SIZE/2, 6, YELLOW);
                    }
                }
            }
        }

        if (board.fruitPresent) {
            DrawCircle(board.fruitX * TILE_SIZE + TILE_SIZE/2,
                        board.fruitY * TILE_SIZE + TILE_SIZE/2,
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

        std::string scoreText = "Score: " + std::to_string(pacman.score);
        DrawText(scoreText.c_str(), 10, 10, 24, WHITE);

        std::string coinsText = "Coins left: " + std::to_string(board.coinsLeft);
        DrawText(coinsText.c_str(), 10, 40, 20, WHITE);

        std::string highscoreText = "Highscore: " + std::to_string(leaderboard.getHighscore()) + " (" + leaderboard.getHighscoreName() + ")";
        DrawText(highscoreText.c_str(), 10, 70, 20, WHITE);

        EndDrawing();
    }

    // Waits until a key is released (for menu navigation)
    void waitForKeyRelease(int key) {
        while (IsKeyDown(key) && !WindowShouldClose()) {
            BeginDrawing();
            EndDrawing();
        }
    }

    // Main game loop and state machine
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
                    if (!gameOver && board.coinsLeft > 0) {
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

                    std::string scoreText = "Score: " + std::to_string(pacman.score);
                    int scoreWidth = MeasureText(scoreText.c_str(), 32);
                    DrawText(scoreText.c_str(), SCREEN_WIDTH/2 - scoreWidth/2, SCREEN_HEIGHT/2, 32, WHITE);

                    if (leaderboard.tryUpdateHighscore(pacman.score, playerName)) {
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

// Entry point: seeds RNG, creates and runs the game
int main() {
    srand((unsigned int)time(nullptr));
    Game game;
    game.run();
    return 0;
}
