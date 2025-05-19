/**
 * @file GameBoard.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once
#include "Element.h"
#include "GridPoint.h"
#include "PacMan.h"
#include "Ghost.h"
#include <vector>
#include <memory>

class GameBoard {
public:
    GameBoard(int width, int height);
    ~GameBoard();

    void initialize();
    void draw();
    void setElement(int x, int y, Element* element);
    Element* getElement(int x, int y);
    bool isWalkable(int x, int y);
    void updateRandomFruit();
    
    // Game state queries
    int getRemainingCoins() const;
    PacMan* getPacMan() const { return pacman; }
    std::vector<Ghost*>& getGhosts() { return ghosts; }
    
    // Dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width;
    int height;
    std::vector<std::vector<Element*>> grid;
    PacMan* pacman;
    std::vector<Ghost*> ghosts;
    int remainingCoins;

    void loadDefaultLevel();    void clearBoard();
};
