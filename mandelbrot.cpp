#include "include/mandelbrot_zoom.h"

Mandelbrot mandelib;

int main( void ) {
  double speed = 0.9;
  int frames = 40;
  mandelib.zoom(speed, frames);
  return 0;
}