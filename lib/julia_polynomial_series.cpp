#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <fstream>

#include "../include/julia_polynomial_series.h"

using std::complex;
using std::vector;
using std::string;
using std::ofstream;
using std::endl;
using std::max;

#define M_PI 3.14159265358979323846
const complex<double> i (0, 1);

#define ITERCOUNT 50
#define PHISPLIT 60

const int Julia::itercount = ITERCOUNT;
const int Julia::phisplit = PHISPLIT;
const double Julia::dphi = 2 * M_PI / (double) PHISPLIT;

Julia::Julia (
  string dirname, 
  int nframes, 
  int pixels, 
  double t0, 
  double t1, 
  complex_path path, 
  vector<color> gradient
) {
  this->dirname = dirname;
  this->nframes = nframes;
  this->pixels = pixels;
  this->t0 = t0; 
  this->t1 = t1;
  this->path = path;
  this->gradient = gradient;
  this->dt = (t1 - t0) / nframes;
}

// theoretic convergence limit for a specific polynomial
double Julia::theoreticEps(const complex_polynomial &coefs) {
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
  const complex_polynomial &coefs, 
  const complex<double> &z
) {
  complex<double> sum (0, 0);
  for (const complex<double> &a_i : coefs)
    sum = sum * z + a_i;
  return sum;
}

int Julia::escapeCount(
  complex<double> z, 
  const complex_polynomial &coefs, 
  const double &eps
) {
  int count = 1;
  while (count < ITERCOUNT && abs(z) <= eps) {
    z = Julia::horner(coefs, z);
    count++;
  }
  return count;
}

// simulated convergance limit for a specific polynomial
double Julia::simulatedEps(
  const complex_polynomial &coefs, 
  const double &itereps
) {
  double maxradius = 0;
  double r = 2 * itereps / (double) pixels;
  for (double phi = 0; phi < 2 * M_PI; phi += dphi) {
    complex<double> z;
    complex<double> dz = r * exp(i * phi);
    complex<double> z0 = (double) pixels * dz;
    for (z = z0; abs(z) > r; z -= dz) {
      int count = Julia::escapeCount(z, coefs, itereps);
      if (count > 2) break;
    }
    if (abs(z) > maxradius) maxradius = abs(z) + r;
  }
  return maxradius;
}

inline complex<double> Julia::coordTranslate(
  const int &i, const int &j, const double &eps
) {
  double x = 2 * eps * (double) i / pixels - eps;
  double y = 2 * eps * (double) j / pixels - eps;
  return complex<double> (x, y);
}

inline void Julia::colorStream(ofstream &ppm, const int &count) {
  if (count == ITERCOUNT) ppm << "0 0 0  ";
  else {
    const color &rgb = this->gradient[(count == 1) ? 2 : count];
    ppm << rgb[0] << " " << rgb[1] << " " << rgb[2] << "  ";
  }
}

void Julia::writeJuliaPpm(
  const complex_polynomial &coefs, 
  const double &eps,
  const string &filename
) {
  ofstream ppm;
  ppm.open(filename);
  ppm << "P3" << endl;
  ppm << pixels << " " << pixels << endl;
  ppm << 255 << endl;
  for (int j = 0; j < pixels; j++) {
    for (int i = 0; i < pixels; i++) {
      complex<double> z = coordTranslate(i, j, eps);
      int count = Julia::escapeCount(z, coefs, eps);
      Julia::colorStream(ppm, count);
    }
    ppm << endl;
  }
}

double Julia::staticEps( void ) {
  double t = t0, eps = 0;
  for (int i = 0; i < this->nframes; i++) {
    t += dt; 
    const complex_polynomial coefs = Julia::path(t);
    double teps = Julia::theoreticEps(coefs);
    double seps = Julia::simulatedEps(coefs, teps);
    if (eps < seps) eps = seps;
  }
  return eps;
}

void Julia::staticImageSeries( void ) {
  double t = t0, eps = Julia::staticEps();
  for (int i = 0; i < nframes; i++) {
    t += dt;
    string stri = std::to_string(i);
    string filename = dirname + "/julia_" + stri + ".ppm";
    const complex_polynomial coefs = Julia::path(t);
    Julia::writeJuliaPpm(coefs, eps, filename);
  }
}

void Julia::dynamicImageSeries( void ) {
  double t = t0;
  for (int i = 0; i < nframes; i++) {
    t += dt;
    string stri = std::to_string(i);
    string filename = this->dirname + "/julia_" + stri + ".ppm";
    const complex_polynomial coefs = Julia::path(t);
    double teps = Julia::theoreticEps(coefs);
    double seps = Julia::simulatedEps(coefs, teps);
    Julia::writeJuliaPpm(coefs, seps, filename);
  }
}
