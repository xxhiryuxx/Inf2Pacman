#pragma once

class GameController {
public:
    GameController();
    ~GameController();

    char getInput();
    void interpretInput();
    void forwardToPacMan();

private:
    char currentInput;
};

#endif // GAMECONTROLLER_H
