#include <complex>
#include <vector>
#include <iostream>
#include "include/julia_polynomial_series.h"
#include "include/color_mixer.h"

using std::complex;
using std::vector;

#define M_PI 3.14159265358979323846

// The ceofficients returned are ordered as (an, ..., a0)
// The leading coefficient must be NOWHERE NEAR 0!
complex_polynomial path(double t) {
  return {
    complex<double>(1, 0),
    complex<double>(0.1 * t, 1.2),
    complex<double>(0.1, -0.2*t),
    complex<double>(t / 10, 0)
  };
}

vector<color> base_colors {
  {0, 0, 0},
  {10, 20, 40},
  {50, 100, 200},
  {50, 100, 255},
  {80, 240, 255}
};


int main( void ) {
  vector<color> gradient = color_mixer::makeGradient(base_colors, Julia::itercount);
  int pixels = 1500;
  int nframes = 200;
  double a = -8, b = 8;
  Julia julia ("temp_imgs", nframes, pixels, a, b, &path, gradient);
  julia.dynamicImageSeries();
  return 0;
}