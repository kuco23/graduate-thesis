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

#define ITERLIM 50
#define PHISPLIT 60
#define dphi 2 * M_PI / (double) PHISPLIT

Julia::Julia (
  string dirname, 
  int nframes, 
  double t0, 
  double t1, 
  int pixels, 
  complex_path path, 
  ppm_stream stream
) {
  this->nframes = nframes;
  this->pixels = pixels;
  this->t0 = t0; 
  this->t1 = t1;
  this->dirname = dirname;
  this->path = path;
  this->stream = stream;
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

int Julia::convergance(
  complex<double> z, 
  const complex_polynomial &coefs, 
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
  double x = 2 * eps * (double) i / pixels - eps;
  double y = 2 * eps * (double) j / pixels - eps;
  return complex<double> (x, y);
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
      int count = Julia::convergance(z, coefs, eps);
      Julia::stream(ppm, count);
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
