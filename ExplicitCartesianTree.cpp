#include <iostream>

struct Node_ {
  Node_ *left_child;
  Node_ *right_child;

  int key;
  int priority;
  int size;

  Node_(int key_input) {
		size = 1;
		key = key_input;
		left_child = nullptr;
    right_child = nullptr;
		priority = (rand() << 32) | rand();
  }
};

int GetSize(Node_* root) {
  if(root == nullptr) {
    return 0;
  } 
  return root->size;
}

void Update(Node_ *root) {
  if(root == nullptr) {
    return;
  }
  root->size = GetSize(root->left_child) + 1 + GetSize(root->right_child);
}

Node_ *Merge(Node_ *tree_1, Node_ *tree_2) {
  if(tree_1 == nullptr) {
    return tree_2;
  }
  if(tree_2 == nullptr) {
    return tree_1;
  }
  if(tree_1->priority < tree_2->priority) {
    tree_1->right_child = Merge(tree_1->right_child, tree_2);
    Update(tree_1);
    return tree_1;
  } else {
    tree_2->left_child = Merge(tree_1, tree_2->left_child);
    Update(tree_2);
    return tree_2;
  }
}

std::pair<Node_ *, Node_ *> Split(Node_ *root, int key_for_split) {
  if(root == nullptr) {
    return {nullptr, nullptr};
  }
  if(root->key < key_for_split) {
    std::pair<Node_ *, Node_ *> buffer = Split(root->right_child, key_for_split); // tree_1, tree_2
    root->right_child = buffer.first;
    Update(root);
    return std::pair<Node_ *, Node_ *> (root, buffer.second);
  } else {
    std::pair<Node_ *, Node_ *> buffer = Split(root->left_child, key_for_split); // tree_1, tree_2
    root->left_child = buffer.second;
    Update(root);
    return std::pair<Node_ *, Node_ *> (buffer.first, root);
  }
}

Node_ *Insert(Node_ *root, int key_input) {
  Node_ *inserting_node = new Node_(key_input); 
  std::pair<Node_ *, Node_ *> buffer = Split(root, key_input); // tree_1, tree_2
  return Merge(buffer.first, Merge(inserting_node, buffer.second));
}

int main() {
  std::cout << "bidlo\n";
}
