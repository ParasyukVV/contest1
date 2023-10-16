#include <iostream>
#include <vector>

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

Node_ *Erase(Node_ *root, int key_input) {
  std::pair<Node_ *, Node_ *> left_buffer = Split(root, key_input); // tree_left, other_tree
  std::pair<Node_ *, Node_ *> right_buffer = Split(left_buffer.second, key_input + 1); // rubbish, tree_right
  delete right_buffer.first;
  return Merge(left_buffer.first, right_buffer.second);
}

Node_ *Build(const std::vector <int> &input_array) {
  Node_ *answer = nullptr;
  for (int i = 0; i < input_array.size(); ++i) {
    answer = Insert(answer, input_array[i]);
  }
  return answer;
}

// Тут кусок для вывода дерева
std::pair <int, int> data(Node_ *root) {
  if (root == nullptr) {
    return {0, 0};
  }
  return {root->key, root->priority};
}

void BuildArray(Node_ *root, int num, std::vector<std::pair<int, int> > &array) {
  if(root == nullptr) 
    return;
  array[num] = data(root);
  BuildArray(root->left_child, 2 * num + 1, array);
  BuildArray(root->right_child, 2 * num + 2, array);
}

int Depth(Node_ *root) {
  if(root == nullptr) 
    return 0;
  return 1 + std::max(Depth(root->left_child), Depth(root->right_child));
}

int Exponentation (int basis, int indicator) {
  if (indicator == 0) 
    return 1;
  if(indicator % 2 == 0) 
    return Exponentation(basis, indicator / 2) * Exponentation(basis, indicator / 2); // Да, это очень плохо
  else 
    return basis * Exponentation(basis, indicator - 1);
}

void PrintTree(Node_ *root) {
  std::vector<std::pair<int, int> > answer;
  answer.resize(Exponentation(2, Depth(root)) - 1, {0, 0});
  BuildArray(root, 0, answer);
  for(int i = 0; i < answer.size(); ++i) {
    std::cout << '(' << answer[i].first << ", " << answer[i].second << ") ";
  }
  std::cout << '\n';
}
// Кусок про вывод дерева закончился

int main() {
  int n;
  std::cin >> n;
  std::vector<int> array1;
  for (int i = 0, input = 0; i < n; ++i) {
    std::cin >> input;
    array1.push_back(input);
  }
  Node_ *treap1 = Build(array1);
  /*std::cin >> n;
  std::vector<int> array2;
  for (int i = 0, input = 0; i < n; ++i) {
    std::cin >> input;
    array2.push_back(input);
  }
  Node_ *treap2 = Build(array2); */
  treap1 = Erase(treap1, 5);
  PrintTree(treap1);
}
