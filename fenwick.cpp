#include <iostream>
#include <vector>

/*std::vector<int> array;
std::vector<int> fenwick_tree;

void update(int position, int value) {
  for(; position < fenwick_tree.size(); position |= position + 1) {
    fenwick_tree[position] += value;
  }
}

void build() {
  fenwick_tree.resize(array.size(), 0);
  for(int i = 0; i < array.size(); ++i) {
    update(i, array[i]);
  }
}

int prefix_sum(int right) {
  int answer = 0;
  for(; right >= 0; right = (right & (right + 1)) - 1) {
    answer += fenwick_tree[right];
  }
  return answer;
}

int sum(int left, int right) {
  return prefix_sum(right) - prefix_sum(left - 1);
}
*/

class fenwick_tree {
private:
  std::vector<int> tree;

  int prefix_sum(int right) {
    int answer = 0;
    for (; right >= 0; right = (right & (right + 1)) - 1) {
      answer += tree[right];
    }
    return answer;
  }

public:
  void update(int position, int value) {
    for (; position < tree.size(); position |= position + 1) {
      tree[position] += value;
    }
  }

  fenwick_tree(const std::vector<int> &array) {
    tree.resize(array.size(), 0);
    for (int i = 0; i < array.size(); ++i) {
      update(i, array[i]);
    }
  }

  int sum(int left, int right) {
    return prefix_sum(right) - prefix_sum(left - 1);
  }
};

int main()
{
  std::vector<int> input_array;
  int n = 0;
  std::cin >> n;
  for (int i = 0, input = 0; i < n; ++i)
  {
    std::cin >> input;
    input_array.push_back(input);
  }
  fenwick_tree auf(input_array);
  std::cout << auf.sum(2, 3) << '\n';
}