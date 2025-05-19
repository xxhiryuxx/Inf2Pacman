#include "Renderer.h"
#include "GameBoard.h"
#include "Score.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::drawGameBoard(const GameBoard& board) {
    clearScreen();
    // Will be implemented later
}

void Renderer::drawScore(const Score& score) {
    // Will be implemented later
}

void Renderer::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
