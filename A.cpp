#include <iostream>
#include <fstream>
#include <vector>

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  int amount = 0;  // переменная для количества вводимых запросов (т.е. N и K)
  int buffer = 0;  // переменная для вводимых N чисел
  std::vector<int64_t> prefix_sum;
  prefix_sum.push_back(0);  // Будем оставаться в 1 - индексации, нолевой элемент доопределим как 0
  cin >> amount;            // N
  cin >> buffer;
  prefix_sum.push_back(buffer);
  for (int i = 1; i < amount; ++i) {
    cin >> buffer;
    prefix_sum.push_back(buffer + prefix_sum[i]);
  }
  cin >> amount;  // K
  for (int i = 0, left = 0, right = 0; i < amount; ++i) {
    cin >> left >> right;
    cout << prefix_sum[right] - prefix_sum[left - 1] << ' ';
  }
  cin.close();
  cout.close();
  return 0;
}