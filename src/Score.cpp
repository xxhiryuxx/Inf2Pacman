/**
 * @file Score.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

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
