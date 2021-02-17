#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>
#include <string>

class Camera2D
{
public:

protected:
	bool isDraging = false;
	int prevX, prevY;
	double X0, Y0;						// Экранные координаты начала мировой системы координат
	double px, py;                      // Единицы масштаба мировой системы координат, выраженные в пикселях
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X) {		// Переход от мировых координат к экранным (для абсциссы)
		return X * px + X0;
	}
	int WorldToScreenY(double Y) {		// Переход от мировых координат к экранным (для ординаты)
		return Y0 - Y * py;
	}
	double ScreenToWorldX(int X) {  	// Переход от экранных координат к мировым (для абсциссы)
		return (X - X0) / px;
	}
	double ScreenToWorldY(int Y) {	// Переход от экранных координат к мировым (для ординаты)
		return -(Y - Y0) / py;
	}
	double L() {						// Абсцисса левой границы рабочей области окна (в мировых координатах)
		return Camera2D::ScreenToWorldX(0);
	}
	double R() { 						// Абсцисса правой границы рабочей области окна (в мировых координатах)
		return Camera2D::ScreenToWorldX(W);
	}
	double B() {						// Абсцисса левой границы рабочей области окна (в мировых координатах)
		return Camera2D::ScreenToWorldY(H);
	}
	double T() {    					// Абсцисса правой границы рабочей области окна (в мировых координатах)
		return Camera2D::ScreenToWorldY(0);
	}
private:
	double posX, posY;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
public:
	Camera2D(double X0, double Y0, double px, double py) : X0(X0), Y0(Y0), px(px), py(py) {
	}
	void StartDraging(int X, int Y) {
		isDraging = true;
		prevX = X;
		prevY = Y;
	}
	void StopDraging() {
		isDraging = false;
	}
	void Drag(int X, int Y) {
		X0 += X - prevX;
		Y0 += Y - prevY;
		prevX = X;
		prevY = Y;
	}
	bool IsDraging() {
		return isDraging;
	}
	void Clear(HDC dc)	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		Rectangle(dc, 0, 0, r.right, r.bottom);
		// Очистка рабочей области окна
	}
	void SetResolution(HDC dc)	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		int dW = r.right - W, dH = r.bottom - H;
		if (W > 0 && H > 0) {
			double prevPx, prevPy;
			prevPx = px;
			prevPy = py;
			px *= (1 + (double)dW / W);
			py *= (1 + (double)dH / H);
			X0 *= px / prevPx;
			Y0 *= py / prevPy;
		}
		W += dW;
		H += dH;
		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров X0, Y0, px, py таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
	}

	void CompresionX(int delta) {
		px *= (1 + (double)delta / W);
	}

	void CompresionY(int delta) {
		py *= (1 + (double)delta / H);
	}

	void Zoom(int delta) {
		double prevPx = px;
		CompresionX(delta);
		py += px - prevPx;
	}

	void MoveTo(double X, double Y) {
		posX = X;
		posY = Y;
		// Перемещение графического курсора (posX, posY)
		// Обратите внимание, что мы действуем в мировых координатах
	}
	void LineTo(HDC dc, double X, double Y) {
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		posX = X;
		posY = Y;
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc) {
		MoveTo(L(), 0);
		LineTo(dc, R(), 0);
		MoveTo(0, T());
		LineTo(dc, 0, B());
;		// Отрисовка координатных осей
	}
	void Resolution(HDC dc)	{
		std::string st = "X resolution: " + std::to_string(px);
		TextOut(dc, 4, 4, (LPCSTR)(st.c_str()), st.length());
		st = "Y resolution: " + std::to_string(py);
		TextOut(dc, 4, 20, (LPCSTR)(st.c_str()), st.length());
	}
};

#endif CAMERA_2D_H
