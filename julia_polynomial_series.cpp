#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

using std::complex;
using std::vector;
using std::string;
using std::ofstream;
using std::endl;

#define PIXELS 1200
#define ITERLIM 50
#define NIMAGES 1

double t0 = -0.8;
double t1 = 0.8;
double step = (t1 - t0) / NIMAGES;

complex<double> path(double t) {
  return complex<double>(-1.2 + std::abs(t), t);
}

inline complex<double> 
horner(const vector<complex<double>> &coefs, const complex<double> &z) {
  complex<double> sum (0, 0);
  for (int i = coefs.size() - 1; i >= 0; i--)
    sum += sum * z + coefs[i];
  return sum;
}

int convergance(complex<double> z, complex<double> &c, double &eps) {
  int count = 1;
  while (count < ITERLIM && abs(z) <= eps) {
    z = pow(z, 2) + c;
    count++;
  }
  return count;
}

complex<double> coordTranslate(const int &i, const int &j) {
  double x = 4 * (double) i / PIXELS - 2;
  double y = 4 * (double) j / PIXELS - 2;
  return complex<double> (x, y);
}

inline void ppmBasicColorStream(ofstream &ppm, const int &count) {
  double ratio = (count == ITERLIM) ? 0 : (double) count / ITERLIM;
  ppm << std::floor(ratio * 255) << " 0 0  ";
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
      int count = convergance(coordTranslate(i, j), c, eps);
      ppmBasicColorStream(ppm, count);
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
    std::cout << i << endl;
  }
  return 0;
}
