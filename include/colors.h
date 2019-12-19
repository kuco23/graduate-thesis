#include <vector>
using std::vector;

typedef vector<vector<int>> color_series;

namespace colors {
  color_series make_gradient(color_series base_colors, int length);
}