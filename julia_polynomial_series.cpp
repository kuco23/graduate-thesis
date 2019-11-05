#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <iostream>
#include <fstream>

using std::complex;
using std::vector;
using std::string;
using std::ofstream;
using std::endl;
using std::max;
using std::min;
using std::floor;

#define PIXELS 1200
#define ITERLIM 50
#define NIMAGES 100

double t0 = -1;
double t1 = 2;
double step = (t1 - t0) / NIMAGES;

// define a path in a cartesian product of complex planes
// through which a series of julia sets will be plotted
// the vector returned must be ordered from as (an, ..., a0)
vector<complex<double>> path(double t) {
  return {
    complex<double>(0.4*t, 0),
    complex<double>(-t, 0.2*t),
    complex<double>(0, 0),
    complex<double>(-1.2+t, -0.2)
  };
}

// theoretic convergence limit for a specific polynomial
double getTheoreticEps(vector<complex<double>> &coefs) {
  int n = coefs.size();
  double an = abs(coefs[0]);
  double sum = 0;
  for (int i = 1; i < n; i++) sum += abs(coefs[i]);
  return max(
    max(1.0, 2 * sum / an), 
    pow(2 * 1.0001 / an, 1 / (double) (n - 2)))
  ;
}

inline complex<double> horner(
  const vector<complex<double>> &coefs, 
  const complex<double> &z
) {
  complex<double> sum (0, 0);
  for (const complex<double> &a_i : coefs)
    sum = sum * z + a_i;
  return sum;
}

int convergance(
  complex<double> z, 
  const vector<complex<double>> &coefs, 
  const double &eps
) {
  int count = 1;
  while (count < ITERLIM && abs(z) <= eps) {
    z = horner(coefs, z);
    count++;
  }
  return count;
}

complex<double> coordTranslate(const int &i, const int &j, const double &eps) {
  double x = 2 * eps * (double) i / PIXELS - eps;
  double y = 2 * eps * (double) j / PIXELS - eps;
  return complex<double> (x, y);
}

// convergence limit based on a test simulation
// return abs(z) for which the convergence 'starts'
double getSimulatedEps(const vector<complex<double>> &coefs, const double &itereps) {
  complex<double> z_middle;
  int i = floor(PIXELS / 2);
  for (int j = 0; j < PIXELS; j++) {
    complex<double> z_middle = coordTranslate(j, i, itereps);
    int count = convergance(z_middle, coefs, itereps);
    if (count > 2) return abs(z_middle);
  }
  return itereps;
}

inline void ppmBasicColorStream(ofstream &ppm, const int &count) {
  double ratio = (count == ITERLIM) ? 0 : (double) count / ITERLIM;
  ppm << floor(ratio * 255) << " 0 0  ";
}

void writeJuliaPpm(vector<complex<double>> &coefs, string filename) {
  double theor_eps = getTheoreticEps(coefs);
  double simul_eps = getSimulatedEps(coefs, theor_eps);

  ofstream ppm;
  ppm.open(filename);
  
  ppm << "P3" << endl;
  ppm << PIXELS << " " << PIXELS << endl;
  ppm << 255 << endl;

  for (int j = 0; j < PIXELS; j++) {
    for (int i = 0; i < PIXELS; i++) {
      complex<double> z = coordTranslate(i, j, simul_eps);
      int count = convergance(z, coefs, simul_eps);
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
    
    vector<complex<double>> coefs = path(t);
    writeJuliaPpm(coefs, filename);
    std::cout << i << endl;
  }
  return 0;
}
