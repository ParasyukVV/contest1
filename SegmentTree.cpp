#include <iostream>
#include <limits>
#include <vector>

int neutral_element = std::numeric_limits<int>::max();
int size_;

std::vector<int> tree_; // 0 - индексацию хотим

int Parent(int position) {
  return (position - 1) / 2;
}

int LeftChild(int position) {
  return 2 * position + 1;
}

int RightChild(int position) {
  return 2 * position + 2;
}

void Build(const std::vector<int> &input_array) { // Верим что массив 2^k
  size_ = static_cast<int>(input_array.size());
  tree_.resize(2 * size_ - 1, neutral_element);
  for(int i = size_ - 1; i < 2 * size_ - 1; ++i) {
    tree_[i] = input_array[i - size_ + 1];
  }
  for(int i = size_ - 2; i >= 0; --i) {
    tree_[i] = std::min(tree_[LeftChild(i)], tree_[RightChild(i)]);
  }
}

void Update(int position, int value) {
  position += size_ - 1;
  tree_[position] = value;
  while(position != 0) {
    position = Parent(position);
    tree_[position] = std::min(tree_[LeftChild(position)], tree_[RightChild(position)]);
  }
}

bool IsLeftChild (int position) {
  return position % 2 == 1;
}

bool IsRightChild (int position) {
  return position % 2 == 0;
}

int Query(int left, int right) {
  left += size_ - 1;
  right += size_ - 1;
  int answer = neutral_element;
  while (left < right) {
    if(IsLeftChild(right)) {
      answer = std::min(answer, tree_[right]);
    }
    if(IsRightChild(left)) {
      answer = std::min(answer, tree_[left]);
    }
    left = Parent(left + 1);
    right = Parent(right - 1);
  }
  if(left == right) {
    answer = std::min(answer, tree_[left]);
  }
  return answer;
}

int main() {
  int n;
  std::cin >> n; 
  std::vector<int> input_array;
  for(int i = 0, input = 0; i < n; ++i) {
    std::cin >> input;
    input_array.push_back(input);
  }
  Build(input_array);
  //Update(0, 5);
  //Update(3, 0);
  for(int i = 0; i < 2 * size_ - 1; ++i) {
    std::cout << tree_[i] << ' ';
  }
  std::cout << '\n';
  std::cout << Query(1, 3) << '\n';
  return 0;
}