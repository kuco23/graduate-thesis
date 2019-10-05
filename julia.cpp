#include <iostream>
#include <cmath>
#include <fstream>
#include <complex>

using std::complex;
using std::ofstream;
using std::endl;
using std::floor;

#define PIXELS 8000
#define ITERLIM 40

complex<double> c = complex<double>(-1.2, -0.1);
double eps = std::max((double) 2, abs(c));

int convergance(complex<double> z) {
  int count = 0;
  while (count < ITERLIM && abs(z) <= eps) {
    z = pow(z, 2) + c;
    count++;
  }
  return count;
}

complex<double> setPoint(int i, int j) {
  double x = 4 * (double) i / PIXELS - 2;
  double y = 4 * (double) j / PIXELS - 2;
  return complex<double> (x, y);
}

int main( void ) {
  ofstream ppm;
  ppm.open("julia.ppm");

  ppm << "P3" << endl;
  ppm << PIXELS << " " << PIXELS << endl;
  ppm << 255 << endl;

  for (int j = 0; j < PIXELS; j++) {
    for (int i = 0; i < PIXELS; i++) {
      int count = convergance(setPoint(i, j));
      if (count == ITERLIM) ppm << "0 0 0  ";
      else {
        int r = floor(((double) count / ITERLIM) * 255);
        ppm << r << " 0 0  ";
      }
    }
    ppm << endl;
  }

  return 0;
}
