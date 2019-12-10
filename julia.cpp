#include <vector>
#include <complex>
#include <iostream>

#include "include/julia_polynomial_series.h"

using std::vector;
using std::complex;

// define a Julia:: in a cartesian product of complex planes
// through which a series of julia sets will be plotted
// the vector returned must be ordered from as (an, ..., a0)
// KEEP THE LEADING COEFFICIENT FAR AWAY FROM 0
complex_polynomial path(double t) {
  return {
    complex<double>(1, 0),
    complex<double>(0, 0),
    complex<double>(t, 0)
  };
}

inline void colorStream(ofstream &ppm, const int &count, int iterlim) {
  double ratio = (count == iterlim) ? 0 : (double) count / iterlim;
  if (count == 1) ratio = (double) 2 / iterlim; // simulation's gradient fix
  ppm << std::floor(ratio * 255) << " 0 0  ";
}

int main( void ) {
    Julia julia ("images", 10, -1, 2, 600, &path, &colorStream);
    julia.dynamicImageSeries();
    return 0;
}