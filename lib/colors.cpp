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
    direction.push_back(1 / (double) length * (double) (u[i] + v[i]));
  return direction;
}

color_series color_path(
  const vector<int> &rgb1, 
  const vector<int> &rgb2, 
  int npoints
) {
  color_series series {};
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

color_series colors::make_gradient(color_series base_colors, int length) {
  int base_length = base_colors.size();
  int margin = std::floor((double) length / (base_length - 1));
  int leftover = length % margin;
  color_series series {};
  for (int i = 1; i < base_length; i++) {
    color_series temp = color_path(base_colors[i-1], base_colors[i], margin);
    series.insert(series.end(), temp.begin(), temp.end());
    if (leftover > 0) {
      series.push_back(base_colors[i]);
      leftover--;
    }
  }
  return series;
}