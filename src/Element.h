#pragma once

class Element {
public:
    Element();
    virtual ~Element();
    virtual void draw() = 0;
    virtual bool isWalkable() = 0;

protected:
    // Common properties for all elements
    int x;
    int y;
};

#endif // ELEMENT_H
