#include <iostream>
#include <limits>
#include <vector>

template <typename Type>
class MinimumSegmentTree {
 private:
  Type neutral_element_ = std::numeric_limits<Type>::max();
  int size_;
  std::vector<Type> tree_;  // 0 - индексацию хотим

  int Parent(int position) {
    return (position - 1) / 2;
  }

  int LeftChild(int position) {
    return 2 * position + 1;
  }

  int RightChild(int position) {
    return 2 * position + 2;
  }

  bool IsLeftChild(int position) {
    return position % 2 == 1;
  }

  bool IsRightChild(int position) {
    return position % 2 == 0;
  }

  int UpperDegTwo(int x) {
    int answer = 1;
    while (answer < x) {
      answer *= 2;
    }
    return answer;
  }

public:
  explicit MinimumSegmentTree(const std::vector<int> &input_array) {
    int real_size = static_cast<int>(input_array.size());  // Размер копируемого массива
    size_ = UpperDegTwo(real_size);                        // Теперь считаем размер 2^k
    tree_.resize(2 * size_ - 1, neutral_element_);
    for (int i = size_ - 1; i < real_size + size_ - 1; ++i) {
      tree_[i] = input_array[i - size_ + 1];
    }
    for (int i = size_ - 2; i >= 0; --i) {
      tree_[i] = std::min(tree_[LeftChild(i)], tree_[RightChild(i)]);
    }
  }

  explicit MinimumSegmentTree(const std::vector<Type> &input_array) {
    int real_size = static_cast<int>(input_array.size());  // Размер копируемого массива
    size_ = UpperDegTwo(real_size);                        // Теперь считаем размер 2^k
    tree_.resize(2 * size_ - 1, neutral_element_);
    for (int i = size_ - 1; i < real_size + size_ - 1; ++i) {
      tree_[i] = input_array[i - size_ + 1];
    }
    for (int i = size_ - 2; i >= 0; --i) {
      tree_[i] = std::min(tree_[LeftChild(i)], tree_[RightChild(i)]);
    }
  }

  void Update(int position, Type value) {
    position += size_ - 1;
    tree_[position] = value;
    while (position != 0) {
      position = Parent(position);
      tree_[position] = std::min(tree_[LeftChild(position)], tree_[RightChild(position)]);
    }
  }

  Type Query(int left, int right) {
    left += size_ - 1;
    right += size_ - 1;
    Type answer = neutral_element_;
    while (left < right) {
      if (IsLeftChild(right)) {
        answer = std::min(answer, tree_[right]);
      }
      if (IsRightChild(left)) {
        answer = std::min(answer, tree_[left]);
      }
      left = Parent(left + 1);
      right = Parent(right - 1);
    }
    if (left == right) {
      answer = std::min(answer, tree_[left]);
    }
    return answer;
  }
};

int main() {
  int n;
  std::cin >> n;
  std::vector<int> input_array;
  for (int i = 0, input = 0; i < n; ++i) {
    std::cin >> input;
    input_array.push_back(input);
  }
  MinimumSegmentTree<int64_t> seg_tree(input_array);
  std::cout << seg_tree.Query(2, 4) << '\n';
  return 0;
}