#include <complex>
#include <vector>
#include <algorithm>
#include <iostream>

#include "include/julia_polynomial_series.h"
#include "include/colors.h"

#define M_PI 3.14159265358979323846

using std::complex;
using std::vector;

// The ceofficients returned are ordered as (an, ..., a0)
// The leading coefficient must be NOWHERE NEAR 0!
complex_polynomial path(double t) {
  return {
    complex<double>(0.8, 0),
    complex<double>(sin(t) / 2, 1 / 2 - cos(t *1.2) / (abs(t)+1)),
    complex<double>(1 - t / 2, t * sin(t) / 2),
    complex<double>(sin(t) / (abs(t) + 2), -t / 2)
  };
}

vector<color> base_colors {
  {0, 0, 0},
  {10, 10, 40},
  {100, 255, 0},
  {120, 255, 0},
  {140, 255, 0},
  {160, 255, 0},
  {180, 255, 0},
  {200, 255, 0}
};

int main( void ) {
  vector<color> gradient = colors::make_gradient(
    base_colors, Julia::itercount + 1
  );
  int pixels = 2000;
  int nframes = 250;
  double a = -M_PI, b = M_PI;
  Julia julia ("images", nframes, pixels, a, b, &path, gradient);
  julia.staticImageSeries();
  return 0;
}