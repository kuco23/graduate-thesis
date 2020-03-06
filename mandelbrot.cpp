#include <string>
#include <vector>
#include "include/mandelbrot_zoom.h"
#include "include/color_mixer.h"

using std::vector;

const vector<color> base_colors {
  {0, 0, 0},
  {153, 255, 255},
  {10, 20, 30},
  {255, 128, 0},
  {102, 178, 255},
  {255,0,255},
  {204, 204, 255},
  {51, 255, 51},
  {153, 255, 255}
};

int main( void ) {
  Mandelbrot mandelzoom (
    "temp_imgs", 2000, 500, 0.03,
    complex<double> (0.39, 0.18), 1,
    base_colors
  );
  mandelzoom.mandelbrotZoom();
  return 0;
}