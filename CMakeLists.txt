cmake_minimum_required(VERSION 3.15)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    src/Element.cpp
    src/GameBoard.cpp
    src/GameCharacter.cpp
    src/GameController.cpp
    src/Ghost.cpp
    src/GridPoint.cpp
    src/Leaderboard.cpp
    src/PacMan.cpp
    src/Renderer.cpp
    src/Score.cpp
    src/main.cpp
)
