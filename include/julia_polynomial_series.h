#ifndef COMPLEX_POLYNOMIAL_DYNAMICS_JULIA_POLYNOMIAL_SERES_H
#define COMPLEX_POLYNOMIAL_DYNAMICS_JULIA_POLYNOMIAL_SERES_H

#include <complex>
#include <vector>
#include <string>
#include <fstream>
#include <functional>

using std::complex;
using std::vector;
using std::string;
using std::ofstream;
using std::function;

typedef vector<complex<double>> complex_polynomial;
typedef function<complex_polynomial(double)> complex_path;
typedef vector<int> color;

class Julia {
private:
    string dirname;
    complex_path path;
    vector<color> gradient;
    int nframes, pixels;
    double t0, t1, dt;
    static const double dphi;

    double theoreticEps(
        const complex_polynomial &coefs
    );
    inline complex<double> horner(
        const complex_polynomial &coefs, 
        const complex<double> &z
    );
    double simulatedEps(
        const complex_polynomial &coefs, 
        const double &itereps
    );
    double staticEps( void );
    int escapeCount(
        complex<double> z, 
        const complex_polynomial &coefs,
        const double &eps
    );
    inline complex<double> coordTranslate(
        const int &i, 
        const int &j, 
        const double &eps
    );
    inline void colorStream(
        ofstream &ppm, 
        const int &count
    );
    void writeJuliaPpm(
        const complex_polynomial &coefs, 
        const double &eps, 
        const string &filename
    );
public:
    static const int itercount;
    static const int phisplit;
    Julia (
        string dirname, 
        int nframes, 
        int pixels,
        double t0, 
        double t1, 
        complex_path path, 
        vector<color> base_colors
    );
    void staticImageSeries( void );
    void dynamicImageSeries( void );
};

#endif