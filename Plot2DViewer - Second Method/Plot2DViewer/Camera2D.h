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
	double X0, Y0;						// �������� ���������� ������ ������� ������� ���������
	double px, py;                      // ������� �������� ������� ������� ���������, ���������� � ��������
	int W, H;							// ���������� ������� ������� ����
	int WorldToScreenX(double X) {		// ������� �� ������� ��������� � �������� (��� ��������)
		return X * px + X0;
	}
	int WorldToScreenY(double Y) {		// ������� �� ������� ��������� � �������� (��� ��������)
		return Y0 - Y * py;
	}
	double ScreenToWorldX(int X) {  	// ������� �� �������� ��������� � ������� (��� ��������)
		return (X - X0) / px;
	}
	double ScreenToWorldY(int Y) {	// ������� �� �������� ��������� � ������� (��� ��������)
		return -(Y - Y0) / py;
	}
	double L() {						// �������� ����� ������� ������� ������� ���� (� ������� �����������)
		return Camera2D::ScreenToWorldX(0);
	}
	double R() { 						// �������� ������ ������� ������� ������� ���� (� ������� �����������)
		return Camera2D::ScreenToWorldX(W);
	}
	double B() {						// �������� ����� ������� ������� ������� ���� (� ������� �����������)
		return Camera2D::ScreenToWorldY(H);
	}
	double T() {    					// �������� ������ ������� ������� ������� ���� (� ������� �����������)
		return Camera2D::ScreenToWorldY(0);
	}
private:
	double posX, posY;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
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
		// ������� ������� ������� ����
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
		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H, � ����� ������������� �������� ���������� X0, Y0, px, py ����� �������, ����� ���������� ���������� �������� �� ������������ ����
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
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
	}
	void LineTo(HDC dc, double X, double Y) {
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		posX = X;
		posY = Y;
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc) {
		MoveTo(L(), 0);
		LineTo(dc, R(), 0);
		MoveTo(0, T());
		LineTo(dc, 0, B());
;		// ��������� ������������ ����
	}
	void Resolution(HDC dc)	{
		std::string st = "X resolution: " + std::to_string(px);
		TextOut(dc, 4, 4, (LPCSTR)(st.c_str()), st.length());
		st = "Y resolution: " + std::to_string(py);
		TextOut(dc, 4, 20, (LPCSTR)(st.c_str()), st.length());
	}
};

#endif CAMERA_2D_H
