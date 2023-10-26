#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class SegmentTree {
 private:
  struct Node {
    int open_brackets;
    int close_brackets;
    int sequence_length;

    explicit Node(int open_input, int close_input, int sequence_input) {
      open_brackets = open_input;
      close_brackets = close_input;
      sequence_length = sequence_input;
    }
    Node operator+(const Node &x) {
      Node answer(0, 0, 0);
      int length = std::min(x.close_brackets, this->open_brackets);
      answer.sequence_length = x.sequence_length + this->sequence_length + 2 * length;
      answer.open_brackets = x.open_brackets + this->open_brackets - length;
      answer.close_brackets = x.close_brackets + this->close_brackets - length;
      return answer;
    }
  };

  int size_;
  std::vector<Node> tree_;  // 0 - индексацию хотим

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

  Node Query(int permanent_left, int permanent_right, int current_left, int current_right, int position) {
    if (permanent_left <= current_left && current_right <= permanent_right) {
      return tree_[position];
    }
    if (permanent_left > current_right || current_left > permanent_right) {
      return Node(0, 0, 0);
    }
    return Query(permanent_left, permanent_right, current_left, (current_left + current_right) / 2,
                 LeftChild(position)) +
           Query(permanent_left, permanent_right, (current_left + current_right) / 2 + 1, current_right,
                 RightChild(position));
  }

 public:
  explicit SegmentTree(const std::string &input_string) {
    int real_size = static_cast<int>(input_string.size());  // Размер копируемого массива
    size_ = UpperDegTwo(real_size);                         // Теперь считаем размер 2^k
    tree_.resize(2 * size_ - 1, Node(0, 0, 0));
    for (int i = size_ - 1; i < real_size + size_ - 1; ++i) {
      if (input_string[i - size_ + 1] == '(') {
        tree_[i].open_brackets = 1;
      }
      if (input_string[i - size_ + 1] == ')') {
        tree_[i].close_brackets = 1;
      }
    }
    for (int i = size_ - 2; i >= 0; --i) {
      tree_[i] = tree_[LeftChild(i)] + tree_[RightChild(i)];
    }
  }

  int Query(int left, int right) {  // left и right в 1-номерации
    left += size_ - 2;
    right += size_ - 2;
    return Query(left, right, size_ - 1, 2 * size_ - 2, 0).sequence_length;
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");

  std::string s;
  cin >> s;
  SegmentTree seg_tree(s);
  int m = 0;
  cin >> m;
  for (int i = 0, left = 0, right = 0; i < m; ++i) {
    cin >> left >> right;
    cout << seg_tree.Query(left, right) << '\n';
  }

  cin.close();
  cout.close();
  return 0;
}