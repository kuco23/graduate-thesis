#ifndef COMPLEX_POLYNOMIAL_DYNAMICS_COLOR_MIXER_H
#define COMPLEX_POLYNOMIAL_DYNAMICS_COLOR_MIXER_H

#include <vector>
using std::vector;

typedef vector<int> color;

namespace color_mixer {
  vector<color> makeGradient(vector<color> base_colors, int length);
  vector<color> colorChaos(int length);
}

#endif