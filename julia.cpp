#include <complex>
#include <vector>
#include <algorithm>
#include <iostream>

#include "include/julia_polynomial_series.h"
#include "include/color_picker.h"

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

static color_series colors {};
inline void colorStream(ofstream &ppm, const int &count) {
  if (count == ITERLIM) ppm << "0 0 0  ";
  else {
    const vector<int> &picker = colors[(count == 1) ? 2 : count];
    ppm << picker[0] << " " << picker[1] << " " << picker[2] << "  ";
  }
}

int main( void ) {
  colors = color_picker::gradient(255, 255, 0, ITERLIM);
  int nframes = 10;
  double a = -2, b = 1;
  int pixels = 1500;
  Julia julia ("images", nframes, a, b, pixels, &path, &colorStream);
  julia.staticImageSeries();
  return 0;
}