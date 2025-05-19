#include "Game.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

Game::Game() : 
    board(nullptr),
    score(nullptr),
    leaderboard(nullptr),
    controller(nullptr),
    renderer(nullptr),
    gameRunning(false) {
    
    board = new GameBoard(20, 21);  // Standard Pacman dimensions
    score = new Score();
    leaderboard = new Leaderboard();
    controller = new GameController();
    renderer = new Renderer();
}

Game::~Game() {
    delete board;
    delete score;
    delete leaderboard;
    delete controller;
    delete renderer;
}

void Game::initializeGameBoard() {
    board->initialize();
    score->display();
}

void Game::startGame() {
    gameRunning = true;
    
    while (gameRunning) {
        // Get input
        char input = controller->getInput();
        if (input == 'q' || input == 'Q') {
            gameRunning = false;
            break;
        }

        // Update game state
        update();

        // Check for game end conditions
        if (checkGameEnd()) {
            gameOver();
            break;
        }

        // Small delay to control game speed
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::update() {
    // Clear screen
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    // Update board state
    board->updateRandomFruit();
    
    // Draw current state
    renderer->drawGameBoard(*board);
    renderer->drawScore(*score);
}

bool Game::checkGameEnd() {
    return board->getRemainingCoins() == 0;
}

void Game::gameOver() {
    std::cout << "\nGame Over!\n";
    std::cout << "Final Score: " << score->getScore() << std::endl;
    
    std::string playerName;
    std::cout << "Enter your name: ";
    std::cin >> playerName;
    
    leaderboard->addEntry(playerName, score->getScore());
    leaderboard->displayLeaderboard();
}

void Game::saveScore() {
    leaderboard->saveToFile();
}

void Game::loadLeaderboard() {
    leaderboard->loadFromFile();
}
