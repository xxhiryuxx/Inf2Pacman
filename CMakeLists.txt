cmake_minimum_required(VERSION 3.10)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Renderer.cpp
    src/Player.cpp
    src/Ghost.cpp
    src/Entity.cpp
    src/GameBoard.cpp
    src/Leaderboard.cpp
)

add_executable(Pacman_Game ${SOURCES})

find_package(raylib REQUIRED)

target_link_libraries(Pacman_Game PRIVATE raylib)
