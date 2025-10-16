#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <memory>
#include <random>
#include <chrono>
#include <fstream>
#include <set>
using namespace std;

struct BaseDS {
  virtual ~BaseDS() = default;
  virtual void insert(int id, int score) = 0;
  virtual vector<int> search(int id) const = 0;
  virtual long long sum_scores() const = 0;
};

class DS1 : public BaseDS {
  vector<pair<int, int>> data;
public:
  DS1() { data.reserve(1); }
  void insert(int id, int score) override {
    if (data.size() == data.capacity()) {
      size_t new_cap = data.capacity() * 10;
      if (new_cap < 1) new_cap = 1;
      data.reserve(new_cap);
    }
    auto it = lower_bound(data.begin(), data.end(), id, [](const pair<int, int>& p, int i) { return p.first < i; });
    data.insert(it, {id, score});
  }
  vector<int> search(int id) const override {
    auto lit = lower_bound(data.begin(), data.end(), id, [](const pair<int, int>& p, int i) { return p.first < i; });
    auto uit = upper_bound(lit, data.end(), id, [](int i, const pair<int, int>& p) { return i < p.first; });
    vector<int> res;
    for (auto it = lit; it != uit; ++it) res.push_back(it->second);
    return res.empty() ? vector<int>{-1} : res;
  }
  long long sum_scores() const override {
    long long s = 0;
    for (const auto& p : data) s += p.second;
    return s;
  }
};

class DS2 : public BaseDS {
  static const int MAX_ID = 1 << 20;
  vector<list<int>> array;
public:
  DS2() : array(MAX_ID + 1) {}
  void insert(int id, int score) override {
    array[id].push_back(score);
  }
  vector<int> search(int id) const override {
    const auto& l = array[id];
    return l.empty() ? vector<int>{-1} : vector<int>(l.begin(), l.end());
  }
  long long sum_scores() const override {
    long long s = 0;
    for (const auto& l : array) for (int sc : l) s += sc;
    return s;
  }
};

class DS3 : public BaseDS {
  struct Node {
    int id;
    vector<int> scores;
    Node* next;
    Node() : next(nullptr) {}
  };
  Node* head;
public:
  DS3() : head(nullptr) {}
  ~DS3() override {
    Node* cur = head;
    while (cur) {
      Node* nxt = cur->next;
      delete cur;
      cur = nxt;
    }
  }
  void insert(int id, int score) override {
    Node** pp = &head;
    while (*pp && (*pp)->id < id) pp = &((*pp)->next);
    if (*pp && (*pp)->id == id) {
      (*pp)->scores.push_back(score);
    } else {
      Node* newn = new Node;
      newn->id = id;
      newn->scores.push_back(score);
      newn->next = *pp;
      *pp = newn;
    }
  }
  vector<int> search(int id) const override {
    Node* cur = head;
    while (cur && cur->id < id) cur = cur->next;
    if (cur && cur->id == id) return cur->scores;
    return {-1};
  }
  long long sum_scores() const override {
    long long s = 0;
    Node* cur = head;
    while (cur) {
      for (int sc : cur->scores) s += sc;
      cur = cur->next;
    }
    return s;
  }
};

int main() {
    const int total_ops = 100000;
    
    // 不同操作比例: (insert%, search%, sum%)
    vector<tuple<int, int, int, string>> workloads = {
        {70, 30, 0, "Write-Heavy"},      // 寫入為主
        {30, 70, 0, "Read-Heavy"},       // 讀取為主
        {33, 33, 34, "Balanced"},        // 均衡
        {10, 85, 5, "Query-Intensive"},  // 查詢密集
        {50, 40, 10, "Mixed-Analytics"}  // 混合分析
    };
    
    ofstream out("mixed_ops_results.csv");
    out << "Workload,Type,TotalTime,AvgOpTime_us,Throughput_ops_per_sec\n";
    
    for (auto [ins_pct, srch_pct, sum_pct, name] : workloads) {
        cout << "\nWorkload: " << name << " (I:" << ins_pct 
             << "% S:" << srch_pct << "% Sum:" << sum_pct << "%)\n";
        
        for (const string& type : {"DS1", "DS2", "DS3"}) {
            double total_time = 0;
            
            for (int trial = 0; trial < 5; ++trial) {
                unique_ptr<BaseDS> ds;
                if (type == "DS1") ds.reset(new DS1());
                else if (type == "DS2") ds.reset(new DS2());
                else ds.reset(new DS3());
                
                mt19937 rng(trial);
                uniform_int_distribution<int> id_dist(1, 1 << 20);
                uniform_int_distribution<int> score_dist(0, 100);
                uniform_int_distribution<int> op_dist(1, 100);
                
                auto start = chrono::steady_clock::now();
                
                for (int i = 0; i < total_ops; ++i) {
                    int op = op_dist(rng);
                    
                    if (op <= ins_pct) {
                        ds->insert(id_dist(rng), score_dist(rng));
                    } else if (op <= ins_pct + srch_pct) {
                        volatile auto res = ds->search(id_dist(rng));
                    } else {
                        volatile long long s = ds->sum_scores();
                    }
                }
                
                auto end = chrono::steady_clock::now();
                total_time += chrono::duration<double>(end - start).count();
            }
            
            double avg_time = total_time / 5;
            double avg_op_time_us = (avg_time / total_ops) * 1e6;
            double throughput = total_ops / avg_time;
            
            cout << "  " << type << ": " << avg_time << "s, " 
                 << throughput << " ops/sec\n";
            
            out << name << "," << type << "," << avg_time << "," 
                << avg_op_time_us << "," << throughput << "\n";
        }
    }
    
    return 0;
}