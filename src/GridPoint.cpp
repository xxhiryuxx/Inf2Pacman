#include "GridPoint.h"
#include <iostream>

GridPoint::GridPoint() : walkable(true), coin(nullptr), fruit(nullptr) {}

GridPoint::~GridPoint() {
    delete coin;
    delete fruit;
}

void GridPoint::setWalkable(bool isWalkable) {
    walkable = isWalkable;
}

void GridPoint::setCoin(Coin* newCoin) {
    if (coin) delete coin;
    coin = newCoin;
}

void GridPoint::setFruit(Fruit* newFruit) {
    if (fruit) delete fruit;
    fruit = newFruit;
}

Coin* GridPoint::removeCoin() {
    Coin* temp = coin;
    coin = nullptr;
    return temp;
}

Fruit* GridPoint::removeFruit() {
    Fruit* temp = fruit;
    fruit = nullptr;
    return temp;
}

bool GridPoint::hasCoin() const {
    return coin != nullptr;
}

bool GridPoint::hasFruit() const {
    return fruit != nullptr;
}

void GridPoint::draw() {
    if (!walkable) {
        std::cout << "#";
    } else if (fruit) {
        fruit->draw();
    } else if (coin) {
        coin->draw();
    } else {
        std::cout << " ";
    }
}

bool GridPoint::isWalkable() {
    return walkable;
}
