#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>

template <typename Type>
class MaximumSegmentTree {
 private:
  template <typename NodeType>
  struct Node {
    NodeType maximum;
    NodeType promise;

    explicit Node(NodeType value_input) {
      maximum = value_input;
      promise = 0;
    }
    Node operator+(const Node &x) {
      return Node(std::max(this->maximum, x.maximum));
    }
  };

  Type neutral_element_ = std::numeric_limits<Type>::min();
  int size_;
  std::vector<Node<Type> > tree_;

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

  bool IsLeaf(int position) {
    return position >= size_ - 1;
  }

  Type GetMaximum(int position) {
    if (position > 2 * size_ - 2) {
      return neutral_element_;
    }
    return tree_[position].maximum + tree_[position].promise;
  }

  void UpdatePromise(int position, Type new_promise) {
    tree_[position].promise += new_promise;
  }

  void UpdateMaximum(int position) {
    tree_[position].maximum += tree_[position].promise;
  }

  void NullifyPromise(int position) {
    tree_[position].promise = 0;
  }

  void Push(int position) {
    UpdateMaximum(position);
    if (!IsLeaf(position)) {
      UpdatePromise(LeftChild(position), tree_[position].promise);
      UpdatePromise(RightChild(position), tree_[position].promise);
    }
    NullifyPromise(position);
  }

  Type Maximum(int permanent_left, int permanent_right, int current_left, int current_right, int position) {
    Push(position);
    if (permanent_left <= current_left && current_right <= permanent_right) {
      return tree_[position].maximum;
    }
    if (permanent_left > current_right || current_left > permanent_right) {
      return neutral_element_;
    }
    int middle = (current_left + current_right) / 2;
    Type left_max = Maximum(permanent_left, permanent_right, current_left, middle, LeftChild(position));
    Type right_max = Maximum(permanent_left, permanent_right, middle + 1, current_right, RightChild(position));
    return std::max(left_max, right_max);
  }

  void Update(int permanent_left, int permanent_right, int current_left, int current_right, int position,
              Type promise_input) {
    if (permanent_left <= current_left && current_right <= permanent_right) {
      UpdatePromise(position, promise_input);
      return;
    }
    if (permanent_left > current_right || current_left > permanent_right) {
      return;
    }
    Push(position);
    int middle = (current_left + current_right) / 2;
    Update(permanent_left, permanent_right, current_left, middle, LeftChild(position), promise_input);
    Update(permanent_left, permanent_right, middle + 1, current_right, RightChild(position), promise_input);
    tree_[position].maximum = std::max(GetMaximum(LeftChild(position)), GetMaximum(RightChild(position)));
  }

 public:
  explicit MaximumSegmentTree(const std::vector<Type> &input_array) {
    int real_size = static_cast<int>(input_array.size());  // Размер копируемого массива
    size_ = UpperDegTwo(real_size);                        // Теперь считаем размер 2^k
    tree_.resize(2 * size_ - 1, Node<Type>(neutral_element_));
    for (int i = size_ - 1; i < real_size + size_ - 1; ++i) {
      tree_[i] = Node<Type>(input_array[i - size_ + 1]);
    }
    for (int i = size_ - 2; i >= 0; --i) {
      tree_[i] = tree_[LeftChild(i)] + tree_[RightChild(i)];
    }
  }

  Type Maximum(int left, int right) {  // left и right в 1-номерации
    left += size_ - 2;
    right += size_ - 2;
    return Maximum(left, right, size_ - 1, 2 * size_ - 2, 0);
  }

  void Update(int left, int right, Type value) {  // left и right в 1-номерации
    left += size_ - 2;
    right += size_ - 2;
    Update(left, right, size_ - 1, 2 * size_ - 2, 0, value);
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  int amount = 0;
  int64_t buffer = 0;
  cin >> amount;
  std::vector<int64_t> input_array;
  for (int i = 0; i < amount; ++i) {
    cin >> buffer;
    input_array.push_back(buffer);
  }
  MaximumSegmentTree<int64_t> seg_tree(input_array);
  char request_type = 0;
  cin >> amount;
  for (int i = 0, arg1 = 0, arg2 = 0; i < amount; ++i) {
    cin >> request_type;
    if (request_type == 'm') {
      cin >> arg1 >> arg2;
      cout << seg_tree.Maximum(arg1, arg2) << ' ';
    }
    if (request_type == 'a') {
      cin >> arg1 >> arg2 >> buffer;
      seg_tree.Update(arg1, arg2, buffer);
    }
  }
  cin.close();
  cout.close();
}