#include <iostream>
#include <fstream>
#include <limits>

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  int n = 0;
  cin >> n;
  int64_t current_sum = 0;
  int negative_prefix = 0;  // Поддерживаем тут вершину, сумма на (рассмотренном) префиксе до которой отрицательна
  int max_negative_value = std::numeric_limits<int>::min();  // Максимальное отрицательное значение
  int max_negative_position = 0;  // Позиция максимального отрицательного значения
  int64_t answer_sum = 0;
  int answer_left = 0;
  int answer_right = 0;
  for (int i = 1, input_value = 0; i <= n; ++i) {
    cin >> input_value;
    if (input_value > max_negative_value) {
      max_negative_value = input_value;
      max_negative_position = i;
    }
    current_sum += input_value;
    if (current_sum < 0) {
      negative_prefix = i;  // Префикс до i точно не входит в искомый отрезок
      current_sum = 0;
    } else if (current_sum > answer_sum) {
      answer_left = negative_prefix + 1;
      answer_right = i;
      answer_sum = current_sum;
    }
  }
  if (answer_left == 0) {  // Случай всех отрицательных значений
    answer_left = max_negative_position;
    answer_right = max_negative_position;
    answer_sum = max_negative_value;
  }
  cout << answer_left << ' ' << answer_right << ' ' << answer_sum;
  cin.close();
  cout.close();
  return 0;
}