/**
 * @file GameController.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once

class GameController {
public:
    GameController();
    ~GameController();

    char getInput();
    void interpretInput();
    void forwardToPacMan();

private:    char currentInput;
};
