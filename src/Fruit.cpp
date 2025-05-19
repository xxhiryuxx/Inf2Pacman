/**
 * @file Fruit.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "Fruit.h"
#include <iostream>

Fruit::Fruit() : points(100) {}

Fruit::~Fruit() {}

int Fruit::getPoints() const {
    return points;
}

void Fruit::draw() {
    std::cout << "F";
}

bool Fruit::isWalkable() {
    return true;
}
