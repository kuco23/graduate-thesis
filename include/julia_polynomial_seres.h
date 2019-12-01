#ifndef COMPLEX_POLYNOMIAL_DYNAMICS_JULIA_POLYNOMIAL_SERES_H
#define COMPLEX_POLYNOMIAL_DYNAMICS_JULIA_POLYNOMIAL_SERES_H

#include <complex>
#include <vector>
#include <string>
#include <fstream>

using std::complex;
using std::vector;
using std::string;
using std::ofstream;

class Julia {
private:
    vector<complex<double>> path(double t);
    double theoreticEps(const vector<complex<double>> &coefs);
    inline complex<double> horner(
        const vector<complex<double>> &coefs, const complex<double> &z
    );
    double simulatedEps(
        const vector<complex<double>> &coefs, const double &itereps
    );
    double staticEps( void );
    int convergance(
        complex<double> z, const vector<complex<double>> &coefs,
        const double &eps
    );
    inline complex<double> coordTranslate(
        const int &i, const int &j, const double &eps
    );
    inline void ppmBasicColorStream(ofstream &ppm, const int &count);
    void writeJuliaPpm(
        const vector<complex<double>> &coefs, 
        const double &eps, const string &filename
    );
public:
    void imageSeries(const string dirname, const bool static_img);
};

#endif