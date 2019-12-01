#include <cmath>
#include <complex>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

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

inline complex<double>
mbfun(const complex<double> &z, const complex<double> &c) {
  return z * z + c;
}

int convergance(const complex<double> &point) {
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
coordTranslate(const int &i, const int &j) {
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
      int count = convergance(coordTranslate(i, j));
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
