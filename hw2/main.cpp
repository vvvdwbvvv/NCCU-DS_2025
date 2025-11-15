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

struct AVLNode {
    int id;
    int score;
    int height = 1;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
};

using avl_addr = AVLNode*;


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
    avl_addr CreateAVLTree(int id, int score) {
        return createRoot(id, score);
    }

    avl_addr InsertAVLTree(int id, int score, avl_addr root) {
            if (!root) {
                return createRoot(id, score);
            }
            if (id < root->id) {
                root->left = InsertAVLTree(id, score, root->left);
            } else if (id > root->id) {
                root->right = InsertAVLTree(id, score, root->right);
            } else {
                root->score = score;
                return root;
            }
            updateHeight(root);
            int balance = getBalance(root);

            if (balance > 1 && getBalance(root->left) >= 0) {
                return rotateRight(root);
            }
            if (balance > 1 && getBalance(root->left) < 0) {
                root->left = rotateLeft(root->left);
                return rotateRight(root);
            }
            if (balance < -1 && getBalance(root->right) <= 0) {
                return rotateLeft(root);
            }
            if (balance < -1 && getBalance(root->right) > 0) {
                root->right = rotateRight(root->right);
                return rotateLeft(root);
        }
        return root;
    }

    void PrintAVLTree(avl_addr root) const {
        if (!root) {
            return;
        }
        PrintAVLTree(root->left);
        std::cout << "id: " << root->id << ", score: " << root->score << ", height: " << root->height << '\n';
        PrintAVLTree(root->right);
    }

    int HeightAVLTree(avl_addr root) const {
        if (!root) {
            return 0;
        }
        return root->height;
    }

    double SearchAVGAVLTree(avl_addr root, int id) const {
        int sum = 0;
        int count = 0;
        std::function<void(avl_addr)> dfs = [&](avl_addr node) {
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
    static avl_addr createRoot(int id, int score) {
        return new AVLNode{id, score};
    }

    static int height(avl_addr node) {
            return node ? node->height : 0;
        }

        static void updateHeight(avl_addr node) {
            node->height = std::max(height(node->left), height(node->right)) + 1;
        }

        static int getBalance(avl_addr node) {
            return node ? height(node->left) - height(node->right) : 0;
        }

        static avl_addr rotateRight(avl_addr y) {
            avl_addr x = y->left;
            avl_addr T2 = x->right;
            x->right = y;
            y->left = T2;
            updateHeight(y);
            updateHeight(x);
            return x;
        }

        static avl_addr rotateLeft(avl_addr x) {
            avl_addr y = x->right;
            avl_addr T2 = y->left;
            y->left = x;
            x->right = T2;
            updateHeight(x);
            updateHeight(y);
            return y;
        }
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
