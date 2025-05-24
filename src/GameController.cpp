#include "GameController.h"
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int _kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

GameController::GameController()
    : pacman(nullptr)
    , currentInput(0)
    , lastDirection(Direction::NONE)
{
    initializeInputMap();
}

GameController::~GameController() {
    // PacMan is owned by GameBoard, don't delete it here
}

void GameController::initializeInputMap() {
    inputMap['w'] = Direction::UP;
    inputMap['W'] = Direction::UP;
    inputMap['s'] = Direction::DOWN;
    inputMap['S'] = Direction::DOWN;
    inputMap['a'] = Direction::LEFT;
    inputMap['A'] = Direction::LEFT;
    inputMap['d'] = Direction::RIGHT;
    inputMap['D'] = Direction::RIGHT;
}

void GameController::setPacMan(PacMan* player) {
    pacman = player;
}

char GameController::getInput() {
    if (_kbhit()) {
        currentInput = _getch();
        return currentInput;
    }
    return 0;
}

void GameController::interpretInput() {
    Direction newDirection = mapInputToDirection(currentInput);
    if (newDirection != Direction::NONE) {
        lastDirection = newDirection;
    }
}

Direction GameController::mapInputToDirection(char input) {
    auto it = inputMap.find(input);
    if (it != inputMap.end()) {
        return it->second;
    }
    return Direction::NONE;
}

void GameController::forwardToPacMan() {
    if (!pacman || lastDirection == Direction::NONE) {
        return;
    }
    pacman->moveDirection(lastDirection);
}

Direction GameController::getLastDirection() const {
    return lastDirection;
}
