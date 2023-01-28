// add(x)      -   add element to set
// sum(l, r)   -   sum of   l <= x <= r

#include <iostream>

using std::cin;
using std::cout;

struct Node {
    int value = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    long long sum = 0;
    explicit Node(int new_value) : value(new_value), sum(value) {}
};

class Splay {
public:
    explicit Splay() = default;
    ~Splay();
    Splay& operator=(const Splay& splay) = delete;
    Splay& operator=(Splay&& splay) = delete;
    Splay(const Splay& splay) = delete;
    Splay(Splay&& splay) = delete;
    void add(int value);
    long long sum(int left, int right);

private:
    Node* root = nullptr;

    long long get_sum_less(int value);
    Node* find(Node* node, int value) const;
    void insert(Node* node, int value);
    std::pair<Node*, Node*> split(Node* node, int value);
    void update_sum(Node* node);
    Node* right_rotate(Node* node);
    Node* left_rotate(Node* node);
    Node* splay(Node* node, int value);
    void clear(Node* node);
};

void Splay::add(int value) {
    if (!root)
        root = new Node(value);
    else if (value == find(root, value)->value)
        return;
    else
        insert(root, value);
}

long long Splay::sum(int left, int right) {
    if (!root)
        return 0;
    return get_sum_less(right) - get_sum_less(left - 1);
}

long long Splay::get_sum_less(int value) {
    auto parts = split(root, value);
    if (!parts.first) {
        root = parts.second;
        return (parts.second && parts.second->value == value) ? (value) : (0);
    } else if (!parts.second) {
        root = parts.first;
        return parts.first->sum;
    } else {
        long long result = parts.first->sum;
        parts.second = splay(parts.second, find(parts.second, parts.first->value)->value);
        if (parts.second->value == value)
            result += parts.second->value;
        parts.second->left = parts.first;
        update_sum(parts.second);
        root = parts.second;
        return result;
    }
}

Node* Splay::find(Node* node, int value) const {
    if (!node)
        return nullptr;
    if (node->value == value)
        return node;
    if (node->left && value < node->value)
        return find(node->left, value);
    if (node->right && value > node->value)
        return find(node->right, value);
    return node;
}

void Splay::insert(Node* node, int value) {
    auto parts = split(node, value);
    Node* new_node = new Node(value);
    new_node->left = parts.first;
    new_node->right = parts.second;
    update_sum(new_node);
    root = new_node;
}

std::pair<Node*, Node*> Splay::split(Node* node, int value) {
    if (!node)
        return {nullptr, nullptr};
    Node* temp = splay(node, value);
    if (temp->value < value) {
        Node* right = temp->right;
        temp->right = nullptr;
        update_sum(temp);
        return {temp, right};
    } else {
        Node* left = temp->left;
        temp->left = nullptr;
        update_sum(temp);
        return {left, temp};
    }
}

void Splay::update_sum(Node* node) {
    node->sum = node->value;
    if (node->left)
        node->sum += node->left->sum;
    if (node->right)
        node->sum += node->right->sum;
}

Node* Splay::right_rotate(Node* node) {
    Node* temp = node->left;
    node->left = temp->right;
    update_sum(node);
    temp->right = node;
    update_sum(temp);
    return temp;
}

Node* Splay::left_rotate(Node* node) {
    Node* temp = node->right;
    node->right = temp->left;
    update_sum(node);
    temp->left = node;
    update_sum(temp);
    return temp;
}

Node* Splay::splay(Node* node, int value) {
    if (!node || node->value == value)
        return node;
    if (node->value > value) {
        if (!node->left)
            return node;
        if (node->left->value > value) {
            node->left->left = splay(node->left->left, value);
            update_sum(node->left);
            update_sum(node);
            node = right_rotate(node);
        } else if (node->left->value < value) {
            node->left->right = splay(node->left->right, value);
            update_sum(node->left);
            update_sum(node);
            if (node->left->right)
                node->left = left_rotate(node->left);
        }
        return (!node->left) ? (node) : (right_rotate(node));
    } else {
        if (!node->right)
            return node;
        if (node->right->value > value) {
            node->right->left = splay(node->right->left, value);
            update_sum(node->right);
            update_sum(node);
            if (node->right->left) {
                node->right = right_rotate(node->right);
                update_sum(node);
            }
        } else if (node->right->value < value) {
            node->right->right = splay(node->right->right, value);
            update_sum(node->right);
            update_sum(node);
            node = left_rotate(node);
        }
        return (!node->right) ? (node) : (left_rotate(node));
    }
}

void Splay::clear(Node* node) {
    if (node->left)
        clear(node->left);
    if (node->right)
        clear(node->right);
    delete node;
}

Splay::~Splay() {
    if (root)
        clear(root);
}
