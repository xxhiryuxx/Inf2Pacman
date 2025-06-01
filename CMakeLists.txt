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

# Plattformabhängige Konfiguration
if(APPLE)
    message(STATUS "Configuring for macOS")
    # Optional: Falls Homebrew raylib nicht automatisch gefunden wird
    set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/raylib")
elseif(WIN32)
    message(STATUS "Configuring for Windows")
    # Optional: Benutzerdefinierter raylib-Pfad für Windows (falls nötig)
    # set(CMAKE_PREFIX_PATH "C:/libs/raylib")
endif()

# Raylib finden
find_package(raylib REQUIRED)

# Raylib einbinden
target_link_libraries(Pacman_Game PRIVATE raylib)

# Optional: Für bessere IDE-Erkennung (z. B. CLion, VSCode)
target_include_directories(Pacman_Game PRIVATE ${raylib_INCLUDE_DIRS})