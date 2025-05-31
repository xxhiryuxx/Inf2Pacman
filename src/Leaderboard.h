#pragma once
#include <string>
#include <vector>

class Leaderboard {
private:
    std::string filename;
    std::vector<std::pair<int, std::string>> entries;

public:
    Leaderboard(const std::string& filename);
    int getHighscore() const;
    std::string getHighscoreName() const;
    bool tryUpdateHighscore(int score, const std::string& name);
    void load();
    void save() const;
};
