/**
 * @file Game.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "Game.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

Game::Game() : 
    board(new GameBoard()),
    score(new Score()),
    leaderboard(new Leaderboard()),
    controller(new GameController()),
    renderer(new Renderer()),
    currentState(GameState::MENU),
    gameRunning(false),
    lastUpdateTime(std::chrono::steady_clock::now())
{
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
                saveScore();
            }
        }
        
        render();
    }
}

void Game::processInput() {
    char input = controller->getInput();
    
    switch (currentState) {
        case GameState::PLAYING:
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
    board->update();
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
    auto currentCell = board->getGridPoint(pacman->getPosition());
    if (currentCell->isCollectible()) {
        if (currentCell->getContent() == CellContent::POWER_PELLET) {
            pacman->collectPowerPellet();
            for (auto ghost : ghosts) {
                ghost->setScared(true);
            }
        }
        score->addPoints(currentCell->getPointValue());
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
        score->addPoints(ghost->getPointValue());
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
        case GameState::HIGH_SCORE:
            renderer->renderHighScores(leaderboard);
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

void Game::saveScore() {
    leaderboard->addScore(score->getScore());
    leaderboard->saveToFile();
}
