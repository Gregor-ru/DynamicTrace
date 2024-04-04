#pragma once
#include "framework.h"
#include "point.h"
#include "trail.h"

#include <Windows.h>

class Object {
public:
    Object();
    Object(Point _pos, Point _size, COLORREF _color, COLORREF _back_color, int _trail_len);
    void TrailInit();
    void Control();
    void Move();
    void TrailUpdate(HDC hdc, RECT rc);

private:
    Point pos;
    Point size;
    Point speed;
    Point last_pos;
    COLORREF color;
    COLORREF back_color;
    int speed_factor;
    int trail_len;
    int trail_len_now;
    Trail* trail;

    void Show(HDC hdc);
    void TrailShow(HDC hdc);
    void AllShow(HDC hdc, RECT rc);
    int IfMove() const;
};
