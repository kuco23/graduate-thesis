#include "include/julia_polynomial_seres.h"

int main( void ) {
    Julia julia;
    string dirname = "images";
    julia.imageSeries(dirname, true);
    return 0;
}