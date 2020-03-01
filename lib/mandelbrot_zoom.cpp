#include <cmath>
#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "../include/mandelbrot_zoom.h"

using std::complex;
using std::vector;
using std::floor;
using std::max;
using std::string;
using std::ofstream;
using std::endl;

using std::cout;

#define ITERCOUNT 40
int Mandelbrot::itercount = ITERCOUNT;

Mandelbrot::Mandelbrot(
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
) {
  this->dirname = dirname;
  this->pixels = pixels;
  this->nframes = nframes;
  this->speed = speed;
  this->re0 = re0;
  this->re1 = re1;
  this->im0 = im0;
  this->im1 = im1;
  this->zoom_point = zoom_point;
  this->gradient = gradient;
}

inline complex<double> Mandelbrot::mandelbrotPolynomial(
  const complex<double> &z, 
  const complex<double> &c
) {
  return z * z + c;
}

int Mandelbrot::convergance(const complex<double> &point) {
  double eps = 2; 
  int count = 0;
  complex<double> z (0, 0);
  while (count < Mandelbrot::itercount && abs(z) <= eps) {
    z = Mandelbrot::mandelbrotPolynomial(z, point);
    count++;
  }
  return count;
}

complex<double> Mandelbrot::coordTranslate(
  const int &i, const int &j
) {
  return complex<double> (
    (double) i * (re1 - re0) / this->pixels + re0,
    (double) j * (im1 - im0) / this->pixels + im0
  );
}

inline void Mandelbrot::setGradient(ofstream &ppm, const int &count) {
  if (count == this->itercount) ppm << "0 0 0  ";
  else {
    const color &rgb = this->gradient[(count == 1) ? 2 : count];
    ppm << rgb[0] << " " << rgb[1] << " " << rgb[2] << "  ";
  }
}

void Mandelbrot::writePPM(string filename) {
  ofstream ppm;
  ppm.open(filename);
  ppm << "P3" << endl;
  ppm << this->pixels << " " << this->pixels << endl;
  ppm << 255 << endl;
  for (int j = 0; j < this->pixels; j++) {
    for (int i = 0; i < this->pixels; i++) {
      int count = Mandelbrot::convergance(
        Mandelbrot::coordTranslate(i, j)
      );
      Mandelbrot::setGradient(ppm, count);
    }
    ppm << endl;
  }
}

void Mandelbrot::zoom( void ) {
  double rx = this->zoom_point.real();
  double ry = this->zoom_point.imag();
  this->re0 = rx - (rx - this->re0) * this->speed;
  this->im0 = ry - (ry - this->im0) * this->speed;
  this->re1 = rx + (this->re1 - rx) * this->speed;
  this->im1 = ry + (this->im1 - ry) * this->speed;
}

void Mandelbrot::mandelbrotZoom( void )  {
  for (int i = 0; i < this->nframes; i++) {
    string stri = std::to_string(i);
    string filename = this->dirname + "/mandelbrot_" + stri + ".ppm";
    Mandelbrot::writePPM(filename);
    cout << this->re0 << " " << this->re1 << std::endl;
    cout << this->im0 << " " << this->im1 << std::endl;
    Mandelbrot::zoom();
  }
}