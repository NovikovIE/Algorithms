#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::max;
using std::min;
using std::string;

const int NOT_IN_TREE = 2e9;

struct Node {
    int value;
    int height = 1;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int val) : value(val) {}
};

class AVLTree {
public:
    explicit AVLTree() = default;
    ~AVLTree() { clear(root); }
    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;
    void insert(int value);
    void erase(int value);
    bool exists(int value) const { return find(root, value); }
    bool next(int value, int& next_value) const;
    bool prev(int value, int& prev_value) const;

private:
    Node* root = nullptr;

    Node* insert(Node* node, int value);
    Node* balance(Node* node);
    int height_balance(Node* node) const;
    Node* rotate_right(Node* node);
    Node* rotate_left(Node* node);
    bool find(Node* node, int value) const;
    void update_height(Node* node);
    Node* erase(Node* node, int value);
    Node* find_max(Node* node) const;
    Node* remove(Node* node);
    int next(Node* node, int value) const;
    int prev(Node* node, int value) const;
    void clear(Node* node);
};

void AVLTree::insert(int value) {
    if (exists(value))
        return;
    if (!root)
        root = insert(root, value);
    else
        insert(root, value);
}

Node* AVLTree::insert(Node* node, int value) {
    if (!node)
        return new Node(value);
    if (node->value < value)
        node->right = insert(node->right, value);
    else if (node->value > value)
        node->left = insert(node->left, value);
    return balance(node);
}

Node* AVLTree::balance(Node* node) {
    update_height(node);
    if (height_balance(node) == 2) {
        if (((node->right) ? (height_balance(node->right)) : (0)) < 0)
            node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    if (height_balance(node) == -2) {
        if (((node->left) ? (height_balance(node->left)) : (0)) > 0)
            node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    return node;
}

int AVLTree::height_balance(Node* node) const {
    int left_height = (node->left) ? (node->left->height) : (0);
    int right_height = (node->right) ? (node->right->height) : (0);
    return right_height - left_height;
}

Node* AVLTree::rotate_right(Node* node) {
    bool is_root = (node == root);
    Node* left = node->left;
    node->left = left->right;
    left->right = node;
    if (is_root)
        root = left;
    update_height(node);
    update_height(left);
    return left;
}

Node* AVLTree::rotate_left(Node* node) {
    bool is_root = (node == root);
    Node* right = node->right;
    node->right = right->left;
    right->left = node;
    if (is_root)
        root = right;
    update_height(node);
    update_height(right);
    return right;
}

bool AVLTree::find(Node* node, int value) const {
    if (!node)
        return false;
    if (node->value < value)
        return find(node->right, value);
    if (node->value > value)
        return find(node->left, value);
    return true;
}

void AVLTree::update_height(Node* node) {
    int left_height = (node->left) ? (node->left->height) : (0);
    int right_height = (node->right) ? (node->right->height) : (0);
    node->height = max(left_height, right_height) + 1;
}

Node* AVLTree::erase(Node* node, int value) {
    if (node->value < value)
        node->right = erase(node->right, value);
    else if (node->value > value)
        node->left = erase(node->left, value);
    else {
        bool is_root = (node == root);
        Node* right = node->right;
        Node* left = node->left;
        delete node;
        if (!left) {
            if (is_root)
                root = right;
            return right;
        }
        Node* max_in_left = find_max(left);
        max_in_left->left = remove(left);
        max_in_left->right = right;
        if (is_root) {
            root = balance(max_in_left);
            return root;
        }
        return balance(max_in_left);
    }
    return balance(node);
}

Node* AVLTree::find_max(Node* node) const {
    if (node->right)
        return find_max(node->right);
    return node;
}

Node* AVLTree::remove(Node* node) {
    if (!node->right)
        return node->left;
    node->right = remove(node->right);
    return balance(node);
}

int AVLTree::next(Node* node, int value) const {
    if (!node || (!node->left && !node->right && node->value <= value))
        return NOT_IN_TREE;
    if (value >= node->value)
        return next(node->right, value);
    if (!node->left)
        return node->value;
    int temp = next(node->left, value);
    return (temp == NOT_IN_TREE) ? (node->value) : (min(node->value, temp));
}

int AVLTree::prev(Node* node, int value) const {
    if (!node || (!node->left && !node->right && node->value >= value))
        return NOT_IN_TREE;
    if (value <= node->value)
        return prev(node->left, value);
    if (!node->right)
        return node->value;
    int temp = prev(node->right, value);
    return (temp == NOT_IN_TREE) ? (node->value) : (max(node->value, temp));
}

void AVLTree::clear(Node* node) {
    if (!node)
        return;
    if (node->left)
        clear(node->left);
    if (node->right)
        clear(node->right);
    delete node;
}

bool AVLTree::next(int value, int& next_value) const {
    next_value = next(root, value);
    return next_value != NOT_IN_TREE;
}

bool AVLTree::prev(int value, int& prev_value) const {
    prev_value = prev(root, value);
    return prev_value != NOT_IN_TREE;
}

void AVLTree::erase(int value) {
    if (exists(value))
        erase(root, value);
}
