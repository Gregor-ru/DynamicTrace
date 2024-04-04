#pragma once
#include "point.h"

class Trail {
public:
    Point pos;
    int lifeTime;

    Trail() : pos(Point()), lifeTime(0) {}
    Trail(Point _pos, int _lifeTime) : pos(_pos), lifeTime(_lifeTime) {}
};