#ifndef COMPLEX_POLYNOMIAL_DYNAMICS_MANDELBROT_ZOOM_H
#define COMPLEX_POLYNOMIAL_DYNAMICS_MANDELBROT_ZOOM_H

#include <complex>
#include <string>
#include <iostream>

using std::complex;
using std::string;
using std::ofstream;

class Mandelbrot {
private:
	inline complex<double> mandelbrotPolynomial(
		const complex<double> &z, 
		const complex<double> &c
	);
	int convergance(
		const complex<double> &point
	);
	complex<double> coordTranslate(
		const int &i, 
		const int &j
	);
	inline int setGradient(
		const int &count
	);
	void writePPM(
		string filename
	);
	void imageZoom(
		const double &speed
	);
public:
	void zoom (
		double speed, 
		int frames
	);
};

#endif