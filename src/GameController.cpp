#include "GameController.h"
#include "Renderer.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

// problems with missing functions in file pacman

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
#ifdef _WIN32
    if (_kbhit()) {
        currentInput = _getch();
        return currentInput;
    }
    return 0;
#else
    struct termios oldt, newt;
    int oldf;
    char ch = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    if (read(STDIN_FILENO, &ch, 1) == 1) {
        currentInput = ch;
        ch = currentInput;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return ch;
#endif
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

    // Use try_move to handle actual movement based on the direction
    int newX = pacman->getX();
    int newY = pacman->getY();
    
    switch (lastDirection) {
        case Direction::UP:
            newY--;
            break;
        case Direction::DOWN:
            newY++;
            break;
        case Direction::LEFT:
            newX--;
            break;
        case Direction::RIGHT:
            newX++;
            break;
        default:
            return;
    }
    
    pacman->tryMove(newX, newY);
}

Direction GameController::getLastDirection() const {
    return lastDirection;
}
