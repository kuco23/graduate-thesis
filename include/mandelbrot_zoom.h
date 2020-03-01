#ifndef COMPLEX_POLYNOMIAL_DYNAMICS_MANDELBROT_ZOOM_H
#define COMPLEX_POLYNOMIAL_DYNAMICS_MANDELBROT_ZOOM_H

#include <vector>
#include <complex>
#include <string>
#include <fstream>

using std::vector;
using std::complex;
using std::string;
using std::ofstream;

typedef vector<int> color;

class Mandelbrot {
private:
	string dirname;
	int pixels, nframes;
	double speed;
	double re0, re1, im0, im1;
	complex<double> zoom_point;
	vector<color> gradient;

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
	inline void setGradient(
		ofstream &ppm, 
		const int &count
	);
	void writePPM(
		string filename
	);
	void zoom( void );
public:
	static int itercount;
	void mandelbrotZoom ( void );

	Mandelbrot (
		string dirname,
		int pixels,
		int nframes,
		double speed,
		double re0,
		double re1,
		double im0,
		double im1,
		complex<double> zoom_point,
		vector<color> gradient
	);
};

#endif