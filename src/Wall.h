#pragma once
#include "Element.h"

class Wall : public Element {
public:
    Wall();
    virtual ~Wall();
    
    // From Element
    virtual char draw() const override;
    virtual bool isWalkable() const override;
};
