#include <iostream>
#include <cmath>
#include <fstream>
#include <complex>

using std::complex;
using std::ofstream;
using std::endl;
using std::getchar;
using std::floor;

#define ITERCOUNT 20
#define PIXELS 600
#define QUOT 2.4 / PIXELS

complex<double>
mbtFun(complex<double> z, complex<double> c) {
  return z * z + c;
}

int convergance(complex<double> point) {
  int count = 0;
  complex<double> z (0, 0);
  while (count < ITERCOUNT && abs(z) <= 2) {
    z = mbtFun(z, point);
    count++;
  }
  return count;
}

complex<double> setPoint(int i, int j) {
  double x = QUOT * i - 1.8;
  double y = QUOT * j - 1.2;
  return complex<double> (x, y);
}

int main( void ) {
  ofstream ppm;
  ppm.open("mandelbrot_pics/mandelbrot.ppm");

  ppm << "P3" << endl;
  ppm << PIXELS << " " << PIXELS << endl;
  ppm << 255 << endl;

  for (int i = 0; i < PIXELS; i++) {
    for (int j = 0; j < PIXELS; j++) {
      complex<double> point = setPoint(i, j);
      int count = convergance(point);
      if (count == ITERCOUNT) ppm << "0 0 0  ";
      else ppm << floor(((double) count / ITERCOUNT) * 255) << " 0 0  ";
    }
    ppm << endl;
  }

  return 0;
}
