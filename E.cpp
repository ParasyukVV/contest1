#include <iostream>
#include <vector>
#include <fstream>
#include <limits>

template <typename Type>
class SumSegmentTree {
 private:
  Type neutral_element_ = 0;
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
  explicit SumSegmentTree(const std::vector<Type> &input_array) {
    int real_size = static_cast<int>(input_array.size());  // Размер копируемого массива
    size_ = UpperDegTwo(real_size);                        // Теперь считаем размер 2^k
    tree_.resize(2 * size_ - 1, neutral_element_);
    for (int i = size_ - 1; i < real_size + size_ - 1; ++i) {
      tree_[i] = input_array[i - size_ + 1];
    }
    for (int i = size_ - 2; i >= 0; --i) {
      tree_[i] = tree_[LeftChild(i)] + tree_[RightChild(i)];
    }
  }

  void Update(int position, Type value) {
    position += size_ - 1;
    tree_[position] = value;
    while (position != 0) {
      position = Parent(position);
      tree_[position] = tree_[LeftChild(position)] + tree_[RightChild(position)];
    }
  }

  Type Query(int left, int right) {
    left += size_ - 1;
    right += size_ - 1;
    Type answer = neutral_element_;
    while (left < right) {
      if (IsLeftChild(right)) {
        answer += tree_[right];
      }
      if (IsRightChild(left)) {
        answer += tree_[left];
      }
      left = Parent(left + 1);
      right = Parent(right - 1);
    }
    if (left == right) {
      answer += tree_[left];
    }
    return answer;
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  int amount = 0;  // N и K
  cin >> amount;
  std::vector<int64_t> input_array;
  for (int i = 0, value = 0; i < amount; ++i) {
    cin >> value;
    input_array.push_back(value);
  }
  SumSegmentTree<int64_t> seg_tree(input_array);
  cin >> amount;
  char request_type = 0;
  for (int i = 0, left = 0, right = 0; i < amount; ++i) {
    cin >> request_type >> left >> right;
    if (request_type == 's') {
      cout << seg_tree.Query(left - 1, right - 1) << ' ';
    }
    if (request_type == 'u') {
      seg_tree.Update(left - 1, right);
    }
  }
  cin.close();
  cout.close();
}