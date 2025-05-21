#include "Game.h"
#include "Ghost.h"
#include "PacMan.h"
#include "Renderer.h"
#include "GameBoard.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// gameboard needs implementation to fix bugs in this file

Game::Game() : 
    board(new GameBoard()),
    controller(new GameController()),
    renderer(new Renderer()),
    currentState(GameState::MENU),
    gameRunning(false),
    lastUpdateTime(std::chrono::steady_clock::now())
{
}

Game::~Game() {
    delete board;
    delete controller;
    delete renderer;
}

// Removed: initializeGameBoard() is no longer needed as GameBoard initializes itself in its constructor.

void Game::startGame() {
    gameRunning = true;
    setState(GameState::PLAYING);
    gameLoop();
}

void Game::gameLoop() {
    while (gameRunning) {
        maintainFrameRate();
        
        processInput();
        
        if (currentState == GameState::PLAYING) {
            update();
            handleCollisions();
            checkPowerPelletTimeout();
            
            if (checkGameEnd()) {
                setState(GameState::GAME_OVER);
            }
        }
        
        render();
    }
}

void Game::processInput() {
            if (input == 'p' || input == 'P') {
                pause();
                return;
            }
            controller->interpretInput();
            controller->forwardToPacMan();
            break;
            
        case GameState::PAUSED:
            if (input == 'p' || input == 'P') {
                resume();
            }
            break;
            
        case GameState::GAME_OVER:
            if (input == '\r' || input == '\n') {
                setState(GameState::MENU);
            }
            break;
            
        case GameState::MENU:
            if (input == '\r' || input == '\n') {
                setState(GameState::PLAYING);
            }
            break;
            
        default:
            break;
    }
}

void Game::update() {
    // board->update(); // No longer needed if GameBoard does not require update
    updateGhosts();
}

void Game::updateGhosts() {
    auto ghosts = board->getGhosts();
    for (auto ghost : ghosts) {
        ghost->update();
    }
}

void Game::handleCollisions() {
    auto pacman = board->getPacMan();
    auto ghosts = board->getGhosts();
    
    // Check collisions with collectibles
    auto pos = pacman->getPosition();
    auto currentCell = board->getGridPoint(pos.x, pos.y);
    if (currentCell->isCollectible()) {
        if (currentCell->getContent() == CellContent::POWER_PELLET) {
            pacman->collectPowerPellet();
            for (auto ghost : ghosts) {
                ghost->setScared(true);
            }
        }
        currentCell->setContent(CellContent::EMPTY);
    }
    
    // Check ghost collisions
    for (auto ghost : ghosts) {
        if (pacman->getPosition() == ghost->getPosition()) {
            handlePacmanGhostCollision(ghost);
        }
    }
}

void Game::handlePacmanGhostCollision(Ghost* ghost) {
    if (ghost->isScared()) {
        ghost->returnToSpawn();
    } else {
        setState(GameState::GAME_OVER);
    }
}

void Game::render() {
    renderer->clear();
    board->render();
    
    switch (currentState) {
        case GameState::MENU:
            renderer->renderMenu();
            break;
        case GameState::PLAYING:
            renderer->renderScore(score->getScore());
            break;
        case GameState::PAUSED:
            renderer->renderPauseScreen();
            break;
        case GameState::GAME_OVER:
            renderer->renderGameOver(score->getScore());
            break;
    }
    
    renderer->display();
}

void Game::maintainFrameRate() {
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastUpdateTime
    );
    
    if (timeSinceLastUpdate < FRAME_DURATION) {
        std::this_thread::sleep_for(FRAME_DURATION - timeSinceLastUpdate);
    }
    
    lastUpdateTime = std::chrono::steady_clock::now();
}

void Game::checkPowerPelletTimeout() {
    auto pacman = board->getPacMan();
    if (pacman->isPowered()) {
        pacman->updatePowerTimer();
        if (pacman->getPowerTimer() <= 0) {
            for (auto ghost : board->getGhosts()) {
                ghost->setScared(false);
            }
        }
    }
}

void Game::setState(GameState newState) {
    currentState = newState;
}

GameState Game::getState() const {
    return currentState;
}

void Game::pause() {
    setState(GameState::PAUSED);
}

void Game::resume() {
    setState(GameState::PLAYING);
}

bool Game::checkGameEnd() {
    return board->getRemainingCollectibles() == 0;
}

