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
    NodeType sum;
    int priority;
    int size;

    explicit Node(NodeType input_value) {
      size = 1;
      value = input_value;
      sum = input_value;
      left_child = nullptr;
      right_child = nullptr;
      parent = nullptr;
      priority = rand();
    }

    ~Node() {
      delete left_child;
      delete right_child;
    }
  };

  Node<TreeType> *even_tree_;
  Node<TreeType> *odd_tree_;

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

  void SetParent(Node<TreeType> *child, Node<TreeType> *parent) {
    if (child == nullptr) {
      return;
    }
    child->parent = parent;
  }

  void Update(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
    root->sum = GetSum(root->left_child) + root->value + GetSum(root->right_child);
    SetParent(root->left_child, root);
    SetParent(root->right_child, root);
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

  std::pair<Node<TreeType> *, Node<TreeType> *> Split(Node<TreeType> *root, int position_for_split) {
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

  TreeType Sum(Node<TreeType> *root, int left_position, int right_position) {
    std::pair<Node<TreeType> *, Node<TreeType> *> left_buffer = Split(root, left_position);
    std::pair<Node<TreeType> *, Node<TreeType> *> right_buffer =
        Split(left_buffer.second, right_position - left_position + 1);
    TreeType answer = GetSum(right_buffer.first);
    root = Merge(left_buffer.first, Merge(right_buffer.first, right_buffer.second));
    return answer;
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

  int HalfIndex(int index) {
    return (index + 2) / 2 - 1;
  }

 public:
  explicit ImplicitCartesianTree() {
    even_tree_ = nullptr;
    odd_tree_ = nullptr;
  }

  explicit ImplicitCartesianTree(const std::vector<TreeType> &array) {
    std::vector<TreeType> even_array;
    std::vector<TreeType> odd_array;
    int size = static_cast<int>(array.size());
    for (int i = 0; i < size; ++i) {
      if (i % 2 == 0) {
        even_array.push_back(array[i]);
      } else {
        odd_array.push_back(array[i]);
      }
    }
    even_tree_ = SortedBuild(even_array);
    odd_tree_ = SortedBuild(odd_array);
  }

  ~ImplicitCartesianTree() {
    delete even_tree_;
    delete odd_tree_;
  }

  void Operation(int left_position, int right_position) {
    int even_left_position = HalfIndex(left_position + left_position % 2);
    int odd_left_position = HalfIndex(left_position + (left_position + 1) % 2);
    int even_right_position = HalfIndex(right_position - right_position % 2);
    int odd_right_position = HalfIndex(right_position - (right_position + 1) % 2);
    std::pair<Node<TreeType> *, Node<TreeType> *> even_left_buffer = Split(even_tree_, even_left_position);
    std::pair<Node<TreeType> *, Node<TreeType> *> even_right_buffer =
        Split(even_left_buffer.second, even_right_position - even_left_position + 1);

    std::pair<Node<TreeType> *, Node<TreeType> *> odd_left_buffer = Split(odd_tree_, odd_left_position);
    std::pair<Node<TreeType> *, Node<TreeType> *> odd_right_buffer =
        Split(odd_left_buffer.second, odd_right_position - odd_left_position + 1);

    even_tree_ = Merge(even_left_buffer.first, Merge(odd_right_buffer.first, even_right_buffer.second));
    odd_tree_ = Merge(odd_left_buffer.first, Merge(even_right_buffer.first, odd_right_buffer.second));
  }

  TreeType Sum(int left_position, int right_position) {
    int even_left_position = HalfIndex(left_position + left_position % 2);
    int odd_left_position = HalfIndex(left_position + (left_position + 1) % 2);
    int even_right_position = HalfIndex(right_position - right_position % 2);
    int odd_right_position = HalfIndex(right_position - (right_position + 1) % 2);
    return Sum(even_tree_, even_left_position, even_right_position) +
           Sum(odd_tree_, odd_left_position, odd_right_position);
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");

  int n = 1;
  int m = 1;
  int swapper_number = 0;

  cin >> n >> m;
  while (n && m) {
    ++swapper_number;
    cout << "Swapper " << swapper_number << ":\n";
    std::vector<int64_t> array;
    for (int i = 0, value = 0; i < n; ++i) {
      cin >> value;
      array.push_back(value);
    }

    ImplicitCartesianTree<int64_t> treap(array);

    for (int i = 0, left = 0, right = 0, request_type = 0; i < m; ++i) {
      cin >> request_type >> left >> right;
      --left;
      --right;
      if (request_type == 1) {
        treap.Operation(left, right);
      }
      if (request_type == 2) {
        cout << treap.Sum(left, right) << '\n';
      }
    }

    cin >> n >> m;
  }

  cin.close();
  cout.close();
}
