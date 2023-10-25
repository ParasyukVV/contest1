#include <iostream>
#include <vector>
#include <limits>

template <typename TreeType>
class ImplicitCartesianTree {
 private:
  template <typename NodeType>
  struct Node {
    Node *left_child;
    Node *right_child;

    NodeType value;
    int priority;
    int size;

    explicit Node(NodeType input_value) {
      size = 1;
      value = input_value;
      left_child = nullptr;
      right_child = nullptr;
      priority = rand();
    }
    ~Node() {
      delete this->left_child;
      delete this->right_child;
    }
  };

  Node<TreeType> *tree_;

  int GetSize(Node<TreeType> *root) {
    if (root == nullptr) {
      return 0;
    }
    return root->size;
  }

  void Update(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
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

  std::pair<Node<TreeType> *, Node<TreeType> *> Split(Node<TreeType> *root, int position_for_split) {  // В левое дерево должно уходить [0, position_for_split)
    if (root == nullptr) {
      return {nullptr, nullptr};
    }
    if (position_for_split <= GetSize(root->left_child)) {
      std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root->left_child, position_for_split);
      root->left_child = buffer.second;
      Update(root);
      return std::pair<Node<TreeType> *, Node<TreeType> *>(buffer.first, root);
    }
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root->right_child, position_for_split - GetSize(root->left_child) - 1);
    root->right_child = buffer.first;
    Update(root);
    return std::pair<Node<TreeType> *, Node<TreeType> *>(root, buffer.second);
  }

  void Operation(Node<TreeType> *root, int left_position, int right_position) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, left_position);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer = Split(left_buffer.second, right_position - left_position + 1);
    root = Merge(right_buffer.first, Merge(left_buffer.first, right_buffer.second));
  }

  Node<TreeType> *Insert(Node<TreeType> *root, int position, TreeType input_value) {
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root, position);
    return Merge(buffer.first, Merge(new Node<TreeType>(input_value), buffer.second));
  }

  Node<TreeType> *Build(const std::vector<TreeType> &input_array) {
    Node<TreeType> *answer = nullptr;
    int size = static_cast<int>(input_array.size());
    for (int i = 0; i < size; ++i) {
      answer = Insert(answer, i, input_array[i]);
    }
    return answer;
  }

 public:
  explicit ImplicitCartesianTree() {
    tree_ = nullptr;
  }

  explicit ImplicitCartesianTree(const std::vector<TreeType> &array) {
    tree_ = Build(array);
  }

  void Operation(int left_position, int right_position) {
    return Operation(tree_, left_position, right_position);
  }

  void DestroyPrint() {  // Можно было бы и за линию
    while (tree_) {
      std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(tree_, 1);
      std::cout << buffer.first->value << ' ';
      tree_ = buffer.second;
      delete buffer.first;
    }
    std::cout << '\n';
  }
};

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;
  std::vector<int> array;
  for (int i = 1; i <= n; ++i) {
    array.push_back(i);
  }
  ImplicitCartesianTree<int> treap(array);
  for (int i = 0, left = 0, right = 0; i < m; ++i) {
    std::cin >> left >> right;
    --left;
    --right;
    treap.Operation(left, right);
  }
  treap.DestroyPrint();
}
