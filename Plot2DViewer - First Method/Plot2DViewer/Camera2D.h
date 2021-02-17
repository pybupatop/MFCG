#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>
#include <cmath>

class Camera2D
{
protected:
	double L, R, B, T;					// Мировые координаты границ рабочей области окна
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		int _X = ((X - L) / (R - L)) * W;
		return _X;
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		int _Y = ((T - Y) / (T - B)) * H;
		return _Y;
	}
	double ScreenToWorldX(int X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		return 0.0;
	}
	double ScreenToWorldY(int Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		return 0.0;
	}
private:
	double posX, posY;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{
	}
	void Clear(HDC dc)
	{
		Rectangle(dc, 0, 0, W, H);
		// Очистка рабочей области окна
	}
	void SetResolution(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		H = r.bottom;
		W = r.right;
		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров L, R, B, T таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
	}
	void MoveTo(HDC dc, double X, double Y)
	{
		::MoveToEx(dc,WorldToScreenX(X), WorldToScreenY(Y), nullptr);
		// Перемещение графического курсора (posX, posY)
		// Обратите внимание, что мы действуем в мировых координатах
	}
	void LineTo(HDC dc, double X, double Y)
	{
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc)
	{
		Camera2D::MoveTo(dc,L, 0);
		Camera2D::LineTo(dc,R, 0);
		Camera2D::MoveTo(dc,0, T);
		Camera2D::LineTo(dc, 0, B);
		// Отрисовка координатных осей
	}
};

#endif CAMERA_2D_H
