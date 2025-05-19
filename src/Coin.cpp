/**
 * @file Coin.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "Coin.h"
#include <iostream>

Coin::Coin() : points(10) {}

Coin::~Coin() {}

int Coin::getPoints() const {
    return points;
}

void Coin::draw() {
    std::cout << ".";
}

bool Coin::isWalkable() {
    return true;
}
