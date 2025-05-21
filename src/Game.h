#pragma once
#include "GameBoard.h"
#include "Score.h"
#include "Leaderboard.h"
#include "GameController.h"
#include "Renderer.h"
#include <chrono>
#include <thread>

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    HIGH_SCORE
};

class Game {
public:
    Game();
    ~Game();

    void initializeGameBoard();
    void startGame();
    void gameLoop();
    void processInput();
    void update();
    void render();
    bool checkGameEnd();
    void handleCollisions();
    void updateGhosts();
    void saveScore();
    void loadLeaderboard();
    void setState(GameState newState);
    GameState getState() const;
    void pause();
    void resume();

private:
    static constexpr int FRAME_RATE = 60;
    static constexpr std::chrono::milliseconds FRAME_DURATION{1000 / FRAME_RATE};
    
    GameBoard* board;
    Score* score;
    Leaderboard* leaderboard;
    GameController* controller;
    Renderer* renderer;
    GameState currentState;
    bool gameRunning;
    std::chrono::steady_clock::time_point lastUpdateTime;
    
    void maintainFrameRate();
    void checkPowerPelletTimeout();
    void handlePacmanGhostCollision(Ghost* ghost);
};
