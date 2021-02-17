#define _USE_MATH_DEFINES
#include <cmath>

double					// Исходные значения параметров X0, Y0, px, py
	X0=192,
	Y0=180,
	px=100,
	py=100;

double Parabola(double x) {
	return x*x - 1;
}

double Sinusoid(double x) {
	return sin(x);
}

double Elipse(double fi) {
	return cos(3 * fi) + sin(fi);
}


double Spiral(double fi) {
	return fi / 10;
}
