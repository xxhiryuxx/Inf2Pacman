
#include "Leaderboard.h"
#include <fstream>
#include <algorithm>
#include <iostream>

Leaderboard::Leaderboard() {}

Leaderboard::~Leaderboard() {}

void Leaderboard::loadFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No existing leaderboard found. Creating new one.\n";
        return;
    }

    std::string name;
    int score;
    
    entries.clear();
    while (file >> name >> score) {
        entries.push_back({name, score});
    }
    
    // Sort entries after loading
    std::sort(entries.begin(), entries.end(), 
        [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
            return a.score > b.score;
        });
}

void Leaderboard::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not save leaderboard.\n";
        return;
    }

    // Save only top 5 scores
    size_t count = std::min(entries.size(), size_t(5));
    for (size_t i = 0; i < count; ++i) {
        file << entries[i].name << " " << entries[i].score << std::endl;
    }
}

void Leaderboard::displayLeaderboard() {
    std::cout << "\n=== TOP 5 HIGH SCORES ===\n";
    std::cout << "------------------------\n";
    
    size_t count = std::min(entries.size(), size_t(5));
    for (size_t i = 0; i < count; ++i) {
        std::cout   << (i + 1) << ". " << entries[i].name 
                    << ": " << entries[i].score << std::endl;
    }
    std::cout << "------------------------\n";
}

void Leaderboard::addEntry(const std::string& name, int score) {
    // Check if this score is high enough to be in top 5
    if (!entries.empty() && entries.size() >= 5 && score <= entries[4].score) {
        return;  // Score not high enough
    }

    entries.push_back({name, score});
    std::sort(entries.begin(), entries.end(), 
        [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
            return a.score > b.score;
        });
    
    // Keep only top 5 scores
    if (entries.size() > 5) {
        entries.resize(5);
    }
    
    // Save immediately when we have a new high score
    saveToFile();
}