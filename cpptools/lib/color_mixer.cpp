#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include "../include/color_mixer.h"

using namespace std::chrono;

unsigned seed = system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);
std::uniform_int_distribution<int> random_int(0, 255);

vector<double> direct(
  const vector<int> &u, 
  const vector<int> &v, 
  int length
) {
  int n = u.size();
  vector<double> direction {};
  for (int i = 0; i < n; i++)
    direction.push_back(1 / (double) length * (double) (v[i] - u[i]));
  return direction;
}

vector<color> colorPath(
  color &rgb1, 
  color &rgb2, 
  int npoints
) {
  vector<color> series {};
  vector<double> direction = direct(rgb1, rgb2, npoints);
  double red = rgb1[0], green = rgb1[1], blue = rgb1[2];
  for (int i = 0; i < npoints; i++) {
    if (series.size() == npoints) break;
    series.push_back({
      static_cast<int>(red), 
      static_cast<int>(green), 
      static_cast<int>(blue)
    });
    red += direction[0];
    green += direction[1];
    blue += direction[2];
  }
  return series;
}

vector<color> color_mixer::makeGradient (
  vector<color> base_colors, 
  int length
) {
  int base_length = base_colors.size();
  int modulo_margin = length / (base_length - 1);
  int leftover = length % (base_length - 1);
  vector<color> series {};
  for (int i = 1; i < base_length; i++) {
    int margin = modulo_margin;
    if (leftover > 0) {
      margin++;
      leftover--;
    }
    vector<color> temp = colorPath(
      base_colors[i-1], base_colors[i], margin
    );
    series.insert(series.end(), temp.begin(), temp.end());
  }
  return series;
}

vector<color> color_mixer::colorChaos(int length) {
  vector<color> color_series {};
  for (int i = 0; i < length; i++) {
    color rgb {-1, -1, -1};
    for (int j = 0; j < 3; j++) 
      rgb[j] = random_int(generator);
    color_series.push_back(rgb);
  }
  return color_series;
}