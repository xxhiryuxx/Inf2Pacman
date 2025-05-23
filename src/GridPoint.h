#pragma once
#include "Element.h"

/**
 * Defines the possible contents of a grid cell in the game board
 */
enum class CellContent {
    EMPTY,        // Empty cell that can be moved through
    WALL,         // Wall that blocks movement
    COIN,         // Collectible coin worth 10 points
    FRUIT,        // Special fruit worth 100 points
    POWER_PELLET  // Power-up that makes ghosts vulnerable
};

/**
 * Represents a single cell in the game grid
 * Manages the cell's content and properties
 */
class GridPoint : public Element {
public:
    GridPoint();
    virtual ~GridPoint();

    // Content management
    void setContent(CellContent content);  // Set what's in this cell
    CellContent getContent() const;        // Get current cell contents
    void setWalkable(bool walkable);       // Set if characters can move through
    bool isCollectible() const;            // Check if contains coin/fruit
    int getPointValue() const;             // Get point value if collectible

    // From Element
    virtual char draw() const override;    // Returns character representation
    virtual bool isWalkable() const override;

private:
    bool walkable;
    CellContent content;
};