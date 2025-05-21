#pragma once
#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string name;
    int score;
};

class Leaderboard {
public:
    Leaderboard();
    ~Leaderboard();

    void loadFromFile();
    void saveToFile();
    void displayLeaderboard();
    void addEntry(const std::string& name, int score);

private:
    std::vector<LeaderboardEntry> entries;
    const std::string filename = "leaderboard.txt";
};
