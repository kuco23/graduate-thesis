#include "../include/color_picker.h"

color_series color_picker::gradient(int r, int g, int b, int length) {
  color_series color_list {};
  double ratio = (double) 1 / length;
  for (double i = 0; i <= length; i++)
    color_list.push_back({
      static_cast<int>(r * i * ratio), 
      static_cast<int>(g * i * ratio),
      static_cast<int>(b * i * ratio)
    });
  return color_list;
}