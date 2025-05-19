/**
 * @file Game.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once
#include "GameBoard.h"
#include "Score.h"
#include "Leaderboard.h"
#include "GameController.h"
#include "Renderer.h"

class Game {
public:
    Game();
    ~Game();

    void initializeGameBoard();
    void startGame();
    void update();
    bool checkGameEnd();
    void saveScore();
    void loadLeaderboard();

private:
    GameBoard* board;
    Score* score;
    Leaderboard* leaderboard;
    GameController* controller;
    Renderer* renderer;    bool gameRunning;
};
