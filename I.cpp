#include <iostream>
#include <vector>

struct Node {
  Node *left_child;
  Node *right_child;
  Node *parent;

  int key;
  int priority;

  explicit Node(int key_input, int priority_input) {
    left_child = nullptr;
    right_child = nullptr;
    parent = nullptr;

    key = key_input;
    priority = priority_input;
  }
};

Node *LowerPriorityAncestor(Node *current_node, int priority_input) {
  if (current_node == nullptr) {
    return nullptr;
  }
  if (current_node->priority < priority_input) {
    return current_node;
  }
  return LowerPriorityAncestor(current_node->parent, priority_input);
}

Node *SortedBuild(std::vector<std::pair<int, int>> sorted_array) {
  if (sorted_array.empty()) {
    return nullptr;
  }
  Node *answer = new Node(sorted_array[0].first, sorted_array[0].second);
  Node *node_with_max_key = answer;
  int size = static_cast<int>(sorted_array.size());
  for (int i = 1; i < size; ++i) {
    Node *new_node = new Node(sorted_array[i].first, sorted_array[i].second);
    Node *parent_for_new_node = LowerPriorityAncestor(node_with_max_key, sorted_array[i].second);
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
  return answer;
}

struct AnswerNode {
  int parent;
  int left;
  int right;

  explicit AnswerNode(int parent_input = 0, int left_input = 0, int right_input = 0) {
    parent = parent_input;
    left = left_input;
    right = right_input;
  }
};

void Count(Node *tree, std::vector<AnswerNode> &answer) {
  if (tree == nullptr) {
    return;
  }
  int parent = 0;
  int left = 0;
  int right = 0;
  if (tree->parent != nullptr) {
    parent = tree->parent->key;
  }
  if (tree->left_child != nullptr) {
    left = tree->left_child->key;
  }
  if (tree->right_child != nullptr) {
    right = tree->right_child->key;
  }
  answer[tree->key - 1] = AnswerNode(parent, left, right);
  Count(tree->left_child, answer);
  Count(tree->right_child, answer);
}

int main() {
  std::vector<std::pair<int, int>> sorted_array;
  int n = 0;
  std::cin >> n;
  for (int i = 0, value = 0; i < n; ++i) {
    std::cin >> value >> value;
    sorted_array.emplace_back(i + 1, value);
  }
  Node *tree = SortedBuild(sorted_array);
  std::vector<AnswerNode> answer(n);
  Count(tree, answer);
  std::cout << "YES\n";
  for (int i = 0; i < n; ++i) {
    std::cout << answer[i].parent << ' ' << answer[i].left << ' ' << answer[i].right << '\n';
  }
}
