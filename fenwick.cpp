#include <iostream>
#include <vector>
#include <fstream>

template<typename Type>
class fenwick_tree {
private:
  std::vector<Type> tree;

  int sum(int right) {
    Type answer = 0;
    for (; right >= 0; right = (right & (right + 1)) - 1) {
      answer += tree[right];
    }
    return answer;
  }

public:
  void increase_by_value(int position, Type value) {
    for (; position < tree.size(); position |= position + 1) {
      tree[position] += value;
    }
  }

  void change_value(int position, Type value) {
    increase_by_value(position, value - sum(position, position));
  }

  fenwick_tree(const std::vector<Type> &array) {
    std::vector<Type> prefix_sum;
    prefix_sum.push_back(0);
    for(int i = 1; i < array.size(); ++i) {
      prefix_sum.push_back(array[i] + prefix_sum[i - 1]);
    }
    for(int i = 0; i < prefix_sum.size(); ++i) {
      tree.push_back(prefix_sum[i] - prefix_sum[(i & (i + 1)) - 1]);
    }
  }

  fenwick_tree(const std::vector<int> &array) {
    std::vector<Type> prefix_sum;
    prefix_sum.push_back(0);
    for(int i = 1; i < array.size(); ++i) {
      prefix_sum.push_back(array[i] + prefix_sum[i - 1]);
    }
    for(int i = 0; i < prefix_sum.size(); ++i) {
      tree.push_back(prefix_sum[i] - prefix_sum[(i & (i + 1)) - 1]);
    }
  }

  int sum(int left, int right) {
    return sum(right) - sum(left - 1);
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