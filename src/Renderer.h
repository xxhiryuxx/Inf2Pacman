/**
 * @file Renderer.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once

class GameBoard;
class Score;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void drawGameBoard(const GameBoard& board);
    void drawScore(const Score& score);

private:    void clearScreen();
};
