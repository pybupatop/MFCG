#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"

class Scene2D : public Camera2D {
private:
	typedef double (*Func)(double);
public:
	Scene2D(double L, double R, double B, double T) : Camera2D(L, R, B, T) {
	}
	void Plot(HDC dc, Func f, bool axes=true) {
		if (axes)
			Axes(dc);
		int N = W;
		double h = (R - L) / N;
		Camera2D::MoveTo(dc, L, 0);
		for (double i = L ; i <= R ; i+= h) {
			Camera2D::LineTo(dc,i, f(i));
		}
		// ���������� ������� ������� f
		// ����� ������������ � �������� ��������� ������� f ���������� ��� ������� �������:
		// f(x);
	}
};

#endif SCENE_2D_H
