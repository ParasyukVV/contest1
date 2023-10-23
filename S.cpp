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
    Node *parent;

    NodeType value;
    NodeType minimum;
    int priority;
    int size;
    bool reversed;

    explicit Node(NodeType input_value) {
      size = 1;
      value = input_value;
      minimum = input_value;
      reversed = false;
      left_child = nullptr;
      right_child = nullptr;
      parent = nullptr;
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

  void SetParent(Node<TreeType> *child, Node<TreeType> *parent) {
    if (child == nullptr) {
      return;
    }
    child->parent = parent;
  }

  void ChangeReverse(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    root->reversed = !root->reversed;
  }

  void Update(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
    root->minimum = std::min(GetMinimum(root->left_child), GetMinimum(root->right_child));
    root->minimum = std::min(root->minimum, root->value);
    SetParent(root->left_child, root);
    SetParent(root->right_child, root);
  }
  void Push(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    if (root->reversed) {
      root->reversed = false;
      ChangeReverse(root->left_child);
      ChangeReverse(root->right_child);
      std::swap(root->left_child, root->right_child);
    }
  }

  Node<TreeType> *Merge(Node<TreeType> *tree_1, Node<TreeType> *tree_2) {
    Push(tree_1);
    Push(tree_2);
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

  std::pair<Node<TreeType> *, Node<TreeType> *> Split(Node<TreeType> *root, int position_for_split) {
    Push(root);
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

  void Reverse(Node<TreeType> *root, int left_position, int right_position) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, left_position);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer =
        Split(left_buffer.second, right_position - left_position + 1);
    ChangeReverse(right_buffer.first);
    root = Merge(left_buffer.first, Merge(right_buffer.first, right_buffer.second));
  }

  Node<TreeType> *Build(const std::vector<TreeType> &input_array) {
    if (input_array.size() == 0) {
      return nullptr;
    }
    Node<TreeType> *answer = nullptr;
    Node<TreeType> *key_maximum = nullptr;
    Insert(answer, Node<TreeType>(input_array[0]));
    key_maximum = answer;
  }

  Node<TreeType> *LowerPriorityAncestor(Node<TreeType> *current_node, int priority_input) {
    if (current_node == nullptr) {
      return nullptr;
    }
    if (current_node->priority < priority_input) {
      return current_node;
    }
    return LowerPriorityAncestor(current_node->parent, priority_input);
  }

  void TreeUpdate(Node<TreeType> *current_node) {
    if (current_node == nullptr) {
      return;
    }
    TreeUpdate(current_node->left_child);
    TreeUpdate(current_node->right_child);
    Update(current_node);
  }

  Node<TreeType> *SortedBuild(const std::vector<TreeType> &array) {
    if (array.empty()) {
      return nullptr;
    }
    auto *answer = new Node<TreeType>(array[0]);
    Node<TreeType> *node_with_max_key = answer;
    int size = static_cast<int>(array.size());
    for (int i = 1; i < size; ++i) {
      auto *new_node = new Node<TreeType>(array[i]);
      Node<TreeType> *parent_for_new_node = LowerPriorityAncestor(node_with_max_key, new_node->priority);
      if (parent_for_new_node == nullptr) {
        answer->parent = new_node;
        new_node->left_child = answer;
        answer = new_node;
      } else {
        new_node->parent = parent_for_new_node;
        new_node->left_child = parent_for_new_node->right_child;
        if (new_node->left_child != nullptr) {
          new_node->left_child->parent = new_node;
        }
        parent_for_new_node->right_child = new_node;
      }
      node_with_max_key = new_node;
    }
    TreeUpdate(answer);
    return answer;
  }

 public:
  explicit ImplicitCartesianTree() {
    tree_ = nullptr;
  }

  explicit ImplicitCartesianTree(const std::vector<TreeType> &array) {
    tree_ = SortedBuild(array);
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

  void Reverse(int left_position, int right_position) {
    Reverse(tree_, left_position, right_position);
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
  int n = 0;
  int m = 0;
  cin >> n >> m;
  std::vector<int> input_array;
  for (int i = 0, input = 0; i < n; ++i) {
    cin >> input;
    input_array.push_back(input);
  }
  ImplicitCartesianTree<int> treap(input_array);
  for (int i = 0, request_type = 0, left = 0, right = 0; i < m; ++i) {
    cin >> request_type >> left >> right;
    if (request_type == 1) {
      treap.Reverse(left - 1, right - 1);
    }
    if (request_type == 2) {
      cout << treap.Minimum(left - 1, right - 1) << '\n';
    }
  }
  cin.close();
  cout.close();
}
