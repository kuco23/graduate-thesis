#include <string>
#include <vector>
#include "include/colors.h"
#include "include/mandelbrot_zoom.h"

using std::vector;

vector<color> base_colors {
  {0, 0, 0},
  {10, 20, 40},
  {50, 100, 200},
  {50, 100, 255},
  {80, 240, 255}
};

int main( void ) {
  vector<color> gradient = colors::make_gradient(
    base_colors, Mandelbrot::itercount
  );
  Mandelbrot mandelzoom (
    "images", 1000, 100, 0.95,
    -2, 2, -2, 2, complex<double> (1, 0),
    gradient
  );
  mandelzoom.mandelbrotZoom();
  return 0;
}