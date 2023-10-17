#include <iostream>
#include <vector>
#include <fstream>
#include <string>

template <typename TreeType>
class ExplicitCartesianTree {
private:
  template <typename NodeType>
  struct Node_ {
    Node_ *left_child;
    Node_ *right_child;

    NodeType key;
    int priority;
    int size;

    Node_(NodeType key_input) {
      size = 1;
      key = key_input;
      left_child = nullptr;
      right_child = nullptr;
      priority = (rand() << 32) | rand();
    }
  };

  Node_<TreeType> *tree_;

  int GetSize_(Node_<TreeType> *root) {
    if(root == nullptr) {
      return 0;
    } 
    return root->size;
  }

  bool CompareKey_(Node_ <TreeType> *root, TreeType input_key) {
    if(root == nullptr) {
      return false;
    } 
    return root->key == input_key;
  }

  void Update_(Node_<TreeType> *root) {
    if(root == nullptr) {
      return;
    }
    root->size = GetSize_(root->left_child) + 1 + GetSize_(root->right_child);
  }

  Node_<TreeType> *Merge_(Node_<TreeType> *tree_1, Node_<TreeType> *tree_2) {
    if(tree_1 == nullptr) {
      return tree_2;
    }
    if(tree_2 == nullptr) {
      return tree_1;
    }
    if(tree_1->priority < tree_2->priority) {
      tree_1->right_child = Merge_(tree_1->right_child, tree_2);
      Update_(tree_1);
      return tree_1;
    } else {
      tree_2->left_child = Merge_(tree_1, tree_2->left_child);
      Update_(tree_2);
      return tree_2;
    }
  }

  std::pair<Node_<TreeType> *, Node_<TreeType> *> Split_(Node_<TreeType> *root, TreeType key_for_split) {
    if(root == nullptr) {
      return {nullptr, nullptr};
    }
    if(root->key < key_for_split) {
      std::pair<Node_<TreeType> *, Node_<TreeType> *> buffer = Split_(root->right_child, key_for_split); // tree_1, tree_2
      root->right_child = buffer.first;
      Update_(root);
      return std::pair<Node_<TreeType> *, Node_<TreeType> *> (root, buffer.second);
    } else {
      std::pair<Node_<TreeType> *, Node_<TreeType> *> buffer = Split_(root->left_child, key_for_split); // tree_1, tree_2
      root->left_child = buffer.second;
      Update_(root);
      return std::pair<Node_<TreeType> *, Node_<TreeType> *> (buffer.first, root);
    }
  }

  Node_<TreeType> *Insert_(Node_<TreeType> *root, TreeType key_input) {
    Node_<TreeType> *inserting_node = nullptr; 
    std::pair<Node_<TreeType> *, Node_<TreeType> *> buffer = Split_(root, key_input); // tree_1, tree_2
    if(CompareKey_(root, key_input) == false) {
      inserting_node = new Node_<TreeType>(key_input); 
    }
    return Merge_(buffer.first, Merge_(inserting_node, buffer.second));
  }

  Node_<TreeType> *Erase_(Node_<TreeType> *root, TreeType key_input) {
    std::pair<Node_<TreeType> *, Node_<TreeType> *> left_buffer = Split_(root, key_input); // tree_left, other_tree
    std::pair<Node_<TreeType> *, Node_<TreeType> *> right_buffer = Split_(left_buffer.second, key_input + 1); // rubbish, tree_right
    delete right_buffer.first;
    return Merge_(left_buffer.first, right_buffer.second);
  }

  Node_<TreeType> *Build_(const std::vector <TreeType> &input_array) {
    Node_<TreeType> *answer = nullptr;
    for (int i = 0; i < input_array.size(); ++i) {
      answer = Insert_(answer, input_array[i]);
    }
    return answer;
  }

  bool Exists_(Node_<TreeType> *root, TreeType key_input) {
    if(root == nullptr) {
      return false;
    }
    if(root->key == key_input) {
      return true;
    } else if(key_input < root->key) {
      return Exists_(root -> left_child, key_input);
    } else {
      return Exists_(root -> right_child, key_input);
    }
  }

  std::pair<bool, int> OrderStatistics_(Node_<TreeType> *root, int number) { // 0 - индексация
    if(root == nullptr) {
      return std::pair<bool, int> (false, 0);
    }
    int left_size = GetSize_(root->left_child);
    if(left_size == number) {
      return std::pair<bool, int> (true, root->key);
    } else if(left_size < number) { 
      return OrderStatistics_(root->right_child, number - left_size - 1);
    } else {
      return OrderStatistics_(root->left_child, number);
    }

  }

  std::pair<bool, int> Maximum_(Node_<TreeType> *root) {
    if(root == nullptr) {
      return std::pair<bool, int> (false, 0);
    }
    if(root -> right_child == nullptr) {
      return std::pair<bool, int> (true, root->key);
    } 
    return Maximum_(root->right_child);
  }

  std::pair<bool, int> Minimum_(Node_<TreeType> *root) {
    if(root == nullptr) {
      return std::pair<bool, int> (false, 0);
    }
    if(root -> left_child == nullptr) {
      return std::pair<bool, int> (true, root->key);
    } 
    return Minimum_(root->left_child);
  }

  std::pair<bool, int> Previous_(Node_<TreeType> *root, TreeType key_input) {
    std::pair<Node_<TreeType> *, Node_<TreeType> *> buffer = Split_(root, key_input);
    std::pair<bool, int> answer = Maximum_(buffer.first);
    Merge_(buffer.first, buffer.second);
    return answer;
  }

  std::pair<bool, int> Next_(Node_<TreeType> *root, TreeType key_input) {
    std::pair<Node_<TreeType> *, Node_<TreeType> *> buffer = Split_(root, key_input + 1);
    std::pair<bool, int> answer = Minimum_(buffer.second);
    Merge_(buffer.first, buffer.second);
    return answer;
  }

public:
  explicit ExplicitCartesianTree() {
    tree_ = nullptr;
  }

  explicit ExplicitCartesianTree(const std::vector <TreeType> &array) {
    tree_ = Build_(array);
  }

  void Insert(TreeType key_input) {
    tree_ = Insert_(tree_, key_input);
  }

  void Erase(TreeType key_input) {
    tree_ = Erase_(tree_, key_input);
  }

  bool Exists(TreeType key_input) {
    return Exists_(tree_, key_input);
  }

  std::pair<bool, int> Previous(TreeType key_input) {
    return Previous_(tree_, key_input);
  }

  std::pair<bool, int> Next(TreeType key_input) {
    return Next_(tree_, key_input);
  }

  std::pair<bool, int> OrderStatistics(int number) {
    return OrderStatistics_(tree_, number);
  }
};

int main() {
  std::ifstream cin;
  std::ofstream cout;
  cin.open("input.txt");
  cout.open("output.txt");
  std::string request_type;
  int input_value = 0;
  ExplicitCartesianTree<int> treap;
  std::pair<bool, int> buffer;
  while(cin >> request_type) {
    cin >> input_value;
    switch (request_type[0])
    {
    case 'i':
      treap.Insert(input_value);
      break;
    
    case 'd':
      treap.Erase(input_value);
      break;
    
    case 'e':
      if(treap.Exists(input_value) == true) {
        cout << "true\n";
      } else {
        cout << "false\n";
      }
      break;

    case 'n':
      buffer = treap.Next(input_value);
      if(buffer.first == true) {
        cout << buffer.second << '\n';
      } else {
        cout << "none\n";
      }
      break;

    case 'p':
      buffer = treap.Previous(input_value);
      if(buffer.first == true) {
        cout << buffer.second << '\n';
      } else {
        cout << "none\n";
      }
      break;

    case 'k':
      buffer = treap.OrderStatistics(input_value);
      if(buffer.first == true) {
        cout << buffer.second << '\n';
      } else {
        cout << "none\n";
      }
      break;
    }
  }
  cin.close();
  cout.close();
}
