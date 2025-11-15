#include <algorithm>
#include <functional>
#include <iostream>

struct Node {
    int id;
    int score;
    Node* left = nullptr;
    Node* right = nullptr;
};

using addr = Node*;


/* init */
class BST {
public:
    addr InsertBST(int id, int score, addr root) {
        if (!root) {
            return CreateBST(id, score);
        }
        if (id < root->id) {
            root->left = InsertBST(id, score, root->left);
        } else {
            root->right = InsertBST(id, score, root->right);
        }
        return root;
    }

    void PrintBST(addr root) const {
        if (!root) {
            return;
        }
        PrintBST(root->left);
        std::cout << "id: " << root->id << ", score: " << root->score << '\n';
        PrintBST(root->right);
    }

    int HeightBST(addr root) const {
        if (!root) {
            return 0;
        }
        int leftHeight = HeightBST(root->left);
        int rightHeight = HeightBST(root->right);
        return std::max(leftHeight, rightHeight) + 1;
    }

    double SearchAVGBST(addr root, int id) const {
        int sum = 0;
        int count = 0;
        std::function<void(addr)> dfs = [&](addr node) {
            if (!node) {
                return;
            }
            if (node->id == id) {
                sum += node->score;
                ++count;
            }
            dfs(node->left);
            dfs(node->right);
        };
        dfs(root);
        if (count == 0) {
            return 0.0;
        }
        return static_cast<double>(sum) / count;
    }

private:
    static addr CreateBST(int id, int score) {
        return new Node{id, score, nullptr, nullptr};
    }
};

class AVLTree {
public:
    AVLTree() = default;
};

class Treap {
public:
    Treap() = default;
};

class SkipList {
public:
    SkipList() = default;
};

int main() {
    BST bst;
    AVLTree avl;
    Treap treap;
    SkipList skipList;
    (void)bst;
    (void)avl;
    (void)treap;
    (void)skipList;
    return 0;
}
