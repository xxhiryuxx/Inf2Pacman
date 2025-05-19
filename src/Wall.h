/**
 * @file Ghost.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 17:56
 */

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