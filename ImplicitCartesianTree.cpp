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
      //std::cout << "приоритет " << priority << '\n';
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

  /*bool CompareKey(Node<TreeType> *root, TreeType input_key) {
    if (root == nullptr) {
      return false;
    }
    return root->key == input_key;
  }*/

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
    /*if(position_for_split < GetSize(root->left_child)) {
      std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(root->left_child, position_for_split);
      root->left_child = buffer.second;
      Update(root);
      return std::pair<Node<TreeType> *, Node<TreeType> *>(buffer.first, root);
    }*/
  }

  /*bool Exists(Node<TreeType> *root, TreeType key_input) {
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
  }*/

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

  /*Node<TreeType> *Build(const std::vector<TreeType> &input_array) {
    Node<TreeType> *answer = nullptr;
    for (int i = 0; i < input_array.size(); ++i) {
      answer = Insert(answer, input_array[i]);
    }
    return answer;
  }*/

  /*std::pair<bool, TreeType> OrderStatistics(Node<TreeType> *root, int number) {  // 0 - индексация
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
  }*/

  /*std::pair<bool, TreeType> Maximum(Node<TreeType> *root) {
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
  }*/

  TreeType Minimum(Node<TreeType> *root, int left_position, int right_position) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, left_position);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer = Split(left_buffer.second, right_position - left_position + 1);
    std::cout << GetSize(left_buffer.first) << ' ' << GetSize(right_buffer.first) << ' ' << GetSize(right_buffer.second) << '\n';
    TreeType answer = GetMinimum(right_buffer.first);
    root = Merge(left_buffer.first, Merge(right_buffer.first, right_buffer.second));
    return answer;
  }

 public:
  explicit ImplicitCartesianTree() {
    tree_ = nullptr;
  }

  /*explicit ImplicitCartesianTree(const std::vector<TreeType> &array) {
    tree_ = Build(array);
  }*/

  void Insert(int position, TreeType key_input) {
    tree_ = Insert(tree_, position, key_input);
  }

  void Erase(TreeType key_input) {
    tree_ = Erase(tree_, key_input);
  }

  /*bool Exists(TreeType key_input) {
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
  }*/

  TreeType Minimum(int left_position, int right_position) {
    return Minimum(tree_, left_position, right_position);
  }

  int GetSize() {
    return GetSize(tree_);
  }
};

int main() {
  ImplicitCartesianTree<int64_t> treap;
  treap.Insert(0, 6);
  treap.Insert(0, 5);
  treap.Insert(0, 4);
  treap.Insert(0, 1);
  treap.Insert(0, 3);
  treap.Insert(0, 7);
  std::cout << treap.Minimum(0, 0) << '\n';
  std::cout << treap.Minimum(1, 1) << '\n';
  //std::cout << treap.Minimum(2, 2) << '\n';
  //std::cout << treap.Minimum(3, 3) << '\n';
  //std::cout << treap.Minimum(4, 4) << '\n';
  //std::cout << treap.Minimum(5, 5) << '\n';
  //std::cout << treap.GetSize() << '\n';
}
