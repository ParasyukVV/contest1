#include <iostream>
#include <limits>
#include <vector>
#include <fstream>

template <typename Type>
class MinimumSegmentTree {
 private:
  Type neutral_element_ = std::numeric_limits<Type>::max();
  int size_x_;
  int size_y_;
  std::vector<std::vector<Type>> tree_;

  int Parent(int position) {
    return (position - 1) / 2;
  }

  int LeftChild(int position) {
    return 2 * position + 1;
  }

  int RightChild(int position) {
    return 2 * position + 2;
  }

  int UpperDegTwo(int x) {
    int answer = 1;
    while (answer < x) {
      answer *= 2;
    }
    return answer;
  }

  Type Minimum(int permanent_left_x, int permanent_right_x, int current_left_x, int current_right_x, int position_x,
               int permanent_left_y, int permanent_right_y, int current_left_y, int current_right_y, int position_y) {
    if (permanent_left_x <= current_left_x && current_right_x <= permanent_right_x) {
      if (permanent_left_y <= current_left_y && current_right_y <= permanent_right_y) {
        return tree_[position_x][position_y];
      }
      if (permanent_left_y > current_right_y || current_left_y > permanent_right_y) {
        return neutral_element_;
      }
      int middle_y = (current_left_y + current_right_y) / 2;
      Type left_minimum = Minimum(permanent_left_x, permanent_right_x, current_left_x, current_right_x, position_x,
                                  permanent_left_y, permanent_right_y, current_left_y, middle_y, LeftChild(position_y));
      Type right_minimum =
          Minimum(permanent_left_x, permanent_right_x, current_left_x, current_right_x, position_x, permanent_left_y,
                  permanent_right_y, middle_y + 1, current_right_y, RightChild(position_y));
      return std::min(left_minimum, right_minimum);
    }
    if (permanent_left_x > current_right_x || current_left_x > permanent_right_x) {
      return neutral_element_;
    }
    int middle_x = (current_left_x + current_right_x) / 2;
    Type left_minimum = Minimum(permanent_left_x, permanent_right_x, current_left_x, middle_x, LeftChild(position_x),
                                permanent_left_y, permanent_right_y, current_left_y, current_right_y, position_y);
    Type right_minimum =
        Minimum(permanent_left_x, permanent_right_x, middle_x + 1, current_right_x, RightChild(position_x),
                permanent_left_y, permanent_right_y, current_left_y, current_right_y, position_y);
    return std::min(left_minimum, right_minimum);
  }

 public:
  explicit MinimumSegmentTree(const std::vector<std::vector<Type>> &input_array) {
    int real_size_x = static_cast<int>(input_array.size());
    size_x_ = UpperDegTwo(real_size_x);
    int real_size_y = static_cast<int>(input_array[0].size());
    size_y_ = UpperDegTwo(real_size_y);

    tree_.resize(2 * size_x_ - 1, std::vector<Type>(2 * size_y_ - 1, neutral_element_));
    for (int i = size_x_ - 1; i < real_size_x + size_x_ - 1; ++i) {
      for (int j = size_y_ - 1; j < real_size_y + size_y_ - 1; ++j) {
        tree_[i][j] = input_array[i - size_x_ + 1][j - size_y_ + 1];
      }
      for (int j = size_y_ - 2; j >= 0; --j) {
        tree_[i][j] = std::min(tree_[i][LeftChild(j)], tree_[i][RightChild(j)]);
      }
    }
    for (int j = 0; j < 2 * size_y_ - 1; ++j) {
      for (int i = size_x_ - 2; i >= 0; --i) {
        tree_[i][j] = std::min(tree_[LeftChild(i)][j], tree_[RightChild(i)][j]);
      }
    }
  }

  Type Minimum(int left_x, int left_y, int right_x, int right_y) {
    left_x += size_x_ - 2;
    right_x += size_x_ - 2;
    left_y += size_y_ - 2;
    right_y += size_y_ - 2;
    return Minimum(left_x, right_x, size_x_ - 1, 2 * size_x_ - 2, 0, left_y, right_y, size_y_ - 1, 2 * size_y_ - 2, 0);
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  int m = 0;
  int n = 0;
  cin >> m >> n;
  std::vector<std::vector<int64_t>> input_array(m, std::vector<int64_t>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> input_array[i][j];
    }
  }
  MinimumSegmentTree<int64_t> seg_tree(input_array);
  int request_amount = 0;
  cin >> request_amount;
  for (int i = 0, left_x = 0, right_x = 0, left_y = 0, right_y = 0; i < request_amount; ++i) {
    cin >> left_x >> left_y >> right_x >> right_y;
    cout << seg_tree.Minimum(left_x, left_y, right_x, right_y) << '\n';
  }
  cin.close();
  cout.close();
  return 0;
}