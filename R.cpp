#include <iostream>
#include <vector>
#include <limits>
#include <fstream>

template <typename TreeType>
class ImplicitCartesianTree {
 private:
  template <typename NodeType>
  struct Node {
    Node *left_child;
    Node *right_child;

    NodeType value;
    NodeType minimum;
    int priority;
    int size;

    explicit Node(NodeType input_value) {
      size = 1;
      value = input_value;
      minimum = input_value;
      left_child = nullptr;
      right_child = nullptr;
      priority = rand();
    }
  };

  Node<TreeType> *tree_;

  int GetSize(Node<TreeType> *root) {
    if (root == nullptr) {
      return 0;
    }
    return root->size;
  }

  TreeType GetMinimum(Node<TreeType> *root) {
    if (root == nullptr) {
      return std::numeric_limits<TreeType>::max();
    }
    return root->minimum;
  }

  void Update(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
    root->minimum = std::min(GetMinimum(root->left_child), GetMinimum(root->right_child));
    root->minimum = std::min(root->minimum, root->value);
  }

  Node<TreeType> *Merge(Node<TreeType> *tree_1, Node<TreeType> *tree_2) {
    if (tree_1 == nullptr) {
      return tree_2;
    }
    if (tree_2 == nullptr) {
      return tree_1;
    }
    if (tree_1->priority < tree_2->priority) {
      tree_1->right_child = Merge(tree_1->right_child, tree_2);
      Update(tree_1);
      return tree_1;
    }
    tree_2->left_child = Merge(tree_1, tree_2->left_child);
    Update(tree_2);
    return tree_2;
  }

  std::pair<Node<TreeType> *, Node<TreeType> *> Split(
      Node<TreeType> *root, int position_for_split) {  // В левое дерево должно уходить [0, position_for_split)
    if (root == nullptr) {
      return {nullptr, nullptr};
    }
    if (position_for_split <= GetSize(root->left_child)) {
      std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root->left_child, position_for_split);
      root->left_child = buffer.second;
      Update(root);
      return std::pair<Node<TreeType> *, Node<TreeType> *>(buffer.first, root);
    }
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer =
        Split(root->right_child, position_for_split - GetSize(root->left_child) - 1);
    root->right_child = buffer.first;
    Update(root);
    return std::pair<Node<TreeType> *, Node<TreeType> *>(root, buffer.second);
  }

  Node<TreeType> *Insert(Node<TreeType> *root, int position, TreeType input_value) {
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root, position);
    return Merge(buffer.first, Merge(new Node<TreeType>(input_value), buffer.second));
  }

  Node<TreeType> *Erase(Node<TreeType> *root, int position) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, position);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer = Split(left_buffer.second, position + 1);
    delete right_buffer.first;
    return Merge(left_buffer.first, right_buffer.second);
  }

  TreeType Minimum(Node<TreeType> *root, int left_position, int right_position) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, left_position);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer =
        Split(left_buffer.second, right_position - left_position + 1);
    TreeType answer = GetMinimum(right_buffer.first);
    root = Merge(left_buffer.first, Merge(right_buffer.first, right_buffer.second));
    return answer;
  }

 public:
  explicit ImplicitCartesianTree() {
    tree_ = nullptr;
  }

  void Insert(int position, TreeType key_input) {
    tree_ = Insert(tree_, position, key_input);
  }

  void Erase(TreeType key_input) {
    tree_ = Erase(tree_, key_input);
  }

  TreeType Minimum(int left_position, int right_position) {
    return Minimum(tree_, left_position, right_position);
  }

  int GetSize() {
    return GetSize(tree_);
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  ImplicitCartesianTree<int> treap;
  int amount = 0;
  cin >> amount;
  char request_type = 0;
  for (int i = 0, arg1 = 0, arg2 = 0; i < amount; ++i) {
    cin >> request_type >> arg1 >> arg2;
    if (request_type == '+') {
      treap.Insert(arg1, arg2);
    }
    if (request_type == '?') {
      cout << treap.Minimum(arg1 - 1, arg2 - 1) << '\n';
    }
  }
  cin.close();
  cout.close();
}
