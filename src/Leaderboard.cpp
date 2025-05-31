#include "Leaderboard.h"

#include <fstream>
#include <algorithm>

Leaderboard::Leaderboard(const std::string& filename) : filename(filename) {
    load();
}

int Leaderboard::getHighscore() const {
    if (!entries.empty()) {
        return entries.front().first;
    }
    return 0;
}

std::string Leaderboard::getHighscoreName() const {
    if (!entries.empty()) {
        return entries.front().second;
    }
    return "---";
}

bool Leaderboard::tryUpdateHighscore(int score, const std::string& name) {
    if (entries.empty() || score > entries.front().first) {
        entries.insert(entries.begin(), {score, name});
        if (entries.size() > 10) entries.resize(10);
        save();
        return true;
    }
    return false;
}

void Leaderboard::load() {
    entries.clear();
    std::ifstream file(filename);
    int score;
    std::string name;
    while (file >> score && std::getline(file, name)) {
        if (!name.empty() && name[0] == ' ') name = name.substr(1);
        entries.emplace_back(score, name);
    }
}

void Leaderboard::save() const {
    std::ofstream file(filename);
    for (const auto& entry : entries) {
        file << entry.first << " " << entry.second << "\n";
    }
}
