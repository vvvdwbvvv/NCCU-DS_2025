#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

// Reuse data structure definitions from main.cpp but ignore its main()
#define main main_unused_for_eval
#include "main.cpp"
#undef main

using Clock = std::chrono::high_resolution_clock;
using Microseconds = std::chrono::duration<double, std::micro>;

struct DataItem {
  int id;
  int score;
};

int main() {
  std::cout << "start\n";
  std::mt19937 rng(123456);
  std::uniform_int_distribution<int> distId(1, (1 << 20));
  std::uniform_int_distribution<int> distScore(0, 100);
  std::uniform_real_distribution<double> distPriority(0.0, 1.0);

  // Ns: problem sizes to test, here 2^11 ~ 2^25.
  const std::vector<int> Ns = [] {
    std::vector<int> v;
    for (int k = 10; k <= 20; ++k) {
      v.push_back(1 << k);
    }
    return v;
  }();
  const int numTrials = 10;

  // Figure 1: average insert time per element (microseconds)
  std::cout << "Figure 1: measuring insert time for BST, AVL, Treap, SkipList(p=0.5)\n";
  std::ofstream fig1("evals/fig1_insert_time.csv");
  fig1 << "n,BST_us_per_insert,AVL_us_per_insert,Treap_us_per_insert,SkipList_"
          "p0.5_us_per_insert\n";

  for (int n : Ns) {
    double bstSum = 0.0;
    double avlSum = 0.0;
    double treapSum = 0.0;
    double skipSum = 0.0;

    for (int t = 0; t < numTrials; ++t) {
      std::vector<DataItem> data(n);
      for (int i = 0; i < n; ++i) {
        data[i].id = distId(rng);
        data[i].score = distScore(rng);
      }

      // BST所以我其實
      {
        std::cout << "Figure 1: n=" << n << ", trial=" << t
                  << " - BST insert\n";
        BST bst;
        addr root = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          root = bst.InsertBST(item.id, item.score, root);
        }
        auto end = Clock::now();
        bstSum += Microseconds(end - start).count() / n;
        FreeBST(root);
      }

      // AVL (silent insert)
      {
        std::cout << "Figure 1: n=" << n << ", trial=" << t
                  << " - AVL insert\n";
        AVLTree avl;
        avl_addr root = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          root = avl.InsertAVLTree(item.id, item.score, root);
        }
        auto end = Clock::now();
        avlSum += Microseconds(end - start).count() / n;
        FreeAVL(root);
      }

      // Treap (min-heap on priority)
      {
        std::cout << "Figure 1: n=" << n << ", trial=" << t
                  << " - Treap insert\n";
        Treap treap;
        treap_addr root = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          double p = distPriority(rng);
          root = treap.InsertTreap(item.id, item.score, p, root);
        }
        auto end = Clock::now();
        treapSum += Microseconds(end - start).count() / n;
        FreeTreap(root);
      }

      // Skip list with pr[H] = 0.5
      {
        std::cout << "Figure 1: n=" << n << ", trial=" << t
                  << " - SkipList(p=0.5) insert\n";
        SkipList skipList(0.5);
        skip_addr head = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        auto end = Clock::now();
        skipSum += Microseconds(end - start).count() / n;
        FreeSkipList(head);
      }
    }

    fig1 << n << ',' << (bstSum / numTrials) << ',' << (avlSum / numTrials)
         << ',' << (treapSum / numTrials) << ',' << (skipSum / numTrials)
         << '\n';
  }

  fig1.close();

  // Figure 2: average SearchAVGXXX time per query (microseconds)
  std::cout << "Figure 2: measuring search time for BST, AVL, Treap, SkipList(p=0.5)\n";
  std::ofstream fig2("evals/sfig2_search_time.csv");
  fig2 << "n,BST_us_per_search,AVL_us_per_search,Treap_us_per_search,SkipList_"
          "p0.5_us_per_search\n";

  volatile double sink = 0.0; // prevent optimization away of search results

  for (int n : Ns) {
    double bstSum = 0.0;
    double avlSum = 0.0;
    double treapSum = 0.0;
    double skipSum = 0.0;

    for (int t = 0; t < numTrials; ++t) {
      std::vector<DataItem> data(n);
      std::vector<int> queries(n);
      for (int i = 0; i < n; ++i) {
        data[i].id = distId(rng);
        data[i].score = distScore(rng);
        queries[i] = distId(rng);
      }

      // BST
      {
        std::cout << "Figure 2: n=" << n << ", trial=" << t
                  << " - BST search\n";
        BST bst;
        addr root = nullptr;
        for (const auto &item : data) {
          root = bst.InsertBST(item.id, item.score, root);
        }
        auto start = Clock::now();
        for (int q : queries) {
          sink = bst.SearchAVGBST(root, q);
        }
        auto end = Clock::now();
        bstSum += Microseconds(end - start).count() / n;
        FreeBST(root);
      }

      // AVL
      {
        std::cout << "Figure 2: n=" << n << ", trial=" << t
                  << " - AVL search\n";
        AVLTree avl;
        avl_addr root = nullptr;
        for (const auto &item : data) {
          root = avl.InsertAVLTree(item.id, item.score, root);
        }
        auto start = Clock::now();
        for (int q : queries) {
          sink = avl.SearchAVGAVLTree(root, q);
        }
        auto end = Clock::now();
        avlSum += Microseconds(end - start).count() / n;
        FreeAVL(root);
      }

      // Treap
      {
        std::cout << "Figure 2: n=" << n << ", trial=" << t
                  << " - Treap search\n";
        Treap treap;
        treap_addr root = nullptr;
        for (const auto &item : data) {
          double p = distPriority(rng);
          root = treap.InsertTreap(item.id, item.score, p, root);
        }
        auto start = Clock::now();
        for (int q : queries) {
          sink = treap.SearchAVGTreap(root, q);
        }
        auto end = Clock::now();
        treapSum += Microseconds(end - start).count() / n;
        FreeTreap(root);
      }

      // Skip list p=0.5
      {
        std::cout << "Figure 2: n=" << n << ", trial=" << t
                  << " - SkipList(p=0.5) search\n";
        SkipList skipList(0.5);
        skip_addr head = nullptr;
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        auto start = Clock::now();
        for (int q : queries) {
          sink = skipList.SearchAVGSkipList(head, q);
        }
        auto end = Clock::now();
        skipSum += Microseconds(end - start).count() / n;
        FreeSkipList(head);
      }
    }

    fig2 << n << ',' << (bstSum / numTrials) << ',' << (avlSum / numTrials)
         << ',' << (treapSum / numTrials) << ',' << (skipSum / numTrials)
         << '\n';
  }

  fig2.close();

  // Figure 3: average height
  std::cout << "Figure 3: measuring height for BST, AVL, Treap, SkipList(p=0.5,p=0.75,p=0.25), AVL(BF<=3)\n";
  std::ofstream fig3("evals/fig3_height.csv");
  fig3 << "n,BST_height,AVL_height,Treap_height,SkipList_p0.5_height,"
          "SkipList_p0.75_height,SkipList_p0.25_height,AVL_BF3_height\n";

  for (int n : Ns) {
    double bstSum = 0.0;
    double avlSum = 0.0;
    double treapSum = 0.0;
    double skip05Sum = 0.0;
    double skip075Sum = 0.0;
    double skip025Sum = 0.0;
    double avlBF3Sum = 0.0;

    for (int t = 0; t < numTrials; ++t) {
      std::vector<DataItem> data(n);
      for (int i = 0; i < n; ++i) {
        data[i].id = distId(rng);
        data[i].score = distScore(rng);
      }

      // BST
      {
        std::cout << "Figure 3: n=" << n << ", trial=" << t
                  << " - BST height\n";
        BST bst;
        addr root = nullptr;
        for (const auto &item : data) {
          root = bst.InsertBST(item.id, item.score, root);
        }
        bstSum += bst.HeightBST(root);
        FreeBST(root);
      }

      // AVL
      {
        std::cout << "Figure 3: n=" << n << ", trial=" << t
                  << " - AVL height\n";
        AVLTree avl;
        avl_addr root = nullptr;
        for (const auto &item : data) {
          root = avl.InsertAVLTree(item.id, item.score, root);
        }
        avlSum += avl.HeightAVLTree(root);
        FreeAVL(root);
      }

      // AVL with |balance factor| <= 3
      {
        std::cout << "Figure 3: n=" << n << ", trial=" << t
                  << " - AVL(BF<=3) height\n";
        AVLTreeBF3 avlBF3;
        avl_addr root = nullptr;
        for (const auto &item : data) {
          root = avlBF3.InsertAVLTreeBF3(item.id, item.score, root);
        }
        avlBF3Sum += avlBF3.HeightAVLTreeBF3(root);
        FreeAVL(root);
      }

      // Treap
      {
        std::cout << "Figure 3: n=" << n << ", trial=" << t
                  << " - Treap height\n";
        Treap treap;
        treap_addr root = nullptr;
        for (const auto &item : data) {
          double p = distPriority(rng);
          root = treap.InsertTreap(item.id, item.score, p, root);
        }
        treapSum += treap.HeightTreap(root);
        FreeTreap(root);
      }

      // Skip list p=0.5
      {
        std::cout << "Figure 3: n=" << n << ", trial=" << t
                  << " - SkipList(p=0.5) height\n";
        SkipList skipList(0.5);
        skip_addr head = nullptr;
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        skip05Sum += skipList.HeightSkipList(head);
        FreeSkipList(head);
      }

      // Skip list p=0.75
      {
        std::cout << "Figure 3: n=" << n << ", trial=" << t
                  << " - SkipList(p=0.75) height\n";
        SkipList skipList(0.75);
        skip_addr head = nullptr;
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        skip075Sum += skipList.HeightSkipList(head);
        FreeSkipList(head);
      }

      // Skip list p=0.25
      {
        std::cout << "Figure 3: n=" << n << ", trial=" << t
                  << " - SkipList(p=0.25) height\n";
        SkipList skipList(0.25);
        skip_addr head = nullptr;
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        skip025Sum += skipList.HeightSkipList(head);
        FreeSkipList(head);
      }
    }

    double bstAvg = bstSum / numTrials;
    double avlAvg = avlSum / numTrials;
    double treapAvg = treapSum / numTrials;
    double skip05Avg = skip05Sum / numTrials;
    double skip075Avg = skip075Sum / numTrials;
    double skip025Avg = skip025Sum / numTrials;
    double avlBF3Avg = avlBF3Sum / numTrials;

    fig3 << n << ',' << bstAvg << ',' << avlAvg << ',' << treapAvg << ','
         << skip05Avg << ',' << skip075Avg << ',' << skip025Avg << ','
         << avlBF3Avg << '\n';
  }

  fig3.close();

  (void)sink; // silence unused warning
  std::cout << "Evaluation finished. CSV files written: "
               "fig1_insert_time.csv, fig2_search_time.csv, fig3_height.csv\n";
  return 0;
}
