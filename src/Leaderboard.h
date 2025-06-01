// Leaderboard class: manages high scores and player names
#pragma once
#include <string>
#include <vector>

class Leaderboard {
private:
    std::string filename; // File to store leaderboard
    std::vector<std::pair<int, std::string>> entries; // Vector of (score, name)

public:
    Leaderboard(const std::string& filename); // Constructor
    int getHighscore() const;                 // Get highest score
    std::string getHighscoreName() const;     // Get name of highscore holder
    bool tryUpdateHighscore(int score, const std::string& name); // Try to update highscore
    void load();                              // Load leaderboard from file
    void save() const;                        // Save leaderboard to file
};
