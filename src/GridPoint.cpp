#include "GridPoint.h"
#include <iostream>

GridPoint::GridPoint() : content(CellContent::EMPTY) {}

void GridPoint::setContent(CellContent newContent) {
    content = newContent;
}

CellContent GridPoint::getContent() const {
    return content;
}

bool GridPoint::isCollectible() const {
    return  content == CellContent::COIN || 
            content == CellContent::FRUIT || 
            content == CellContent::POWER_PELLET;
}

int GridPoint::getPointValue() const {
    switch(content) {
        case CellContent::COIN:
            return 10;
        case CellContent::FRUIT:
            return 100;
        case CellContent::POWER_PELLET:
            return 50;
        default:
            return 0;
    }
}

char GridPoint::draw() const {
    switch(content) {
        case CellContent::WALL:
            return '#';  // Wall
        case CellContent::COIN:
            return '.';  // Coin
        case CellContent::FRUIT:
            return 'F';  // Fruit
        case CellContent::POWER_PELLET:
            return 'O';  // Power Pellet
        case CellContent::EMPTY:
        default:
            return ' ';  // Empty space
    }
}
