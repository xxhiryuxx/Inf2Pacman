#pragma once
struct MazeCell {
    bool visited;
    bool wall;
    MazeCell() : visited(false), wall(true) {}
};
