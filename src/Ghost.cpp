#include "Ghost.h"

Ghost::Ghost(int startX, int startY, Color color)
    : startX(startX), startY(startY), color(color) {
    x = startX;
    y = startY;
}

void Ghost::resetToStart() {
    x = startX;
    y = startY;
}