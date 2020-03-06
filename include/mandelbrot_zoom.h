#ifndef COMPLEX_POLYNOMIAL_DYNAMICS_MANDELBROT_ZOOM_H
#define COMPLEX_POLYNOMIAL_DYNAMICS_MANDELBROT_ZOOM_H

#include <vector>
#include <utility>
#include <complex>
#include <string>
#include <fstream>

using std::vector;
using std::pair;
using std::complex;
using std::string;
using std::ofstream;

typedef vector<int> color;

class Mandelbrot {
private:
	double speed, re0, re1, im0, im1;
	vector<color> gradient;

	int escapetime(
		const complex<double> &point
	);
	complex<double> coordsToComplex(
		const int &i, 
		const int &j
	);
	inline void writeHeaders(
		ofstream &ppm
	);
	inline void writeRGB(
		ofstream &ppm, 
		const int &count
	);
	void writePPM(
		string filename
	);
	void zoom( void );
	
public:
	string dirname;
	int pixels, nframes;
	complex<double> zoom_point;
	vector<color> base_colors;
	static int itercount;

	Mandelbrot (
		string dirname,
		int pixels,
		int nframes,
		double speed,
		complex<double> zoom_point,
		double radius,
		vector<color> base_colors
	);
	void mandelbrotZoom ( void );
};

#endif