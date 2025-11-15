#include <algorithm>
#include <functional>
#include <iostream>

struct Node {
    int id;
    int score;
    Node* left = nullptr;
    Node* right = nullptr;

    Node() = default;
    Node(int idValue, int scoreValue, Node* leftNode = nullptr, Node* rightNode = nullptr)
        : id(idValue), score(scoreValue), left(leftNode), right(rightNode) {}
};

using addr = Node*;

struct AVLNode {
    int id;
    int score;
    int height = 1;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;

    AVLNode() = default;
    AVLNode(int idValue, int scoreValue, AVLNode* leftNode = nullptr, AVLNode* rightNode = nullptr)
        : id(idValue), score(scoreValue), height(1), left(leftNode), right(rightNode) {}
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
        return new Node(id, score);
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
        return new AVLNode(id, score);
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
    addr bstRoot = nullptr;
    bstRoot = bst.InsertBST(10, 80, bstRoot);
    bstRoot = bst.InsertBST(5, 90, bstRoot);
    bstRoot = bst.InsertBST(15, 70, bstRoot);
    bstRoot = bst.InsertBST(5, 95, bstRoot);  // duplicate id to test averaging
    bst.PrintBST(bstRoot);
    std::cout << "BST Height: " << bst.HeightBST(bstRoot) << '\n';
    std::cout << "BST AVG for id 5: " << bst.SearchAVGBST(bstRoot, 5) << '\n';

    AVLTree avl;
    avl_addr avlRoot = nullptr;
    avlRoot = avl.InsertAVLTree(10, 80, avlRoot);
    avlRoot = avl.InsertAVLTree(5, 90, avlRoot);
    avlRoot = avl.InsertAVLTree(15, 70, avlRoot);
    avlRoot = avl.InsertAVLTree(5, 95, avlRoot);
    avl.PrintAVLTree(avlRoot);
    std::cout << "AVL Height: " << avl.HeightAVLTree(avlRoot) << '\n';
    std::cout << "AVL AVG for id 5: " << avl.SearchAVGAVLTree(avlRoot, 5) << '\n';

    Treap treap;
    SkipList skipList;
    (void)treap;
    (void)skipList;
    return 0;
}
