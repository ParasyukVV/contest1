#include <iostream>
#include <vector>

template <typename TreeType>
class ExplicitCartesianTree {
 private:
  template <typename NodeType>
  struct Node {
    Node *left_child;
    Node *right_child;

    NodeType key;
    NodeType sum;
    int priority;
    int size;

    explicit Node(NodeType key_input) {
      size = 1;
      key = key_input;
      sum = key_input;
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

  TreeType GetSum(Node<TreeType> *root) {
    if (root == nullptr) {
      return 0;
    }
    return root->sum;
  }

  bool CompareKey(Node<TreeType> *root, TreeType input_key) {
    if (root == nullptr) {
      return false;
    }
    return root->key == input_key;
  }

  void Update(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
    root->sum = GetSum(root->left_child) + root->key + GetSum(root->right_child);
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

  std::pair<Node<TreeType> *, Node<TreeType> *> Split(Node<TreeType> *root, TreeType key_for_split) {
    if (root == nullptr) {
      return {nullptr, nullptr};
    }
    if (root->key < key_for_split) {
      std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root->right_child, key_for_split);
      root->right_child = buffer.first;
      Update(root);
      return std::pair<Node<TreeType> *, Node<TreeType> *>(root, buffer.second);
    }
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root->left_child, key_for_split);
    root->left_child = buffer.second;
    Update(root);
    return std::pair<Node<TreeType> *, Node<TreeType> *>(buffer.first, root);
  }

  Node<TreeType> *Insert(Node<TreeType> *root, TreeType key_input) {
    Node<TreeType> *inserting_node = nullptr;
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root, key_input);
    if (!CompareKey(root, key_input)) {
      inserting_node = new Node<TreeType>(key_input);
    }
    return Merge(buffer.first, Merge(inserting_node, buffer.second));
  }

  Node<TreeType> *Erase(Node<TreeType> *root, TreeType key_input) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, key_input);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer = Split(left_buffer.second, key_input + 1);
    delete right_buffer.first;
    return Merge(left_buffer.first, right_buffer.second);
  }

  Node<TreeType> *Build(const std::vector<TreeType> &input_array) {
    Node<TreeType> *answer = nullptr;
    for (int i = 0; i < input_array.size(); ++i) {
      answer = Insert(answer, input_array[i]);
    }
    return answer;
  }

  bool Exists(Node<TreeType> *root, TreeType key_input) {
    if (root == nullptr) {
      return false;
    }
    if (root->key == key_input) {
      return true;
    }
    if (key_input < root->key) {
      return Exists(root->left_child, key_input);
    }
    return Exists(root->right_child, key_input);
  }

  std::pair<bool, TreeType> OrderStatistics(Node<TreeType> *root, int number) {  // 0 - индексация
    if (root == nullptr) {
      return {false, 0};
    }
    int left_size = GetSize(root->left_child);
    if (left_size == number) {
      return {true, root->key};
    }
    if (left_size < number) {
      return OrderStatistics(root->right_child, number - left_size - 1);
    }
    return OrderStatistics(root->left_child, number);
  }

  std::pair<bool, TreeType> Maximum(Node<TreeType> *root) {
    if (root == nullptr) {
      return {false, 0};
    }
    if (root->right_child == nullptr) {
      return {true, root->key};
    }
    return Maximum(root->right_child);
  }

  std::pair<bool, TreeType> Minimum(Node<TreeType> *root) {
    if (root == nullptr) {
      return {false, 0};
    }
    if (root->left_child == nullptr) {
      return {true, root->key};
    }
    return Minimum(root->left_child);
  }

  std::pair<bool, TreeType> Previous(Node<TreeType> *root, TreeType key_input) {
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root, key_input);
    std::pair<bool, TreeType> answer = Maximum(buffer.first);
    root = Merge(buffer.first, buffer.second);
    return answer;
  }

  std::pair<bool, TreeType> Next(Node<TreeType> *root, TreeType key_input) {
    std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root, key_input + 1);
    std::pair<bool, TreeType> answer = Minimum(buffer.second);
    root = Merge(buffer.first, buffer.second);
    return answer;
  }

  TreeType Sum(Node<TreeType> *root, TreeType left_value, TreeType right_value) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, left_value);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer = Split(left_buffer.second, right_value + 1);
    TreeType answer = GetSum(right_buffer.first);
    root = Merge(left_buffer.first, Merge(right_buffer.first, right_buffer.second));
    return answer;
  }

 public:
  explicit ExplicitCartesianTree() {
    tree_ = nullptr;
  }

  explicit ExplicitCartesianTree(const std::vector<TreeType> &array) {
    tree_ = Build(array);
  }

  void Insert(TreeType key_input) {
    tree_ = Insert(tree_, key_input);
  }

  void Erase(TreeType key_input) {
    tree_ = Erase(tree_, key_input);
  }

  bool Exists(TreeType key_input) {
    return Exists(tree_, key_input);
  }

  std::pair<bool, TreeType> Previous(TreeType key_input) {
    return Previous(tree_, key_input);
  }

  std::pair<bool, TreeType> Next(TreeType key_input) {
    return Next(tree_, key_input);
  }

  std::pair<bool, TreeType> OrderStatistics(int number) {
    return OrderStatistics(tree_, number);
  }

  TreeType Sum(TreeType left_value, TreeType right_value) {
    return Sum(tree_, left_value, right_value);
  }
};

int main() {
  ExplicitCartesianTree<int> treap;
  treap.Insert(9);
  std::cout << treap.Sum(2, 8) << '\n';
}
