#pragma once
#include "Element.h"

class Fruit : public Element {
public:
    Fruit();
    virtual ~Fruit();

    int getPoints() const;
    
    // From Element
    virtual void draw() override;
    virtual bool isWalkable() override;

private:
    int points;
};

#endif // FRUIT_H
