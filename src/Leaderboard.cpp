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
    bool updated = false;
    bool nameFound = false;
    int previousHighscore = getHighscore();
    bool newHighscore = false;

    // Prüfen, ob der Spieler bereits existiert
    for (auto& entry : entries) {
        if (entry.second == name) {
            nameFound = true;
            if (score > entry.first) {
                entry.first = score; // Score aktualisieren
                updated = true;
            }
            break;
        }
    }

    // Wenn der Spieler neu ist
    if (!nameFound) {
        entries.emplace_back(score, name);
        updated = true;
    }

    if (updated) {
        // Sortieren nach Score absteigend
        std::sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
            return a.first > b.first;
        });

        // Maximal 10 Einträge behalten
        if (entries.size() > 10) entries.resize(10);

        save();
    }

    if (updated && score >= previousHighscore) {
        newHighscore = true; // Neuer Highscore erreicht
    }

    return newHighscore;
}

// Loads leaderboard entries from file
void Leaderboard::load() {
    entries.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::ofstream createFile(filename); // Leere Datei erstellen
        return;
    }

    std::string name;
    int score;
    while (file >> name >> score) {
        entries.emplace_back(score, name);
    }

    // Sortieren nach Score absteigend
    std::sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
        return a.first > b.first;
    });

    // Maximal 10 Einträge behalten
    if (entries.size() > 10) entries.resize(10);
}

// Saves leaderboard entries to file
void Leaderboard::save() const {
    std::ofstream file(filename);
    for (const auto& entry : entries) {
        file << entry.second << " " << entry.first << "\n";
    }
}
