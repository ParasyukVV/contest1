#include <iostream>

// Сделать норм рандом

template<typename treapType>
class treap {
private:
    treapType key;
    int priority;
    treap *leftChild;
    treap *rightChild;
    int size;
public:
    treap(treapType keyValue) { // Конструктор по ключу
		size = 1;
		key = keyValue;
		leftChild = nullptr;
        rightChild = nullptr;
		priority = rand(); // !Тут надо сделать нормальный рандом!
	}	

    int GetSize(treap *root) { // Число элементов в декартовом дереве за O(1)
        if(root == nullptr) return 0;
        return root -> size;
    }

    std::pair<treap *, treap *> SplitByExplicitKey(treap *root, treapType key) {// Split по явному ключу за O(log N)
        if(root == nullptr) 
            return {nullptr, nullptr};
        if(key < root->key) { 
            std::pair<treap *, treap *> buffer = SplitByExplicitKey(root->leftChild, key); // Если введенный ключ < ключа в вершине дерева, то сплитим левое поддерево
            root->left = buffer.second; // Подвешиваем правую часть сплита левого поддерева на место левого поддерева
            return {buffer.first, root}; 
        } else {
            std::pair<treap *, treap *> buffer = SplitByExplicitKey(root->rightChild, key); // Если введенный ключ >= ключа в вершине дерева, то сплитим правое поддерево
            root->right = buffer.first; // Подвешиваем левую часть сплита правого поддерева на место правого поддерева
            return {root, buffer.second}; 
        }
    }

    treap * Merge(treap *left, treap *right) { // Объединение двух деревьев за O(log N). Тут мы верим что все ключи в left меньше всех ключей в right
        if(left == nullptr)
            return right;
        if(right == nullptr) 
            return left;
        if(left->priority < right->priority) { 
            left->rightChild = Merge(left->rightChild, right); // Если приоритет левого дерева меньше, то мерджим правого потомка левого дерева с правым деревом и подвешиваем результат на место правого потомка левого дерева
            return left; // Результат остаётся деревом поиска по ключу в предположении что все ключи в исходном left меньше всех ключей в исходном right
        } else {
            right->leftChild = Merge(left, right->leftChild); // Если приоритет левого дерева больше либо равен, то мерджим левое дерево с левым потомком правого дерева и подвешиваем результат на место левого потомка правого дерева
            return right; // Результат остаётся деревом поиска по ключу в предположении что все ключи в исходном left меньше всех ключей в исходном right
        }
    }

    treap * Insert (treap *root, treap *input) { // Добавление новой вершины в дерево за O(log N). input - это одна вершина, а не дерево
        if(root == nullptr)
            return input;
        std::pair<treap *, treap *> buffer = SplitByExplicitKey(root, input->key); // Сплитим дерево по ключу вставляемой вершины
        return Merge(Merge(buffer.first, input), buffer.second);
    }

    treap * Erase (treap *root, treapType key) { // Удаление вершины по ключу за O(log N)
        if(root == nullptr)
            return nullptr;
        if(key == root->key)
            return Merge(root->leftChild, root->rightChild); // Если введённый ключ равен ключу корня, то мерджим потомков корня
        if(key < root->key) {
            root->leftChild = Erase(root->leftChild, key); // Если введённый ключ меньше ключа в корне, то удаляем из левого поддерева по тому же ключу
            return root;
        } 
        if(key > root->key) {
            root->rightChild = Erase(root->rightChild, key); // Если введённый ключ больше ключа в корне, то удаляем из правого поддерева по тому же ключу
            return root;
        }
    }
};

int main() {
    treap<int> auf(78);
    std::cout << "test\n";
    return 0;
}