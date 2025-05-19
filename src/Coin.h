#pragma once
#include "Element.h"

class Coin : public Element {
public:
    Coin();
    virtual ~Coin();

    int getPoints() const;
    
    // From Element
    virtual void draw() override;
    virtual bool isWalkable() override;

private:
    int points;
};

#endif // COIN_H
