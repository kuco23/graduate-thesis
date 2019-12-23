#include <vector>
using std::vector;

typedef vector<int> color;

namespace colors {
  vector<color> make_gradient(vector<color> base_colors, int length);
}