#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

const int WIDTH = 15;
const int HEIGHT = 10;

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
        }
    }

    int getHighscore() const {
        if (entries.empty()) return 0;
        return std::max_element(entries.begin(), entries.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; })->first;
    }

    std::string getHighscoreName() const {
        if (entries.empty()) return "None";
        return std::max_element(entries.begin(), entries.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; })->second;
    }

    bool tryUpdateHighscore(int score, const std::string& playerName) {
        int currentHighscore = getHighscore();
        if (score > currentHighscore) {
            entries.push_back({score, playerName});
            save();
            return true;
        }
        return false;
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

struct Game {
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

    Game() 
        : field(HEIGHT, std::vector<Cell>(WIDTH, COIN)),
          score(0),
          gameOver(false),
          coinsLeft(0),
          fruitPresent(false),
          leaderboard("Leaderboard.txt")
    {
        // Spielfeld initialisieren: Rand mit Wänden, Rest mit Münzen, eine feste Wandstelle (3,3)
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
        ghosts = {
            {WIDTH - 2, HEIGHT - 2}, 
            {1, HEIGHT - 2}, 
            {WIDTH - 2, 1}, 
            {WIDTH / 2, HEIGHT / 2}
        };

        // Startposition von Pacman frei machen und Münze entfernen
        field[pacman.y][pacman.x] = EMPTY;
        coinsLeft--;
    }

    void getPlayerName() {
        std::cout << "Please enter your name: ";
        std::getline(std::cin, playerName);
        if (playerName.empty()) playerName = "Player";
    }

    void movePacman() {
        std::cout << "Move (WASD + Enter): ";
        char input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Input-Puffer leeren
        input = std::tolower(input);

        int dx = 0, dy = 0;
        if (input == 'w') dy = -1;
        else if (input == 's') dy = 1;
        else if (input == 'a') dx = -1;
        else if (input == 'd') dx = 1;
        else return; // Ungültige Eingabe -> kein Zug

        int nx = pacman.x + dx;
        int ny = pacman.y + dy;

        // Prüfe Spielfeldgrenzen und Wände
        if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) return;
        if (field[ny][nx] == WALL) return;

        // Bewege Pacman
        pacman.x = nx;
        pacman.y = ny;

        // Münze einsammeln
        if (field[ny][nx] == COIN) {
            field[ny][nx] = EMPTY;
            score += 10;
            coinsLeft--;
        }

        // Frucht einsammeln
        if (fruitPresent && fruitX == nx && fruitY == ny) {
            score += 100;
            fruitPresent = false;
        }
    }

    void moveGhosts() {
        for (auto &g : ghosts) {
            int tries = 0;
            bool moved = false;
            // Versuche bis zu 4 mal, einen gültigen Zug zu finden
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
            // Falls kein Zug möglich, bleibt der Geist stehen
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
        if (fruitPresent) return;
        if (rand() % 20 != 0) return; // 1 zu 20 Chance pro Zug

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
        // Bildschirm "leeren" (einfach viele neue Zeilen)
        std::cout << std::string(50, '\n');

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (pacman.x == x && pacman.y == y) {
                    std::cout << "P";
                    continue;
                }

                bool drawn = false;
                for (auto &g : ghosts) {
                    if (g.x == x && g.y == y) {
                        std::cout << "G";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                if (fruitPresent && fruitX == x && fruitY == y) {
                    std::cout << "F";
                } else if (field[y][x] == WALL) {
                    std::cout << "#";
                } else if (field[y][x] == COIN) {
                    std::cout << ".";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "Score: " << score << "\n";
    }

    void run() {
        getPlayerName();

        while (!gameOver && coinsLeft > 0) {
            draw();
            movePacman();
            moveGhosts();
            checkCollision();
            spawnFruit();
        }
        draw();

        std::cout << "\n";
        if (gameOver)
            std::cout << "Game Over! You were caught.\n";
        else
            std::cout << "Congratulations! All coins collected.\n";

        if (leaderboard.tryUpdateHighscore(score, playerName)) {
            std::cout << "New Highscore by " << playerName << "! Your score: " << score << "\n";
        } else {
            std::cout << "Your score: " << score << "\n";
            std::cout << "Current Highscore: " << leaderboard.getHighscore()
                      << " by " << leaderboard.getHighscoreName() << "\n";
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.run();
    return 0;
}
