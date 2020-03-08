#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "../include/mandelbrot_zoom.h"
#include "../include/color_mixer.h"

using std::complex;
using std::vector;
using std::string;
using std::to_string;
using std::ofstream;
using std::endl;
using std::max;

#define MIN_ITERCOUNT 300
#define EPS 2

Mandelbrot::Mandelbrot(
  string dirname, 
  int nframes,
  int pixels,
  double speed,
  complex<double> zoom_point,
  double radius,
  vector<color> base_colors
) {
  this->dirname = dirname;
  this->nframes = nframes;
  this->pixels = pixels;
  this->speed = 1 - speed;
  this->zoom_point = zoom_point;
  this->base_colors = base_colors;
  this->re0 = zoom_point.real() - radius;
  this->re1 = zoom_point.real() + radius;
  this->im0 = zoom_point.imag() - radius;
  this->im1 = zoom_point.imag() + radius;
  this->dre = (re1 - re0) / pixels;
  this->dim = (im1 - im0) / pixels;
}

int Mandelbrot::escapetime(const complex<double> &point) {
  int count;
  complex<double> z (0, 0);
  for (count = 0; count < itercount && abs(z) <= EPS; count++)
    z = z * z + point;
  return count;
}

inline complex<double> Mandelbrot::coordsToComplex(
  const int &i, const int &j
) {
  return complex<double> (
    (double) i * this->dre + re0,
    (double) j * this->dim + im0
  );
}

inline void Mandelbrot::writeHeaders(ofstream &ppm) {
  ppm << "P3" << endl;
  ppm << this->pixels << " " << this->pixels << endl;
  ppm << 255 << endl;
}

inline void Mandelbrot::writeRGB(
  ofstream &ppm, const int &count
) {
  if (count == this->itercount) ppm << "0 0 0  ";
  else {
    const color &rgb = this->gradient[count];
    ppm << rgb[0] << " " << rgb[1] << " " << rgb[2] << "  ";
  }
}

void Mandelbrot::writePPM(string filename) {
  ofstream ppm;
  ppm.open(filename);
  writeHeaders(ppm);
  for (int j = 0; j < this->pixels; j++) {
    for (int i = 0; i < this->pixels; i++)
      writeRGB(ppm, escapetime(coordsToComplex(i, j)));
    ppm << endl;
  }
}

void Mandelbrot::zoom( void ) {
  double rx = this->zoom_point.real();
  double ry = this->zoom_point.imag();
  this->re0 = rx - (rx - this->re0) * this->speed;
  this->re1 = rx + (this->re1 - rx) * this->speed;
  this->im0 = ry - (ry - this->im0) * this->speed;
  this->im1 = ry + (this->im1 - ry) * this->speed;
  this->dre = (this->re1 - this->re0) / this->pixels;
  this->dim = (this->im1 - this->im0) / this->pixels;
}

void Mandelbrot::setItercount( void ) {
  this->itercount = max(
    1.0 / (1000 * (re1 - re0)), 
    (double) MIN_ITERCOUNT 
  );
}

void Mandelbrot::setGradient( void ) {
  this->gradient = color_mixer::makeGradient(
    this->base_colors, this->itercount
  );
}

void Mandelbrot::mandelbrotZoom( void )  {
  for (int i = 0; i < this->nframes; i++) {
    setItercount();
    setGradient();
    writePPM(dirname + "/mandelbrot_" + to_string(i) + ".ppm");
    zoom();
  }
}