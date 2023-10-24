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
    Node *parent;

    NodeType value;
    int priority;
    int size;

    explicit Node(NodeType input_value) {
      size = 1;
      value = input_value;
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

  void SetParent(Node<TreeType> *child, Node<TreeType> *parent) {
    if(child == nullptr) {
      return;
    }
    child->parent = parent;
  }

  void Update(Node<TreeType> *root) {
    if (root == nullptr) {
      return;
    }
    root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
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
    if(current_node == nullptr) {
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
    Node<TreeType> *answer = new Node(array[0]);
    Node<TreeType> *node_with_max_key = answer;
    int size = static_cast<int>(array.size());
    for (int i = 1; i < size; ++i) {
      Node<TreeType> *new_node = new Node(array[i]);
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

  void Operation(int left_position, int right_position) {
    return Operation(tree_, left_position, right_position);
  }

  void DestroyPrint() {
    while(tree_) {
      std::pair<Node<TreeType> *, Node<TreeType> *> buffer = Split(tree_, 1);
      std::cout << buffer.first->value << ' ';
      tree_ = buffer.second;
    }
    std::cout << '\n';
  }
};

int main() {
  int n;
  std::cin >> n;
  std::vector<int> array;
  for(int i = 1; i <= n; ++i) {
    array.push_back(i);
  }
  ImplicitCartesianTree treap(array);
  treap.Operation(1, 2);
  treap.DestroyPrint();
}
