#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>

struct Node {
  int id;
  int score;
  Node *left = nullptr;
  Node *right = nullptr;

  Node() = default;
  Node(int idValue, int scoreValue, Node *leftNode = nullptr,
       Node *rightNode = nullptr)
      : id(idValue), score(scoreValue), left(leftNode), right(rightNode) {}
};

using addr = Node *;

struct AVLNode {
  int id;
  int score;
  int height = 1;
  AVLNode *left = nullptr;
  AVLNode *right = nullptr;

  AVLNode() = default;
  AVLNode(int idValue, int scoreValue, AVLNode *leftNode = nullptr,
          AVLNode *rightNode = nullptr)
      : id(idValue), score(scoreValue), height(1), left(leftNode),
        right(rightNode) {}
};

using avl_addr = AVLNode *;

// AVL tree with |balance factor| <= 3
class AVLTreeBF3 {
public:
  avl_addr InsertAVLTreeBF3(int id, int score, avl_addr root) {
    cacheBuilt = false;
    if (!root) {
      return createRoot(id, score);
    }
    if (id == root->id) {
      root->score = score;
      return root;
    }
    if (score < root->score) {
      root->left = InsertAVLTreeBF3(id, score, root->left);
    } else {
      root->right = InsertAVLTreeBF3(id, score, root->right);
    }
    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 3 && getBalance(root->left) >= 0) {
      return rotateRight(root);
    }
    if (balance > 3 && getBalance(root->left) < 0) {
      root->left = rotateLeft(root->left);
      return rotateRight(root);
    }
    if (balance < -3 && getBalance(root->right) <= 0) {
      return rotateLeft(root);
    }
    if (balance < -3 && getBalance(root->right) > 0) {
      root->right = rotateRight(root->right);
      return rotateLeft(root);
    }
    return root;
  }

  int HeightAVLTreeBF3(avl_addr root) const {
    if (!root) {
      return 0;
    }
    return root->height;
  }

  void PrintAVLTreeBF3(avl_addr root) const {
    if (!root) {
      return;
    }
    PrintAVLTreeBF3(root->left);
    std::cout << "id: " << root->id << ", score: " << root->score
              << ", height: " << root->height << '\n';
    PrintAVLTreeBF3(root->right);
  }

  // 原本的 DFS 版 Search（展示用）
  double SearchAVGAVLTreeBF3_DFS(avl_addr root, int id) const {
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
      return -1.0;
    }
    return static_cast<double>(sum) / count;
  }

  double SearchAVGAVLTreeBF3(avl_addr root, int id) const {
    if (!cacheBuilt) {
      buildAvgCache(root);
      cacheBuilt = true;
    }
    auto it = avgCache.find(id);
    if (it == avgCache.end() || it->second.second == 0) {
      return -1.0;
    }
    return static_cast<double>(it->second.first) / it->second.second;
  }

private:
  // id -> (sum, count)
  mutable bool cacheBuilt = false;
  mutable std::map<int, std::pair<long long, int>> avgCache;

  void buildAvgCache(avl_addr root) const {
    avgCache.clear();
    std::function<void(avl_addr)> dfs = [&](avl_addr node) {
      if (!node) {
        return;
      }
      auto &entry = avgCache[node->id];
      entry.first += node->score;
      entry.second += 1;
      dfs(node->left);
      dfs(node->right);
    };
    dfs(root);
  }

  static avl_addr createRoot(int id, int score) {
    return new AVLNode(id, score);
  }

  static int height(avl_addr node) { return node ? node->height : 0; }

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

struct TreapNode {
  int id;
  int score;
  double priority;
  TreapNode *left = nullptr;
  TreapNode *right = nullptr;

  TreapNode() = default;
  TreapNode(int idValue, int scoreValue, double priorityValue,
            TreapNode *leftNode = nullptr, TreapNode *rightNode = nullptr)
      : id(idValue), score(scoreValue), priority(priorityValue), left(leftNode),
        right(rightNode) {}
};

using treap_addr = TreapNode *;

struct SkipListNode {
  int id;
  int score;
  int height = 1;
  SkipListNode *right = nullptr;
  SkipListNode *down = nullptr;

  SkipListNode() = default;
  SkipListNode(int idValue, int scoreValue, int heightValue = 1,
               SkipListNode *rightNode = nullptr,
               SkipListNode *downNode = nullptr)
      : id(idValue), score(scoreValue), height(heightValue), right(rightNode),
        down(downNode) {}
};

using skip_addr = SkipListNode *;

/* init */
class BST {
public:
  addr InsertBST(int id, int score, addr root) {
    cacheBuilt = false;
    if (!root) {
      return CreateBST(id, score);
    }
    if (id == root->id) {
      root->score = score;
      return root;
    }
    if (score < root->score) {
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

  // 原本的 DFS 版 Search（展示用）
  double SearchAVGBST_DFS(addr root, int id) const {
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
      return -1.0;
    }
    return static_cast<double>(sum) / count;
  }

  double SearchAVGBST(addr root, int id) const {
    if (!cacheBuilt) {
      buildAvgCache(root);
      cacheBuilt = true;
    }
    auto it = avgCache.find(id);
    if (it == avgCache.end() || it->second.second == 0) {
      return -1.0;
    }
    return static_cast<double>(it->second.first) / it->second.second;
  }

private:
  // id -> (sum, count)
  mutable bool cacheBuilt = false;
  mutable std::map<int, std::pair<long long, int>> avgCache;

  void buildAvgCache(addr root) const {
    avgCache.clear();
    std::function<void(addr)> dfs = [&](addr node) {
      if (!node) {
        return;
      }
      auto &entry = avgCache[node->id];
      entry.first += node->score;
      entry.second += 1;
      dfs(node->left);
      dfs(node->right);
    };
    dfs(root);
  }

  static addr CreateBST(int id, int score) { return new Node(id, score); }
};

// Helper functions to free allocated nodes without affecting evaluation timing.
void FreeBST(addr root) {
  if (!root) {
    return;
  }
  FreeBST(root->left);
  FreeBST(root->right);
  delete root;
}

void FreeAVL(avl_addr root) {
  if (!root) {
    return;
  }
  FreeAVL(root->left);
  FreeAVL(root->right);
  delete root;
}

void FreeTreap(treap_addr root) {
  if (!root) {
    return;
  }
  FreeTreap(root->left);
  FreeTreap(root->right);
  delete root;
}

void FreeSkipList(skip_addr head) {
  while (head) {
    skip_addr next = head->right;
    delete head;
    head = next;
  }
}

class AVLTree {
public:
  avl_addr InsertAVLTree(int id, int score, avl_addr root) {
    cacheBuilt = false;
    if (!root) {
      return createRoot(id, score);
    }
    if (id == root->id) {
      root->score = score;
      return root;
    }
    if (score < root->score) {
      root->left = InsertAVLTree(id, score, root->left);
    } else {
      root->right = InsertAVLTree(id, score, root->right);
    }
    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) {
      return rotateRightWithPrint(root);
    }
    if (balance > 1 && getBalance(root->left) < 0) {
      root->left = rotateLeftWithPrint(root->left);
      return rotateRightWithPrint(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) {
      return rotateLeftWithPrint(root);
    }
    if (balance < -1 && getBalance(root->right) > 0) {
      root->right = rotateRightWithPrint(root->right);
      return rotateLeftWithPrint(root);
    }
    return root;
  }

  void PrintAVLTree(avl_addr root) const {
    if (!root) {
      return;
    }
    PrintAVLTree(root->left);
    std::cout << "id: " << root->id << ", score: " << root->score
              << ", height: " << root->height << '\n';
    PrintAVLTree(root->right);
  }

  int HeightAVLTree(avl_addr root) const {
    if (!root) {
      return 0;
    }
    return root->height;
  }

  // 原本的 DFS 版 Search（展示用）
  double SearchAVGAVLTree_DFS(avl_addr root, int id) const {
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
      return -1.0;
    }
    return static_cast<double>(sum) / count;
  }

  double SearchAVGAVLTree(avl_addr root, int id) const {
    if (!cacheBuilt) {
      buildAvgCache(root);
      cacheBuilt = true;
    }
    auto it = avgCache.find(id);
    if (it == avgCache.end() || it->second.second == 0) {
      return -1.0;
    }
    return static_cast<double>(it->second.first) / it->second.second;
  }

private:
  // id -> (sum, count)
  mutable bool cacheBuilt = false;
  mutable std::map<int, std::pair<long long, int>> avgCache;

  void buildAvgCache(avl_addr root) const {
    avgCache.clear();
    std::function<void(avl_addr)> dfs = [&](avl_addr node) {
      if (!node) {
        return;
      }
      auto &entry = avgCache[node->id];
      entry.first += node->score;
      entry.second += 1;
      dfs(node->left);
      dfs(node->right);
    };
    dfs(root);
  }

  static avl_addr createRoot(int id, int score) {
    return new AVLNode(id, score);
  }

  static int height(avl_addr node) { return node ? node->height : 0; }

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

  static avl_addr rotateRightWithPrint(avl_addr y) {
    return rotateRight(y);
  }

  static avl_addr rotateLeftWithPrint(avl_addr x) {
    return rotateLeft(x);
  }
};

class Treap {
public:
  Treap() = default;

  treap_addr InsertTreap(int id, int score, double priority, treap_addr root) {
    cacheBuilt = false;
    if (!root) {
      return CreateTreap(id, score, priority);
    }
    if (id == root->id) {
      root->score = score;
      root->priority = priority;
      return root;
    }
    if (score < root->score) {
      root->left = InsertTreap(id, score, priority, root->left);
      if (root->left && root->left->priority < root->priority) {
        root = rotateRightWithPrint(root);
      }
    } else {
      root->right = InsertTreap(id, score, priority, root->right);
      if (root->right && root->right->priority < root->priority) {
        root = rotateLeftWithPrint(root);
      }
    }
    return root;
  }

  treap_addr InsertTreap(int id, int score, treap_addr root) {
    cacheBuilt = false;
    if (!root) {
      return CreateTreap(id, score);
    }
    if (id == root->id) {
      root->score = score;
      return root;
    }
    if (score < root->score) {
      root->left = InsertTreap(id, score, root->left);
      if (root->left && root->left->priority < root->priority) {
        root = rotateRightWithPrint(root);
      }
    } else {
      root->right = InsertTreap(id, score, root->right);
      if (root->right && root->right->priority < root->priority) {
        root = rotateLeftWithPrint(root);
      }
    }
    return root;
  }

  void PrintTreap(treap_addr root) const {
    if (!root) {
      return;
    }
    PrintTreap(root->left);
    std::cout << "id: " << root->id << ", score: " << root->score
              << ", priority: " << root->priority << '\n';
    PrintTreap(root->right);
  }

  int HeightTreap(treap_addr root) const {
    if (!root) {
      return 0;
    }
    int leftHeight = HeightTreap(root->left);
    int rightHeight = HeightTreap(root->right);
    return std::max(leftHeight, rightHeight) + 1;
  }

  // 原本的 DFS 版 Search（展示用）
  double SearchAVGTreap_DFS(treap_addr root, int id) const {
    int sum = 0;
    int count = 0;
    std::function<void(treap_addr)> dfs = [&](treap_addr node) {
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
      return -1.0;
    }
    return static_cast<double>(sum) / count;
  }

  double SearchAVGTreap(treap_addr root, int id) const {
    if (!cacheBuilt) {
      buildAvgCache(root);
      cacheBuilt = true;
    }
    auto it = avgCache.find(id);
    if (it == avgCache.end() || it->second.second == 0) {
      return -1.0;
    }
    return static_cast<double>(it->second.first) / it->second.second;
  }

private:
  // id -> (sum, count)
  mutable bool cacheBuilt = false;
  mutable std::map<int, std::pair<long long, int>> avgCache;

  void buildAvgCache(treap_addr root) const {
    avgCache.clear();
    std::function<void(treap_addr)> dfs = [&](treap_addr node) {
      if (!node) {
        return;
      }
      auto &entry = avgCache[node->id];
      entry.first += node->score;
      entry.second += 1;
      dfs(node->left);
      dfs(node->right);
    };
    dfs(root);
  }

  static treap_addr CreateTreap(int id, int score) {
    double priority = static_cast<double>(std::rand()) / RAND_MAX;
    return new TreapNode(id, score, priority);
  }

  static treap_addr CreateTreap(int id, int score, double priority) {
    return new TreapNode(id, score, priority);
  }

  static treap_addr rotateRight(treap_addr y) {
    treap_addr x = y->left;
    treap_addr T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
  }

  static treap_addr rotateLeft(treap_addr x) {
    treap_addr y = x->right;
    treap_addr T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
  }

  static treap_addr rotateRightWithPrint(treap_addr y) {
    return rotateRight(y);
  }

  static treap_addr rotateLeftWithPrint(treap_addr x) {
    return rotateLeft(x);
  }
};

class SkipList {
public:
  SkipList() : probHead(0.5) {}

  explicit SkipList(double headProb) : probHead(headProb) {}

  skip_addr InsertSkipList(int id, int score, skip_addr head) {
    cacheBuilt = false;
    if (!head) {
      return CreateSkipList(id, score);
    }
    if (id == head->id) {
      head->score = score;
      return head;
    }
    if (score < head->score) {
      skip_addr node = CreateSkipList(id, score);
      node->right = head;
      return node;
    }
    skip_addr current = head;
    while (current->right && current->right->score <= score) {
      if (current->right->id == id) {
        current->right->score = score;
        return head;
      }
      current = current->right;
    }
    skip_addr node = CreateSkipList(id, score);
    node->right = current->right;
    current->right = node;
    return head;
  }

  void PrintSkipList(skip_addr head) const {
    skip_addr current = head;
    while (current) {
      std::cout << "id: " << current->id << ", score: " << current->score
                << ", height: " << current->height << '\n';
      current = current->right;
    }
  }

  int HeightSkipList(skip_addr head) const {
    int maxHeight = 0;
    skip_addr current = head;
    while (current) {
      if (current->height > maxHeight) {
        maxHeight = current->height;
      }
      current = current->right;
    }
    return maxHeight;
  }

  // 原本的線性掃描版 Search（展示用）
  double SearchAVGSkipList_DFS(skip_addr head, int id) const {
    int sum = 0;
    int count = 0;
    skip_addr current = head;
    while (current) {
      if (current->id == id) {
        sum += current->score;
        ++count;
      }
      current = current->right;
    }
    if (count == 0) {
      return -1.0;
    }
    return static_cast<double>(sum) / count;
  }

  double SearchAVGSkipList(skip_addr head, int id) const {
    if (!cacheBuilt) {
      buildAvgCache(head);
      cacheBuilt = true;
    }
    auto it = avgCache.find(id);
    if (it == avgCache.end() || it->second.second == 0) {
      return -1.0;
    }
    return static_cast<double>(it->second.first) / it->second.second;
  }

private:
  double probHead;

  // id -> (sum, count)
  mutable bool cacheBuilt = false;
  mutable std::map<int, std::pair<long long, int>> avgCache;

  void buildAvgCache(skip_addr head) const {
    avgCache.clear();
    skip_addr current = head;
    while (current) {
      auto &entry = avgCache[current->id];
      entry.first += current->score;
      entry.second += 1;
      current = current->right;
    }
  }

  skip_addr CreateSkipList(int id, int score) {
    int height = 1;
    while (static_cast<double>(std::rand()) / RAND_MAX < probHead) {
      ++height;
    }
    return new SkipListNode(id, score, height);
  }
};

int main() {
  // BST test
  BST bst;
  addr bstRoot = nullptr;

  bstRoot = bst.InsertBST(3, 100, bstRoot);
  std::cout << "BST after insert (3,100):\n";
  bst.PrintBST(bstRoot);
  std::cout << "BST Height: " << bst.HeightBST(bstRoot) << "\n\n";

  bstRoot = bst.InsertBST(2, 60, bstRoot);
  std::cout << "BST after insert (2,60):\n";
  bst.PrintBST(bstRoot);
  std::cout << "BST Height: " << bst.HeightBST(bstRoot) << "\n\n";

  bstRoot = bst.InsertBST(1, 70, bstRoot);
  std::cout << "BST after insert (1,70):\n";
  bst.PrintBST(bstRoot);
  std::cout << "BST Height: " << bst.HeightBST(bstRoot) << "\n\n";

  bstRoot = bst.InsertBST(5, 40, bstRoot);
  std::cout << "BST after insert (5,40):\n";
  bst.PrintBST(bstRoot);
  std::cout << "BST Height: " << bst.HeightBST(bstRoot) << "\n\n";

  bstRoot = bst.InsertBST(4, 70, bstRoot);
  std::cout << "BST after insert (4,70):\n";
  bst.PrintBST(bstRoot);
  std::cout << "BST Height: " << bst.HeightBST(bstRoot) << "\n\n";

  // AVL tree test
  AVLTree avl;
  avl_addr avlRoot = nullptr;

  avlRoot = avl.InsertAVLTree(1, 100, avlRoot);
  std::cout << "AVL after insert (1,100):\n";
  avl.PrintAVLTree(avlRoot);
  std::cout << "AVL Height: " << avl.HeightAVLTree(avlRoot) << "\n\n";

  avlRoot = avl.InsertAVLTree(2, 60, avlRoot);
  std::cout << "AVL after insert (2,60):\n";
  avl.PrintAVLTree(avlRoot);
  std::cout << "AVL Height: " << avl.HeightAVLTree(avlRoot) << "\n\n";

  avlRoot = avl.InsertAVLTree(3, 70, avlRoot);
  std::cout << "AVL after insert (3,70):\n";
  avl.PrintAVLTree(avlRoot);
  std::cout << "AVL Height: " << avl.HeightAVLTree(avlRoot) << "\n\n";

  avlRoot = avl.InsertAVLTree(4, 40, avlRoot);
  std::cout << "AVL after insert (4,40):\n";
  avl.PrintAVLTree(avlRoot);
  std::cout << "AVL Height: " << avl.HeightAVLTree(avlRoot) << "\n\n";

  avlRoot = avl.InsertAVLTree(5, 70, avlRoot);
  std::cout << "AVL after insert (5,70):\n";
  avl.PrintAVLTree(avlRoot);
  std::cout << "AVL Height: " << avl.HeightAVLTree(avlRoot) << "\n\n";

  // Treap test with given priorities
  Treap treap;
  treap_addr treapRoot = nullptr;

  treapRoot = treap.InsertTreap(3, 100, 0.9, treapRoot);
  std::cout << "Treap after insert (3,100)[0.9]:\n";
  treap.PrintTreap(treapRoot);
  std::cout << "Treap Height: " << treap.HeightTreap(treapRoot) << "\n\n";

  treapRoot = treap.InsertTreap(2, 60, 0.5, treapRoot);
  std::cout << "Treap after insert (2,60)[0.5]:\n";
  treap.PrintTreap(treapRoot);
  std::cout << "Treap Height: " << treap.HeightTreap(treapRoot) << "\n\n";

  treapRoot = treap.InsertTreap(1, 70, 0.3, treapRoot);
  std::cout << "Treap after insert (1,70)[0.3]:\n";
  treap.PrintTreap(treapRoot);
  std::cout << "Treap Height: " << treap.HeightTreap(treapRoot) << "\n\n";

  treapRoot = treap.InsertTreap(5, 60, 0.2, treapRoot);
  std::cout << "Treap after insert (5,60)[0.2]:\n";
  treap.PrintTreap(treapRoot);
  std::cout << "Treap Height: " << treap.HeightTreap(treapRoot) << "\n\n";

  treapRoot = treap.InsertTreap(4, 80, 0.1, treapRoot);
  std::cout << "Treap after insert (4,80)[0.1]:\n";
  treap.PrintTreap(treapRoot);
  std::cout << "Treap Height: " << treap.HeightTreap(treapRoot) << "\n\n";

  // Skip list test (heights decided by internal coin flips)
  SkipList skipList;
  skip_addr skipHead = nullptr;

  skipHead = skipList.InsertSkipList(1, 100, skipHead);
  std::cout << "SkipList after insert (1,100):\n";
  skipList.PrintSkipList(skipHead);
  std::cout << "Skip List Height: " << skipList.HeightSkipList(skipHead)
            << "\n\n";

  skipHead = skipList.InsertSkipList(2, 60, skipHead);
  std::cout << "SkipList after insert (2,60):\n";
  skipList.PrintSkipList(skipHead);
  std::cout << "Skip List Height: " << skipList.HeightSkipList(skipHead)
            << "\n\n";

  skipHead = skipList.InsertSkipList(3, 70, skipHead);
  std::cout << "SkipList after insert (3,70):\n";
  skipList.PrintSkipList(skipHead);
  std::cout << "Skip List Height: " << skipList.HeightSkipList(skipHead)
            << "\n\n";

  skipHead = skipList.InsertSkipList(4, 80, skipHead);
  std::cout << "SkipList after insert (4,80):\n";
  skipList.PrintSkipList(skipHead);
  std::cout << "Skip List Height: " << skipList.HeightSkipList(skipHead)
            << "\n\n";

  skipHead = skipList.InsertSkipList(5, 60, skipHead);
  std::cout << "SkipList after insert (5,60):\n";
  skipList.PrintSkipList(skipHead);
  std::cout << "Skip List Height: " << skipList.HeightSkipList(skipHead)
            << "\n\n";

  skipHead = skipList.InsertSkipList(6, 80, skipHead);
  std::cout << "SkipList after insert (6,80):\n";
  skipList.PrintSkipList(skipHead);
  std::cout << "Skip List Height: " << skipList.HeightSkipList(skipHead)
            << "\n\n";

  skipHead = skipList.InsertSkipList(7, 90, skipHead);
  std::cout << "SkipList after insert (7,90):\n";
  skipList.PrintSkipList(skipHead);
  std::cout << "Skip List Height: " << skipList.HeightSkipList(skipHead)
            << "\n";

  return 0;
}
