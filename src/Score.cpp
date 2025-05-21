
#include "Score.h"
#include <iostream>

Score::Score() : currentScore(0) {}

Score::~Score() {}

void Score::increase(int points) {
    currentScore += points;
}

int Score::getScore() const {
    return currentScore;
}

void Score::display() {
    std::cout << "Score: " << currentScore << std::endl;
}

