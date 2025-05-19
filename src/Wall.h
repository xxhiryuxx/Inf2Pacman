#pragma once
#include "Element.h"

class Wall : public Element {
public:
    Wall();
    virtual ~Wall();
    
    // From Element
    virtual void draw() override;
    virtual bool isWalkable() override;
};

#endif // WALL_H
