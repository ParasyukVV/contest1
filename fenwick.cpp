#include <iostream>
#include <vector>
#include <fstream>

template <typename Type>
class FenwickTree {
 private:
  std::vector<Type> tree_;
  int size_;

  int Sum(int right) {
    Type answer = 0;
    for (; right >= 0; right = (right & (right + 1)) - 1) {
      answer += tree_[right];
    }
    return answer;
  }

 public:
  void IncreaseByValue(int position, Type value) {
    for (; position < size_; position |= position + 1) {
      tree_[position] += value;
    }
  }

  void ChangeValue(int position, Type value) {
    IncreaseByValue(position, value - Sum(position, position));
  }

  explicit FenwickTree(const std::vector<Type> &array) {
    size_ = static_cast<int>(array.size());
    std::vector<Type> prefix_sum;
    prefix_sum.push_back(0);
    for (int i = 1; i < size_; ++i) {
      prefix_sum.push_back(array[i] + prefix_sum[i - 1]);
    }
    for (int i = 0; i < size_; ++i) {
      tree_.push_back(prefix_sum[i] - prefix_sum[(i & (i + 1)) - 1]);
    }
  }

  explicit FenwickTree(const std::vector<int> &array) {
    size_ = static_cast<int>(array.size());
    std::vector<Type> prefix_sum;
    prefix_sum.push_back(0);
    for (int i = 1; i < size_; ++i) {
      prefix_sum.push_back(array[i] + prefix_sum[i - 1]);
    }
    for (int i = 0; i < size_; ++i) {
      tree_.push_back(prefix_sum[i] - prefix_sum[(i & (i + 1)) - 1]);
    }
  }

  int Sum(int left, int right) {
    return Sum(right) - Sum(left - 1);
  }
};

int main()
{
  std::vector<int> input_array;
  input_array.push_back(0);
  int n = 0;
  std::cin >> n;
  for (int i = 0, input = 0; i < n; ++i)
  {
    std::cin >> input;
    input_array.push_back(input);
  }
  fenwick_tree<int64_t> auf(input_array);
  auf.change_value(2, 10);
  std::cout << auf.sum(1, 5) << '\n';

}