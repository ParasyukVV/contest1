#include <iostream>

// Сделать норм рандом

struct TreapNode {
    int value; // Во избежание путаницы с неявным ключом использую это наименование
    int priority;
    TreapNode *leftChild;
    TreapNode *rightChild;
    int size;
    TreapNode() {
		size = 1;
		leftChild = nullptr;
        rightChild = nullptr;
		priority = rand(); // !Тут надо сделать нормальный рандом!
	}	
    TreapNode(int valueInput) {
        size = 1;
        value = valueInput;
		leftChild = nullptr;
        rightChild = nullptr;
		priority = rand(); // !Тут надо сделать нормальный рандом!
    }
};

int GetSize(TreapNode *root) { // Число элементов в декартовом дереве за O(1)
    if(root == nullptr) return 0;
    return root -> size;
}

void Update(TreapNode *root) {
    root->size = GetSize(root->leftChild) + 1 + GetSize(root->rightChild);
}

std::pair<TreapNode *, TreapNode *> SplitByImplicitKey(TreapNode *root, int key) {// Split по НЕявному ключу за O(log N); в первое уходят вершины c индексами [0, ..., key]
    if(root == nullptr) 
        return {nullptr, nullptr};
    int leftSize = GetSize(root->leftChild); // Размер левого поддерева, то есть неявный ключ root
    if(key < leftSize) { 
        std::pair<TreapNode *, TreapNode *> buffer = SplitByImplicitKey(root->leftChild, key); // Если введенный ключ < ключа в вершине дерева, то сплитим левое поддерево
        root->leftChild = buffer.second; // Подвешиваем правую часть сплита левого поддерева на место левого поддерева
        Update(root);
        return {buffer.first, root}; 
    } else {
        std::pair<TreapNode *, TreapNode *> buffer = SplitByImplicitKey(root->rightChild, key - leftSize - 1); // Если введенный ключ >= ключа в вершине дерева, то сплитим правое поддерево
        root->rightChild = buffer.first; // Подвешиваем левую часть сплита правого поддерева на место правого поддерева
        Update(root);
        return {root, buffer.second}; 
    }
    
}

TreapNode * Merge(TreapNode *left, TreapNode *right) { // Объединение двух деревьев за O(log N). Тут мы верим что все ключи в left меньше всех ключей в right
    if(left == nullptr)
        return right;
    if(right == nullptr) 
        return left;
    if(left->priority < right->priority) { 
        left->rightChild = Merge(left->rightChild, right); // Если приоритет левого дерева меньше, то мерджим правого потомка левого дерева с правым деревом и подвешиваем результат на место правого потомка левого дерева
        Update(left);
        return left; // Результат остаётся деревом поиска по ключу в предположении что все ключи в исходном left меньше всех ключей в исходном right
    } else {
        right->leftChild = Merge(left, right->leftChild); // Если приоритет левого дерева больше либо равен, то мерджим левое дерево с левым потомком правого дерева и подвешиваем результат на место левого потомка правого дерева
        Update(right);
        return right; // Результат остаётся деревом поиска по ключу в предположении что все ключи в исходном left меньше всех ключей в исходном right
    }
}

TreapNode * Insert (TreapNode *root, int position, TreapNode *input) { // Добавление вершины так, что она окажется с индексом pos. input - это одна вершина, а не дерево; key в 0 - нотации
    if(root == nullptr)
        return input;
    std::pair<TreapNode *, TreapNode *> buffer = SplitByImplicitKey(root, position - 1); // Если position меньше 0 или больше размера root, то сработает push forward или back соответственно
    return Merge(Merge(buffer.first, input), buffer.second);
}

TreapNode * Erase (TreapNode *root, int position) { // Удаление вершины по её индексу за O(log N)
    if(root == nullptr)
        return nullptr;
    int leftSize = GetSize(root->leftChild); // Размер левого поддерева, то есть ключ root
    if(position == leftSize)
        return Merge(root->leftChild, root->rightChild); // Если введённый неявный ключ равен неявному ключу корня, то мерджим потомков корня
    if(position < leftSize) {
        root->leftChild = Erase(root->leftChild, position); // Если введённый неявный ключ меньше неявного ключа в корне, то удаляем из левого поддерева по тому же ключу
        return root;
    } 
    if(position > leftSize) {
        root->rightChild = Erase(root->rightChild, position - leftSize - 1); // Если введённый неявный ключ больше неявного ключа в корне, то удаляем из правого поддерева по тому же ключу
        return root;
    }
}

int GetValue(TreapNode *root, int position) { // Значение по неявному ключу за O(log N) по аналогии с Erase
    if(root == nullptr)
        return 0;
    int leftSize = GetSize(root->leftChild); 
    if(position == leftSize)
        return root->value; 
    if(position < leftSize) {
        return GetValue(root->leftChild, position);
    } 
    if(position > leftSize) {
        return GetValue(root->rightChild, position - leftSize - 1); ;
    }
}



int main() {
    TreapNode* root = nullptr;
    root = Insert(root, 0, new TreapNode(7));
    root = Insert(root, 1, new TreapNode(8));
    root = Erase(root, 0);
    std::cout << GetValue(root, 0) << "\n";
    return 0;
}