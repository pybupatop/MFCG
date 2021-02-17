#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>
#include <cmath>

class Camera2D
{
protected:
	double L, R, B, T;					// ������� ���������� ������ ������� ������� ����
	int W, H;							// ���������� ������� ������� ����
	int WorldToScreenX(double X)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		int _X = ((X - L) / (R - L)) * W;
		return _X;
	}
	int WorldToScreenY(double Y)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		int _Y = ((T - Y) / (T - B)) * H;
		return _Y;
	}
	double ScreenToWorldX(int X)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return 0.0;
	}
	double ScreenToWorldY(int Y)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return 0.0;
	}
private:
	double posX, posY;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{
	}
	void Clear(HDC dc)
	{
		Rectangle(dc, 0, 0, W, H);
		// ������� ������� ������� ����
	}
	void SetResolution(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		H = r.bottom;
		W = r.right;
		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H, � ����� ������������� �������� ���������� L, R, B, T ����� �������, ����� ���������� ���������� �������� �� ������������ ����
	}
	void MoveTo(HDC dc, double X, double Y)
	{
		::MoveToEx(dc,WorldToScreenX(X), WorldToScreenY(Y), nullptr);
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
	}
	void LineTo(HDC dc, double X, double Y)
	{
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc)
	{
		Camera2D::MoveTo(dc,L, 0);
		Camera2D::LineTo(dc,R, 0);
		Camera2D::MoveTo(dc,0, T);
		Camera2D::LineTo(dc, 0, B);
		// ��������� ������������ ����
	}
};

#endif CAMERA_2D_H
