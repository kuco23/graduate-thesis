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

#define MIN_ITERCOUNT 3000
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
  this->r = radius;
}

int Mandelbrot::escapetime(const complex<double> &point) {
  int count;
  complex<double> z (0, 0);
  for (count = 0; count < itercount && abs(z) <= EPS; count++)
    z = z * z + point;
  return count;
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

void Mandelbrot::zoom( void ) {
  this->r *= this->speed;
}

void Mandelbrot::setSquare ( void ) {
  this->re0 = zoom_point.real() - r;
  this->re1 = zoom_point.real() + r;
  this->im0 = zoom_point.imag() - r;
  this->im1 = zoom_point.imag() + r;
}

void Mandelbrot::setSteps ( void ) {
  this->dre = (re1 - re0) / pixels;
  this->dim = (im1 - im0) / pixels;
}

void Mandelbrot::setItercount( void ) {
  this->itercount = (int) max(
    10653.0,
    //5.0 / sqrt(r), 
    (double) MIN_ITERCOUNT 
  );
}

void Mandelbrot::setGradient( void ) {
  this->gradient = color_mixer::makeGradient(
    this->base_colors, this->itercount
  );
}

void Mandelbrot::writePPM(string filename) {
  setSquare();
  setSteps();
  setItercount();
  setGradient();
  ofstream ppm;
  ppm.open(filename);
  writeHeaders(ppm);
  complex<double> z = complex<double> (re0, im0);
  complex<double> dz_re = complex<double> (dre, 0);
  complex<double> dz_im = complex<double> (0, dim);
  for (int j = 0; j < this->pixels; j++) {
    for (int i = 0; i < this->pixels; i++) {
      writeRGB(ppm, escapetime(z));
      z += dz_re;
    }
    z += dz_im;
    z.real(re0);
    ppm << endl;
  }
  ppm.close();
}

void Mandelbrot::mandelbrotZoom( void )  {
  for (int i = 0; i < this->nframes; i++) {
    writePPM(dirname + "/mandelbrot_" + to_string(i) + ".ppm");
    zoom();
  }
}