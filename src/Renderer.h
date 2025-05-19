#pragma once

class GameBoard;
class Score;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void drawGameBoard(const GameBoard& board);
    void drawScore(const Score& score);

private:
    void clearScreen();
};

#endif // RENDERER_H
