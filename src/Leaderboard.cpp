#include "Leaderboard.h"

#include <fstream>
#include <algorithm>

// Constructor: loads leaderboard from file
Leaderboard::Leaderboard(const std::string& filename) : filename(filename) {
    load();
}

// Returns the highest score
int Leaderboard::getHighscore() const {
    if (!entries.empty()) {
        return entries.front().first;
    }
    return 0;
}

// Returns the name of the player with the highest score
std::string Leaderboard::getHighscoreName() const {
    if (!entries.empty()) {
        return entries.front().second;
    }
    return "---";
}

// Tries to update the highscore if the new score is higher
bool Leaderboard::tryUpdateHighscore(int score, const std::string& name) {
    if (entries.empty() || score > entries.front().first) {
        entries.insert(entries.begin(), {score, name});
        if (entries.size() > 10) entries.resize(10);
        save();
        return true;
    }
    return false;
}

// Loads leaderboard entries from file
void Leaderboard::load() {
    entries.clear();
    std::ifstream file(filename);
    std::string name;
    int score;
    while (file >> name >> score) {
        entries.emplace_back(score, name);
    }
}

// Saves leaderboard entries to file
void Leaderboard::save() const {
    std::ofstream file(filename);
    for (const auto& entry : entries) {
        file << entry.second << " " << entry.first << "\n";
    }
}
