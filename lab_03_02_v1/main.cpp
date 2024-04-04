#include "framework.h"
#include "object.h"
#include "point.h"
#include "trail.h"

RECT rc;
Object player;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		// ��������� ����
	case WM_SIZE:
	{
		GetClientRect(hWnd, &rc);
	}
	case WM_PAINT:
	{
		// ��������� ��������� ���������� ����� ����
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rcClient;

		GetClientRect(hWnd, &rcClient);

		// ����� ���������, ����������� ���������
		EndPaint(hWnd, &ps);
	}
	break;

	// ����� �� ����������
	case WM_DESTROY:
		PostQuitMessage(0);

		// ��������� ��������� ���� �������������� ����������� �������
	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}

int main()
{
	Point firstPos(100, 100);
	Point secondPos;
	
	WNDCLASSA wndclass;

	memset(&wndclass, 0, sizeof(WNDCLASSA));
	wndclass.lpszClassName = "Game";
	wndclass.lpfnWndProc = WndProc;

	RegisterClassA(&wndclass);

	HWND hwnd;

	hwnd = CreateWindowA
	(
		"Game",
		"Game",
		WS_OVERLAPPEDWINDOW,
		10,
		10,
		640,
		480,
		NULL,
		NULL,
		NULL,
		NULL
	);

	HDC hdc = GetDC(hwnd);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg;
	
	player = Object(Point(100, 100), Point(40, 40), RGB(0, 255, 0), RGB(255, 255, 255), 100);
	
	player.TrailInit();

	while (1)
	{
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{

			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			player.Control();
			player.Move();
			player.TrailUpdate(hdc, rc);
		}
	}

	return 0;
}
