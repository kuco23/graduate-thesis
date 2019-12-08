#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <iostream>
#include <fstream>

#include "../include/julia_polynomial_series.h"

using std::complex;
using std::vector;
using std::string;
using std::ofstream;
using std::endl;
using std::max;

#define M_PI 3.14159265358979323846
#define ITERLIM 50
#define PHISPLIT 60
#define PIXELS 800
#define NIMAGES 40

const complex<double> i (0, 1);
const double dphi = 2 * M_PI / (double) PHISPLIT;
const double t0 = -2, t1 = 1;
const double dt = (t1 - t0) / NIMAGES;

// define a Julia:: in a cartesian product of complex planes
// through which a series of julia sets will be plotted
// the vector returned must be ordered from as (an, ..., a0)
// KEEP THE LEADING COEFFICIENT FAR AWAY FROM 0
vector<complex<double>> Julia::path(double t) {
  return {
    complex<double>(1, 0),
    complex<double>(0, 0),
    complex<double>(t, 0)
  };
}

// theoretic convergence limit for a specific polynomial
double Julia::theoreticEps(const vector<complex<double>> &coefs) {
  int n = coefs.size();
  double an = abs(coefs[0]);
  double sum = 0;
  for (int i = 1; i < n; i++) sum += abs(coefs[i]);
  return max(
    max(1.0, 2 * sum / an), 
    pow(2 * 1.0001 / an, 1 / (double) (n - 2)))
  ;
}

inline complex<double> Julia::horner(
  const vector<complex<double>> &coefs, 
  const complex<double> &z
) {
  complex<double> sum (0, 0);
  for (const complex<double> &a_i : coefs)
    sum = sum * z + a_i;
  return sum;
}

int Julia::convergance(
  complex<double> z, 
  const vector<complex<double>> &coefs, 
  const double &eps
) {
  int count = 1;
  while (count < ITERLIM && abs(z) <= eps) {
    z = Julia::horner(coefs, z);
    count++;
  }
  return count;
}

// convergence limit based on a test simulation
double Julia::simulatedEps(
  const vector<complex<double>> &coefs, 
  const double &itereps
) {
  double maxradius = 0;
  double r = 2 * itereps / (double) PIXELS;
  for (double phi = 0; phi < 2 * M_PI; phi += dphi) {
    complex<double> z;
    complex<double> dz = r * exp(i * phi);
    complex<double> z0 = (double) PIXELS * dz;
    for (z = z0; abs(z) > r; z -= dz) {
      int count = Julia::convergance(z, coefs, itereps);
      if (count > 2) break;
    }
    if (abs(z) > maxradius) maxradius = abs(z) + r;
  }
  return maxradius;
}

inline complex<double> Julia::coordTranslate(
  const int &i, const int &j, const double &eps
) {
  double x = 2 * eps * (double) i / PIXELS - eps;
  double y = 2 * eps * (double) j / PIXELS - eps;
  return complex<double> (x, y);
}

inline void Julia::ppmBasicColorStream(ofstream &ppm, const int &count) {
  double ratio = (count == ITERLIM) ? 0 : (double) count / ITERLIM;
  if (count == 1) ratio = (double) 2 / ITERLIM; // simulation's gradient fix
  ppm << std::floor(ratio * 255) << " 0 0  ";
}

void Julia::writeJuliaPpm(
  const vector<complex<double>> &coefs, 
  const double &eps,
  const string &filename
) {
  ofstream ppm;
  ppm.open(filename);

  ppm << "P3" << endl;
  ppm << PIXELS << " " << PIXELS << endl;
  ppm << 255 << endl;

  for (int j = 0; j < PIXELS; j++) {
    for (int i = 0; i < PIXELS; i++) {
      complex<double> z = coordTranslate(i, j, eps);
      int count = Julia::convergance(z, coefs, eps);
      Julia::ppmBasicColorStream(ppm, count);
    }
    ppm << endl;
  }

}

double Julia::staticEps( void ) {
  double t = t0, eps = 0;
  for (int i = 0; i < NIMAGES; i++) {
    t += dt; 
    const vector<complex<double>> coefs = Julia::path(t);
    double teps = Julia::theoreticEps(coefs);
    double seps = Julia::simulatedEps(coefs, teps);
    if (eps < seps) eps = seps;
  }
  return eps;
}

void Julia::imageSeries(
  const string dirname, 
  const bool static_img
) {
  double eps = (static_img) ? Julia::staticEps() : 0;
  double t = t0;
  for (int i = 0; i < NIMAGES; i++) {
    t += dt;

    string stri = std::to_string(i);
    string filename = dirname + "/julia_" + stri + ".ppm";

    const vector<complex<double>> coefs = Julia::path(t);
    if (!static_img) {
      double teps = Julia::theoreticEps(coefs);
      double seps = Julia::simulatedEps(coefs, teps);
      Julia::writeJuliaPpm(coefs, seps, filename);
    } else Julia::writeJuliaPpm(coefs, eps, filename);
  }
}


