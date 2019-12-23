#include <cmath>
#include "../include/colors.h"

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

vector<color> color_path(
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

vector<color> colors::make_gradient(
  vector<color> base_colors, 
  int length
) {
  int base_length = base_colors.size();
  int modulo_margin = std::floor((double) length / (base_length - 1));
  int leftover = length % modulo_margin;
  vector<color> series {};
  for (int i = 1; i < base_length; i++) {
    int margin = modulo_margin;
    if (leftover > 0) {
      margin++;
      leftover--;
    }
    vector<color> temp = color_path(base_colors[i-1], base_colors[i], margin);
    series.insert(series.end(), temp.begin(), temp.end());
  }
  return series;
}