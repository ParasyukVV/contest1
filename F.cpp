#include <iostream>
#include <vector>
#include <fstream>

template <typename Type>
class FenwickTree {
 private:
  std::vector<std::vector<std::vector<Type>>> tree_;
  int size_x_;
  int size_y_;
  int size_z_;

  int Sum(int x, int y, int z) {
    Type answer = 0;
    for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
      for (int j = y; j >= 0; j = (j & (j + 1)) - 1) {
        for (int k = z; k >= 0; k = (k & (k + 1)) - 1) {
          answer += tree_[i][j][k];
        }
      }
    }
    return answer;
  }

 public:
  void IncreaseByValue(int x, int y, int z, Type value) {
    for (int i = x; i < size_x_; i |= i + 1) {
      for (int j = y; j < size_y_; j |= j + 1) {
        for (int k = z; k < size_z_; k |= k + 1) {
          tree_[i][j][k] += value;
        }
      }
    }
  }

  explicit FenwickTree(int n) {
    size_x_ = size_y_ = size_z_ = n;
    tree_ = std::vector<std::vector<std::vector<Type>>>(n, std::vector<std::vector<Type>>(n, std::vector<Type>(n, 0)));
  }

  int Sum(int x1, int y1, int z1, int x2, int y2, int z2) {
    return Sum(x2, y2, z2) - Sum(x1 - 1, y2, z2) - Sum(x2, y1 - 1, z2) + Sum(x1 - 1, y1 - 1, z2) - Sum(x2, y2, z1 - 1) +
           Sum(x1 - 1, y2, z1 - 1) + Sum(x2, y1 - 1, z1 - 1) - Sum(x1 - 1, y1 - 1, z1 - 1);
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  int n = 0;
  cin >> n;
  FenwickTree<int> fen(n);
  int request_type = 0;
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;
  int z1 = 0;
  int z2 = 0;
  while (request_type != 3) {
    cin >> request_type;
    if (request_type == 1) {
      cin >> x1 >> y1 >> z1 >> x2;
      fen.IncreaseByValue(x1, y1, z1, x2);
    }
    if (request_type == 2) {
      cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
      cout << fen.Sum(x1, y1, z1, x2, y2, z2) << '\n';
    }
  }
  cin.close();
  cout.close();
}