#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <complex>

using std::complex;
using std::floor;
using std::max;
using std::string;
using std::ofstream;
using std::endl;

#define PX 1200
#define NIMAGES 100
#define ITERLIM 40

double re0 = -2, re1 = 2;
double im0 = -2, im1 = 2;
complex<double> center (-2, 0);

complex<double>
mbfun(complex<double> z, complex<double> c) {
  return z * z + c;
}

int convergance(complex<double> point) {
  double eps = max(abs(point), 2.0);
  int count = 0;
  complex<double> z (0, 0);
  while (count < ITERLIM && abs(z) <= eps) {
    z = mbfun(z, point);
    count++;
  }
  return count;
}

complex<double> 
setPoint(int i, int j) {
  double x = (double) i * (re1 - re0) / PX + re0;
  double y = (double) j * (im1 - im0) / PX + im0;
  return complex<double> (x, y);
}

inline int setGradient(const int &count) {
  if (count == ITERLIM)
    return 0;
  return floor(((double) count / ITERLIM) * 255);
}

void writePPM(string filename) {
  ofstream ppm;
  ppm.open(filename);

  ppm << "P3" << endl;
  ppm << PX << " " << PX << endl;
  ppm << 255 << endl;

  for (int j = 0; j < PX; j++) {
    for (int i = 0; i < PX; i++) {
      complex<double> point = setPoint(i, j);
      int count = convergance(point);
      ppm << setGradient(count) << " 0 0  ";
    }
    ppm << endl;
  }

}

void zoom(const double &speed) {
  double center_re = center.real();
  double center_im = center.imag();
  re0 = center_re - (center_re - re0) * speed;
  im0 = center_im - (center_im - im0) * speed;
  re1 = center_re + (re1 - center_re) * speed;
  im1 = center_im + (im1 - center_im) * speed;
}

int main( void ) {
  double speed = 0.9;

  for (int i = 0; i < NIMAGES; i++) {
    string stri = std::to_string(i);
    string filename = "images/mandelbrot_" + stri + ".ppm";
    writePPM(filename);
    zoom(speed);
  }

  return 0;
}
