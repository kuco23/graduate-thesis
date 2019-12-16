#include <vector>
using std::vector;

typedef vector<vector<int>> color_series;

namespace color_picker {
  color_series gradient(int r, int g, int b, int length);
}