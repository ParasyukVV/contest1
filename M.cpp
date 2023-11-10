#include <iostream>
#include <vector>

void ValueModificatorAdd(unsigned int left, unsigned int right, unsigned int add,
                         std::vector<unsigned int> &value_modificator) {
  value_modificator[left] += static_cast<unsigned int>(add);
  value_modificator[right + 1] -= static_cast<unsigned int>(add);
}

void BuildPrefixSum(std::vector<unsigned int> &value_modificator, std::vector<unsigned int> &prefix_sum) {
  int size = static_cast<int>(value_modificator.size()) - 1;
  int64_t current_value = 0;
  for (int i = 1; i < size; ++i) {
    current_value += value_modificator[i];
    prefix_sum[i] = prefix_sum[i - 1] + current_value;
  }
}

unsigned int Sum(int left, int right, std::vector<unsigned int> &prefix_sum) {
  return prefix_sum[right] - prefix_sum[left - 1];
}

unsigned int cur = 0;

unsigned int NextRand(unsigned int a, unsigned int b) {
  cur = cur * a + b;  // вычисляется с переполнениями
  return cur >> 8;    // число от 0 до 2^24 - 1
}

int main() {
  int n = 16777216;
  int m = 0;
  int q = 0;
  unsigned int a = 0;
  unsigned int b = 0;
  std::cin >> m >> q >> a >> b;
  std::vector<unsigned int> prefix_sum(n + 2, 0);
  for (int i = 0; i < m; ++i) {
    int add = static_cast<int>(NextRand(a, b));
    int left = static_cast<int>(NextRand(a, b));
    int right = static_cast<int>(NextRand(a, b));
    if (left > right) {
      std::swap(left, right);
    }
    ++left;
    ++right;
    ValueModificatorAdd(left, right, add, prefix_sum);
  }
  BuildPrefixSum(prefix_sum, prefix_sum);

  unsigned int sum = 0;
  for (int i = 0; i < q; ++i) {
    int left = static_cast<int>(NextRand(a, b));
    int right = static_cast<int>(NextRand(a, b));
    if (left > right) {
      std::swap(left, right);
    }
    ++left;
    ++right;
    sum += Sum(left, right, prefix_sum);  // По модулю 2^32
  }
  std::cout << sum << '\n';
}
