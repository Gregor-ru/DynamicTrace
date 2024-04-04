#include "object.h"

class Square : public Object {
public:
    // Конструкторы
    Square() : Object() {}
    Square(Point pos, Point size, COLORREF color) : Object(pos, size, color) {}

    // Переопределение метода для рисования квадрата
    void Draw(HDC hdc) override {
        // Рисование квадрата
        SelectObject(hdc, GetStockObject(DC_PEN));
        SetDCPenColor(hdc, brush);

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, brush);
        Rectangle(hdc, (int)pos.x, (int)pos.y, (int)(pos.x + size.x), (int)(pos.y + size.y));
    }
};

class Circle : public Object {
public:
    // Конструкторы
    Circle() : Object() {}
    Circle(Point pos, Point size, COLORREF color) : Object(pos, size, color) {}

    // Переопределение метода для рисования круга
    void Draw(HDC hdc) override {
        // Рисование круга
    }
};
