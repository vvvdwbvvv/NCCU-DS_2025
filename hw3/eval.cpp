#include <iostream>
#include <chrono>
#include <random>
#include <map>
#include <unordered_map>
#include <vector>
#include "main.cpp"
#undef main

using namespace std;

void InsertBST(int id, int score);
double SearchAVGBST(int id);
void InsertHT(int id, int score);
double SearchAVGHT(int id);

extern std::map<int, std::vector<int>> bstMap;
extern std::unordered_map<int, std::vector<int>> htMap;

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int trials = 10;

    mt19937 rng(123);
    uniform_int_distribution<int> distId(1, 1 << 20);
    uniform_int_distribution<int> distScore(0, 100);
    uniform_int_distribution<int> distQueryId(1, 1 << 20);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " 1|2\n";
        cerr << "  1: output insertion-time CSV (Figure 1)\n";
        cerr << "  2: output search-time CSV (Figure 2)\n";
        return 1;
    }

    int mode = argv[1][0] - '0';

    if (mode == 1) {
        // Figure 1: insertion time CSV
        cout << "n,BST_insert,HT_insert\n";

        for (int exp = 10; exp <= 20; ++exp) {
        int n = 1 << exp;
        long long bstInsertSum = 0;
        long long htInsertSum = 0;

        for (int t = 0; t < trials; ++t) {
            bstMap.clear();
            htMap.clear();

            auto startBST = chrono::high_resolution_clock::now();
            for (int i = 0; i < n; ++i) {
                int id = distId(rng);
                int score = distScore(rng);
                InsertBST(id, score);
            }
            auto endBST = chrono::high_resolution_clock::now();

            auto startHT = chrono::high_resolution_clock::now();
            for (int i = 0; i < n; ++i) {
                int id = distId(rng);
                int score = distScore(rng);
                InsertHT(id, score);
            }
            auto endHT = chrono::high_resolution_clock::now();

            bstInsertSum += chrono::duration_cast<chrono::nanoseconds>(endBST - startBST).count();
            htInsertSum += chrono::duration_cast<chrono::nanoseconds>(endHT - startHT).count();
        }

        double bstInsertAvg = static_cast<double>(bstInsertSum) / trials;
        double htInsertAvg = static_cast<double>(htInsertSum) / trials;

        cout << n << "," << bstInsertAvg << "," << htInsertAvg << "\n";
        }
    } else if (mode == 2) {
        // Figure 2: search time CSV
        cout << "n,BST_search,HT_search\n";

        for (int exp = 10; exp <= 20; ++exp) {
        int n = 1 << exp;
        long long bstSearchSum = 0;
        long long htSearchSum = 0;

        for (int t = 0; t < trials; ++t) {
            bstMap.clear();
            htMap.clear();

            for (int i = 0; i < n; ++i) {
                int id = distId(rng);
                int score = distScore(rng);
                InsertBST(id, score);
                InsertHT(id, score);
            }

            const int queryTimes = 1000;

            auto startBST = chrono::high_resolution_clock::now();
            for (int q = 0; q < queryTimes; ++q) {
                int id = distQueryId(rng);
                (void)SearchAVGBST(id);
            }
            auto endBST = chrono::high_resolution_clock::now();

            auto startHT = chrono::high_resolution_clock::now();
            for (int q = 0; q < queryTimes; ++q) {
                int id = distQueryId(rng);
                (void)SearchAVGHT(id);
            }
            auto endHT = chrono::high_resolution_clock::now();

            bstSearchSum += chrono::duration_cast<chrono::nanoseconds>(endBST - startBST).count();
            htSearchSum += chrono::duration_cast<chrono::nanoseconds>(endHT - startHT).count();
        }

        double bstSearchAvg = static_cast<double>(bstSearchSum) / trials;
        double htSearchAvg = static_cast<double>(htSearchSum) / trials;

        cout << n << "," << bstSearchAvg << "," << htSearchAvg << "\n";
        }
    } else {
        cerr << "Invalid mode. Use 1 or 2.\n";
        return 1;
    }

    return 0;
}
