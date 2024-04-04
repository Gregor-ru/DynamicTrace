#include "Object.h"

Object::Object() : pos(Point()), size(Point()), speed(Point()), color(RGB(0, 0, 0)), back_color(RGB(0, 0, 0)), speed_factor(3), trail_len(0), trail_len_now(0), trail(nullptr) {}

Object::Object(Point _pos, Point _size, COLORREF _color, COLORREF _back_color, int _trail_len) :
    pos(_pos),
    size(_size),
    color(_color),
    back_color(_back_color),
    trail_len(_trail_len),

    trail_len_now(0),
    speed(Point(0, 0)),
    speed_factor(3),
    last_pos( _pos),
    trail(new Trail[_trail_len * speed_factor]) {}

/*
void Object::Init(Point _pos, Point _size, COLORREF _color, COLORREF _back_color, int _trail_len) {
    pos = _pos;
    size = _size;
    color = _color;
    back_color = _back_color;
    trail_len = _trail_len;

    speed = Point(0, 0);
    trail_len_now = 0;

    TrailInit(_trail_len);
}
*/

void Object::TrailInit() {

    for (int i = 0; i < trail_len * speed_factor; i++) {
        trail[i] = Trail(Point(0, 0), trail_len * speed_factor);
    }
}

void Object::Show(HDC hdc) {
    SelectObject(hdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdc, color);

    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, color);
    Rectangle(hdc, (int)pos.x, (int)pos.y, (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void Object::TrailShow(HDC hdc) {
    for (int i = 0; i < trail_len * speed_factor; i++) {
        double alpha = (trail_len * speed_factor - trail[i].lifeTime) / (trail_len * speed_factor * 1.0);

        COLORREF transparent_color = RGB(
            (1 - alpha) * GetRValue(back_color) + alpha * GetRValue(color),
            (1 - alpha) * GetGValue(back_color) + alpha * GetGValue(color),
            (1 - alpha) * GetBValue(back_color) + alpha * GetBValue(color)
        );

        SelectObject(hdc, GetStockObject(DC_PEN));
        SetDCPenColor(hdc, transparent_color);

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, transparent_color);
        Rectangle(hdc, (int)trail[i].pos.x, (int)trail[i].pos.y, (int)(trail[i].pos.x + size.x), (int)(trail[i].pos.y + size.y));
    }
}

void Object::AllShow(HDC hdc, RECT rc) {
    HDC sqDc = CreateCompatibleDC(hdc);
    HBITMAP sqBm = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

    SelectObject(sqDc, sqBm);

    SelectObject(sqDc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(sqDc, RGB(255, 255, 255));
    Rectangle(sqDc, 0, 0, 640, 840);

    TrailShow(sqDc);
    Show(sqDc);

    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, sqDc, 0, 0, SRCCOPY);
    DeleteDC(sqDc);
    DeleteObject(sqBm);
}

void Object::Control() {

    for (int i = 0; i < 10; i++) {
        if ((GetKeyState(VK_UP) < 0) || (GetKeyState(VK_DOWN) < 0) || (GetKeyState(VK_LEFT) < 0) || (GetKeyState(VK_RIGHT) < 0)){
            break;
        }
        else {
            if (GetAsyncKeyState('0' + i) & 0x8000) {
                speed_factor = i;
                TrailInit();
            }
        }
        
    }

    speed.x = 0;
    speed.y = 0;

    if (GetKeyState(VK_UP) < 0) speed.y = -speed_factor;
    if (GetKeyState(VK_DOWN) < 0) speed.y = speed_factor;
    if (GetKeyState(VK_LEFT) < 0) speed.x = -speed_factor;
    if (GetKeyState(VK_RIGHT) < 0) speed.x = speed_factor;
    if (speed.x != 0 && speed.y != 0) speed = Point(speed.x * 0.7, speed.y * 0.7);
}

void Object::Move() {
    pos.x += speed.x;
    pos.y += speed.y;
}

void Object::TrailUpdate(HDC hdc, RECT rc) {
    switch (IfMove()) {
    case 1: {
        if (trail_len_now < trail_len) {
            trail_len_now++;

            for (int i = trail_len - trail_len_now; i < trail_len; i++) {
                trail[i] = Trail(trail[i + 1].pos, trail[i].lifeTime - speed_factor);
            }

            trail[trail_len - 1] = Trail(pos, 0);
        }
        else {
            for (int i = trail_len - trail_len_now; i < trail_len; i++) {
                trail[i] = Trail(trail[i + 1].pos, trail[i].lifeTime);
            }

            trail[trail_len - 1] = Trail(pos, 0);
        }
        last_pos = pos;
        AllShow(hdc, rc);
    }
          break;
    case -1: {
        while (trail_len_now > 0) {
            for (int i = trail_len - trail_len_now; i < trail_len; i++) {
                trail[i] = Trail(trail[i].pos, trail[i].lifeTime + speed_factor);
            }

            trail_len_now--;
            AllShow(hdc, rc);
            Sleep(1);
        }

        trail[trail_len - 1] = Trail(last_pos, 0);
    }
           break;
    default:
        break;
    }
    Sleep(4);
}

int Object::IfMove() const {
    if (GetKeyState(VK_UP) < 0 || GetKeyState(VK_DOWN) < 0 || GetKeyState(VK_LEFT) < 0 || GetKeyState(VK_RIGHT) < 0) {
        return 1;
    }

    if (GetAsyncKeyState(VK_UP) > 0 || GetAsyncKeyState(VK_DOWN) > 0 || GetAsyncKeyState(VK_LEFT) > 0 || GetAsyncKeyState(VK_RIGHT) > 0) {
        return -1;
    }

    return 0;
}
