#include "Game.h"
#include "Ghost.h"
#include "PacMan.h"
#include "Score.h"
#include "Leaderboard.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

Game::Game() : 
    board(new GameBoard(10,10)),
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
    auto& currentCell = board->getGridPoint(pos.x, pos.y);
    if (currentCell.isCollectible()) {
        if (currentCell.getContent() == CellContent::POWER_PELLET) {
            pacman->collectPowerPellet();
            for (auto ghost : ghosts) {
                ghost->setScared(true);
            }
        }
        score->increase(currentCell.getPointValue());
        currentCell.setContent(CellContent::EMPTY);
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
        score->increase(ghost->getPointValue());
        ghost->returnToSpawn();
    } else {
        setState(GameState::GAME_OVER);
    }
}

void Game::render() {
    renderer->clear();
    
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
    // Power pellet timeout logic removed (not implemented in PacMan)
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
    return board->getRemainingCoins() == 0;
}

void Game::saveScore() {
    // Add score saving logic if needed
    leaderboard->saveToFile();
}

class GameBoard {
public:
    void update();

    Cell* getGridPoint(int x, int y);
};
    leaderboard->saveToFile();
