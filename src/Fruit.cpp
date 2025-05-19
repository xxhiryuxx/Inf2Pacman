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
