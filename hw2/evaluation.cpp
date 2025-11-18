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
  std::mt19937 rng(123456);
  std::uniform_int_distribution<int> distId(1, (1 << 20));
  std::uniform_int_distribution<int> distScore(0, 100);
  std::uniform_real_distribution<double> distPriority(0.0, 1.0);

  // Smaller settings for quick correctness validation; you can increase later.
  const std::vector<int> Ns = {100, 500};
  const int numTrials = 3;

  // Figure 1: average insert time per element (microseconds)
  std::ofstream fig1("eval/fig1_insert_time.csv");
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

      // BST
      {
        BST bst;
        addr root = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          root = bst.InsertBST(item.id, item.score, root);
        }
        auto end = Clock::now();
        bstSum += Microseconds(end - start).count() / n;
      }

      // AVL (silent insert)
      {
        AVLTree avl;
        avl_addr root = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          root = avl.InsertAVLTree(item.id, item.score, root);
        }
        auto end = Clock::now();
        avlSum += Microseconds(end - start).count() / n;
      }

      // Treap (min-heap on priority)
      {
        Treap treap;
        treap_addr root = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          double p = distPriority(rng);
          root = treap.InsertTreap(item.id, item.score, p, root);
        }
        auto end = Clock::now();
        treapSum += Microseconds(end - start).count() / n;
      }

      // Skip list with pr[H] = 0.5
      {
        SkipList skipList(0.5);
        skip_addr head = nullptr;
        auto start = Clock::now();
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        auto end = Clock::now();
        skipSum += Microseconds(end - start).count() / n;
      }
    }

    fig1 << n << ',' << (bstSum / numTrials) << ',' << (avlSum / numTrials)
         << ',' << (treapSum / numTrials) << ',' << (skipSum / numTrials)
         << '\n';
  }

  fig1.close();

  // Figure 2: average SearchAVGXXX time per query (microseconds)
  std::ofstream fig2("eval/sfig2_search_time.csv");
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
      }

      // AVL
      {
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
      }

      // Treap
      {
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
      }

      // Skip list p=0.5
      {
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
      }
    }

    fig2 << n << ',' << (bstSum / numTrials) << ',' << (avlSum / numTrials)
         << ',' << (treapSum / numTrials) << ',' << (skipSum / numTrials)
         << '\n';
  }

  fig2.close();

  // Figure 3: average height
  std::ofstream fig3("eval/fig3_height.csv");
  fig3 << "n,BST_height,AVL_height,Treap_height,SkipList_p0.5_height,"
          "SkipList_p0.75_height,SkipList_p0.25_height,AVL_theory_height\n";

  for (int n : Ns) {
    double bstSum = 0.0;
    double avlSum = 0.0;
    double treapSum = 0.0;
    double skip05Sum = 0.0;
    double skip075Sum = 0.0;
    double skip025Sum = 0.0;

    for (int t = 0; t < numTrials; ++t) {
      std::vector<DataItem> data(n);
      for (int i = 0; i < n; ++i) {
        data[i].id = distId(rng);
        data[i].score = distScore(rng);
      }

      // BST
      {
        BST bst;
        addr root = nullptr;
        for (const auto &item : data) {
          root = bst.InsertBST(item.id, item.score, root);
        }
        bstSum += bst.HeightBST(root);
      }

      // AVL
      {
        AVLTree avl;
        avl_addr root = nullptr;
        for (const auto &item : data) {
          root = avl.InsertAVLTree(item.id, item.score, root);
        }
        avlSum += avl.HeightAVLTree(root);
      }

      // Treap
      {
        Treap treap;
        treap_addr root = nullptr;
        for (const auto &item : data) {
          double p = distPriority(rng);
          root = treap.InsertTreap(item.id, item.score, p, root);
        }
        treapSum += treap.HeightTreap(root);
      }

      // Skip list p=0.5
      {
        SkipList skipList(0.5);
        skip_addr head = nullptr;
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        skip05Sum += skipList.HeightSkipList(head);
      }

      // Skip list p=0.75
      {
        SkipList skipList(0.75);
        skip_addr head = nullptr;
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        skip075Sum += skipList.HeightSkipList(head);
      }

      // Skip list p=0.25
      {
        SkipList skipList(0.25);
        skip_addr head = nullptr;
        for (const auto &item : data) {
          head = skipList.InsertSkipList(item.id, item.score, head);
        }
        skip025Sum += skipList.HeightSkipList(head);
      }
    }

    double bstAvg = bstSum / numTrials;
    double avlAvg = avlSum / numTrials;
    double treapAvg = treapSum / numTrials;
    double skip05Avg = skip05Sum / numTrials;
    double skip075Avg = skip075Sum / numTrials;
    double skip025Avg = skip025Sum / numTrials;

    // Approximate theoretical AVL height bound: 1.44 * log2(n + 2) - 0.328
    double avlTheory = 1.44 * std::log2(n + 2.0) - 0.328;

    fig3 << n << ',' << bstAvg << ',' << avlAvg << ',' << treapAvg << ','
         << skip05Avg << ',' << skip075Avg << ',' << skip025Avg << ','
         << avlTheory << '\n';
  }

  fig3.close();

  (void)sink; // silence unused warning
  std::cout << "Evaluation finished. CSV files written: "
               "fig1_insert_time.csv, fig2_search_time.csv, fig3_height.csv\n";
  return 0;
}
