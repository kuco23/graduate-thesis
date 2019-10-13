#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <complex>

using std::complex;
using std::ofstream;
using std::string;
using std::endl;
using std::floor;

#define PIXELS 1200
#define ITERLIM 50
#define NIMAGES 100

double t0 = -1;
double t1 = 1;
double step = (t1 - t0) / NIMAGES;

complex<double> path(double t) {
  return complex<double>(t / (4 * (t + 1.1)), t * 0.4);
}

int convergance(complex<double> z, complex<double>& c, double& eps) {
  int count = 0;
  while (count < ITERLIM && abs(z) <= eps) {
    z = pow(z, 2) + c;
    count++;
  }
  return count;
}

complex<double> setPoint(int& i, int& j) {
  double x = 4 * (double) i / PIXELS - 2;
  double y = 4 * (double) j / PIXELS - 2;
  return complex<double> (x, y);
}

inline int setGradient(const int &count) {
  if (count == ITERLIM)
    return 0;
  else
    return floor(((double) count / ITERLIM) * 255);
}

void writeJuliaPpm(complex<double> c, string filename) {
  double eps = std::max(abs(c), (double) 2);

  ofstream ppm;
  ppm.open(filename);
  
  ppm << "P3" << endl;
  ppm << PIXELS << " " << PIXELS << endl;
  ppm << 255 << endl;

  for (int j = 0; j < PIXELS; j++) {
    for (int i = 0; i < PIXELS; i++) {
      int count = convergance(setPoint(i, j), c, eps);
      int r = setGradient(count);
      ppm << r << " 0 0  ";
    }
    ppm << endl;
  }

}

int main( void ) {
  double t = t0;
  for (int i = 0; i < NIMAGES; i++) {
    t += step;
    string stri = std::to_string(i);
    string filename = "images/julia_" + stri + ".ppm";
    writeJuliaPpm(path(t), filename);
  }
  return 0;
}
