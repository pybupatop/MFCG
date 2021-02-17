#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"

class Scene2D : public Camera2D {
private:
	typedef double (*Func)(double);
public:
	Scene2D(double X0, double Y0, double px, double py) : Camera2D(X0, Y0, px, py) {
	}
	void Plot(HDC dc, Func f, bool axes = true, bool resolution = false) {
		if (axes)
			Axes(dc);
		double left = L(), right = R();
		double h = (R() - L()) / W;
		double x = left;
		MoveTo(x, f(x));
		while (x < right) {
			x += h;
			LineTo(dc, x, f(x));
		}
	}

	void ElispsPlot(HDC dc, Func f,double h = 0.001, double Period = M_PI * 2, bool axes = true, bool resolution = true) {
		if (axes)
			Axes(dc);
		if (resolution)
			Resolution(dc);
		double x, y;
		double mu = f(0);
		x = cosh(mu) * cos(0);
		y = sinh(mu) * sin(0);
		Camera2D::MoveTo(x, y);
		double nu;
		for (nu = h; nu < Period; nu += h) {
			mu = f(nu);
			x = cosh(mu) * cos(nu);
			y = sinh(mu) * sin(nu);
			Camera2D::LineTo(dc, x, y);
		}
		mu = f(Period);
		x = cosh(mu) * cos(nu);
		y = sinh(mu) * sin(nu);
		Camera2D::LineTo(dc, x, y);
	}
};

#endif SCENE_2D_H
