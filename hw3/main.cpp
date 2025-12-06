#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>


std::map<int, std::vector<int>> bstMap;

void InsertBST(int id, int score){
    auto it = bstMap.find(id);
    if (it==bstMap.end()) {
        std::vector<int> vec;
        vec.push_back(score);
        bstMap[id] = vec;
    }
    else {
        it->second.push_back(score);
    }
}

double SearchAVGBST(int id) {
    auto it = bstMap.find(id);
    if (it == bstMap.end()) {
        return -1.0;
    }
    const std::vector<int>& v = it->second;
    if (v.empty()) {
        return -1.0;
    }

    long long sum = 0;
    for (int score : v) {
        sum += score;

    }
    return static_cast<double>(sum) / static_cast<double>(v.size());
}

std::unordered_map<int, std::vector<int>> htMap;

void InsertHT(int id, int score) {
    auto it = htMap.find(id);
    if(it == htMap.end()) {
        std::vector<int> vec;
        vec.push_back(score);
        htMap[id] = vec;
    } else {
        it -> second.push_back(score);
    }
}

double SearchAVGHT(int id) {
    auto it = htMap.find(id);
    if(it == htMap.end()) {
        return -1.0;
    }
    const std::vector<int>& v = it->second;
    if(v.empty()) {
        return -1.0;
    }
    long long sum = 0;
    for (int score : v) {
        sum += score;
    }
    return static_cast<double>(sum) / static_cast<double>(v.size());


}

void FunctionalTest() {
    std::cout << "==== Functional Test ====\n";

    // 插入一些測試資料
    InsertBST(10, 80);
    InsertBST(10, 90);
    InsertBST(20, 70);

    InsertHT(10, 80);
    InsertHT(10, 90);
    InsertHT(20, 70);

    // 查詢結果
    std::cout << "BST AVG 10 = " << SearchAVGBST(10) << "\n"; // (80+90)/2 = 85
    std::cout << "BST AVG 20 = " << SearchAVGBST(20) << "\n"; // 70
    std::cout << "BST AVG 30 = " << SearchAVGBST(30) << "\n"; // -1

    std::cout << "HT AVG 10  = " << SearchAVGHT(10) << "\n";
    std::cout << "HT AVG 20  = " << SearchAVGHT(20) << "\n";
    std::cout << "HT AVG 30  = " << SearchAVGHT(30) << "\n";
}
