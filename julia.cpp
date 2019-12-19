#include <complex>
#include <vector>
#include <algorithm>
#include <iostream>

#include "include/julia_polynomial_series.h"
#include "include/colors.h"

#define ITERLIM 50

using std::complex;
using std::vector;

// The ceofficients returned must be ordered as (an, ..., a0)
// The leading coefficient must be NOWHERE NEAR 0!
complex_polynomial path(double t) {
  return {
    complex<double>(1, 0),
    complex<double>(0, 0),
    complex<double>(t, 0)
  };
}

color_series base_colors {
  {0, 0, 0},
  {10, 10, 40},
  {50, 100, 244},
  {255, 40, 50},
  {0, 255, 255},
  {255, 0, 255}
};


int main( void ) {
  color_series gradient = colors::make_gradient(
    base_colors, ITERLIM + 1
  );
  int nframes = 100;
  double a = -2, b = 1;
  int pixels = 1500;
  Julia julia ("images", nframes, pixels, a, b, &path, gradient);
  julia.staticImageSeries();
  return 0;
}