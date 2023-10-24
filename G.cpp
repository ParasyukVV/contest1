#include <iostream>
#include <limits>
#include <vector>
#include <fstream>

template <typename Type>
class MaximumSegmentTree {
 private:
  template <typename NodeType>
  struct Node {
    NodeType maximum;
    int number_of_max_elements;

    explicit Node(NodeType value_input, int num = 1) {
      maximum = value_input;
      number_of_max_elements = num;
    }
    Node operator+(const Node &x) {
      if (this->maximum == x.maximum) {
        return Node(this->maximum, this->number_of_max_elements + x.number_of_max_elements);
      }
      if (this->maximum < x.maximum) {
        return Node(x.maximum, x.number_of_max_elements);
      }
      return Node(this->maximum, this->number_of_max_elements);
    }
  };

  Type neutral_element_ = std::numeric_limits<Type>::min();
  int size_;
  std::vector<Node<Type> > tree_;  // 0 - индексацию хотим

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

  Node<Type> Maximum(int permanent_left, int permanent_right, int current_left, int current_right, int position) {
    if (permanent_left <= current_left && current_right <= permanent_right) {
      return tree_[position];
    }
    if (permanent_left > current_right || current_left > permanent_right) {
      return Node<Type>(neutral_element_);
    }
    return Maximum(permanent_left, permanent_right, current_left, (current_left + current_right) / 2,
                   LeftChild(position)) +
           Maximum(permanent_left, permanent_right, (current_left + current_right) / 2 + 1, current_right,
                   RightChild(position));
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

  std::pair<Type, int> Maximum(int left, int right) {  // left и right в 1-номерации
    left += size_ - 2;
    right += size_ - 2;
    Node<Type> answer = Maximum(left, right, size_ - 1, 2 * size_ - 2, 0);
    return {answer.maximum, answer.number_of_max_elements};
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  int amount = 0;
  cin >> amount;
  std::vector<int> input_array;
  for (int i = 0, buffer = 0; i < amount; ++i) {
    cin >> buffer;
    input_array.push_back(buffer);
  }
  MaximumSegmentTree<int> seg_tree(input_array);
  cin >> amount;
  for (int i = 0, left = 0, right = 9; i < amount; ++i) {
    cin >> left >> right;
    std::pair<int, int> answer = seg_tree.Maximum(left, right);
    cout << answer.first << ' ' << answer.second << '\n';
  }
  cin.close();
  cout.close();
  return 0;
}