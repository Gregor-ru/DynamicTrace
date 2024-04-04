#include "framework.h"
#include "object.h"
#include "functions.h"


void WinShow(HDC hdc, RECT rc, Object* obj) {
    HDC sqDc = CreateCompatibleDC(hdc);
    HBITMAP sqBm = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

    SelectObject(sqDc, sqBm);

    SelectObject(sqDc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(sqDc, RGB(255, 255, 255));
    Rectangle(sqDc, 0, 0, 640, 840);

    //obj.TrailShow(sqDc);
    //obj->TrailUpdate(sqDc);

    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, sqDc, 0, 0, SRCCOPY);
    DeleteDC(sqDc);
    DeleteObject(sqBm);
}
