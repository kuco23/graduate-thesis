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

#define M_PI 3.14159265358979323846
const complex<double> i (0, 1);

typedef vector<complex<double>> complex_polynomial;
typedef function<complex_polynomial(double)> complex_path;
typedef function<void(ofstream&, const int&)> ppm_stream;

class Julia {
private:
    string dirname;
    complex_path path;
    ppm_stream stream;
    int nframes, pixels;
    double t0, t1, dt;

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
    int convergance(
        complex<double> z, 
        const complex_polynomial &coefs,
        const double &eps
    );
    inline complex<double> coordTranslate(
        const int &i, 
        const int &j, 
        const double &eps
    );
    inline void ppmBasicColorStream(
        ofstream &ppm, 
        const int &count
    );
    void writeJuliaPpm(
        const complex_polynomial &coefs, 
        const double &eps, 
        const string &filename
    );
public:
    Julia (
        string dirname, int nframes, 
        double t0, double t1, int pixels,
        complex_path path, ppm_stream stream
    );
    void staticImageSeries( void );
    void dynamicImageSeries( void );
};

#endif