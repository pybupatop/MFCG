#include <windows.h>
#include <windowsx.h>
#include "Sight.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {		// �������� ���������
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� ��������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass(&wc);								// ����������� ������ wc

	HWND hWnd = CreateWindow(						// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"MainWindowClass",					// ��� �������� ������
		(LPCSTR)"Figure Viewer",					// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		200,200,400,400,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))	{			// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}
	return 0;
}

// � �������� ������ ����������� ������ ���� ���������� ���������� - �������� ������ ������ Sight
// ��� ���������� �������� �������������� ����������� ��������� � �������, ������������� � ���� ������
const int numberOfSights = 3;
Sight sights[numberOfSights];

int current = 0;

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)	{	// ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
	switch(msg)	{
	case WM_PAINT: {					// ��������� ��������� WM_PAINT ������������ ������ ���, ����� ��������� ��������� ��� ����������� �����������
			HDC dc = GetDC(hWnd);		// ������� GetDC ���������� �������� ����������, � ������� �������� ���������� � ���, � ����� ���� ������������ �����, ������ ������� ������� ������� ���� hWnd, � ����� ����� ������ ��������� ������ ��������� ������� ������� � �.�.
			sights[0].Clear(dc);
			for (int i = numberOfSights - 1 ; i >= 0 ; i--)	{
				sights[i].Draw(dc);
			}
			ReleaseDC(hWnd, dc);		// ������� ReleaseDC �������� �������, ��� ��������� � ����� hWnd �������� dc ������ �� �����
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	case WM_KEYDOWN: {
			sights[current].Move(2 * (wParam == VK_RIGHT)  - 2 * (wParam == VK_LEFT), 2 * (wParam == VK_DOWN) - 2 * (wParam == VK_UP));
			InvalidateRect(hWnd, nullptr, false);
			return 0;
		}
	case WM_RBUTTONDOWN: {
			sights[current].MoveTo(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			InvalidateRect(hWnd, nullptr, false);
			return 0;
		}
	case WM_LBUTTONDOWN: {
			int X, Y;
			X = GET_X_LPARAM(lParam);
			Y = GET_Y_LPARAM(lParam);
			for (int i = 0 ; i < numberOfSights ; i++) {
				if (sights[i].InnerPoint(X, Y))	{
					current = i;
					sights[current].StartDragging(X, Y);
					break;
				}
			}
			return 0;
		}
	case WM_MOUSEMOVE: {
			int X, Y;
			X = GET_X_LPARAM(lParam);
			Y = GET_Y_LPARAM(lParam);
			if (sights[current].IsDragging()) {
				sights[current].Drag(X, Y);
				InvalidateRect(hWnd, nullptr, false);
			}
			return 0;
		}
	case WM_LBUTTONUP: {
			sights[current].StopDragging();
			return 0;
		}
	case WM_MOUSEWHEEL:	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta > 0)
			sights[current].ChangeSize(2);
		else 
			sights[current].ChangeSize(-2);
	}
	case WM_SIZE: {
			InvalidateRect(hWnd, nullptr, false);	// ������� InvalidateRect ��������� ������� ������� ���� hWnd ��������� �����������, � ���������� ���� ������������ ��������� ��������� WM_PAINT
			return 0;								// ����� ������� �� ���������� ����, ��� ����������� ���������� � ��� ���������� �������� ����
		}
	case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
	default: {
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}
