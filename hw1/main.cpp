#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <memory>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
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
  vector<int> ks(15);
  for (int i = 0; i < 15; ++i) ks[i] = 11 + i;
  vector<string> types{"DS1", "DS2", "DS3"};
  ofstream out("results.csv");
  out << "Type,k,n,insert,search100k,sum,estimated\n";
  for (const string& type : types) {
    cout << "Type: " << type << endl;
    double ins_power, srch_power, sum_power;
    if (type == "DS1") {
      ins_power = 2.0;
      srch_power = 1.0;
      sum_power = 1.0;
    } else if (type == "DS2") {
      ins_power = 1.0;
      srch_power = 1.0;
      sum_power = 1.0;
    } else {
      ins_power = 1.0;
      srch_power = 0.0;
      sum_power = 1.0;
    }
    double prev_k = -1;
    double prev_avg_ins = 0, prev_avg_srch = 0, prev_avg_sum = 0;
    bool prev_exceeded = false;
    for (int k : ks) {
      int n = 1 << k;
      int delta = k - prev_k;
      double scale_ins = (prev_k < 0) ? 1.0 : pow(2.0, ins_power * delta);
      double pred_avg_ins = prev_avg_ins * scale_ins;
      double pred_total_ins_time = pred_avg_ins * 10;
      bool use_estimate = prev_exceeded;

      if (use_estimate) {
        double scale_srch = pow(2.0, srch_power * delta);
        double scale_sum = pow(2.0, sum_power * delta);
        double est_ins = prev_avg_ins * scale_ins;
        double est_srch = prev_avg_srch * scale_srch;
        double est_sum = prev_avg_sum * scale_sum;
        cout << "  k=" << k << ", n=" << n << ", estimated insert=" << est_ins << "s, search100k=" << est_srch << "s, sum=" << est_sum << "s (skipped)\n";
        out << type << "," << k << "," << n << "," << est_ins << "," << est_srch << "," << est_sum << ",1\n";
      } else {
        double ins_time = 0, srch_time = 0, sum_time = 0;
        for (int trial = 0; trial < 10; ++trial) {
          unique_ptr<BaseDS> ds;
          if (type == "DS1") ds.reset(new DS1());
          else if (type == "DS2") ds.reset(new DS2());
          else ds.reset(new DS3());
          mt19937 rng(random_device{}() + trial);
          uniform_int_distribution<int> id_d(1, 1 << 20);
          uniform_int_distribution<int> sc_d(0, 100);
          auto start = chrono::steady_clock::now();
          for (int i = 0; i < n; ++i) ds->insert(id_d(rng), sc_d(rng));
          auto end = chrono::steady_clock::now();
          ins_time += chrono::duration<double>(end - start).count();
          vector<int> srch_ids(100000);
          for (int& sid : srch_ids) sid = id_d(rng);
          start = chrono::steady_clock::now();
          for (int sid : srch_ids) {
            volatile auto res = ds->search(sid);
          }
          end = chrono::steady_clock::now();
          srch_time += chrono::duration<double>(end - start).count();
          start = chrono::steady_clock::now();
          volatile long long total = ds->sum_scores();
          end = chrono::steady_clock::now();
          sum_time += chrono::duration<double>(end - start).count();
        }
        double avg_ins = ins_time / 10;
        double avg_srch = srch_time / 10;
        double avg_sum = sum_time / 10;
        cout << "  k=" << k << ", n=" << n << ", insert=" << avg_ins << "s, search100k=" << avg_srch << "s, sum=" << avg_sum << "s\n";
        out << type << "," << k << "," << n << "," << avg_ins << "," << avg_srch << "," << avg_sum << ",0\n";
        prev_k = k;
        prev_avg_ins = avg_ins;
        prev_avg_srch = avg_srch;
        prev_avg_sum = avg_sum;
        prev_exceeded = (avg_ins * 10 > 600);
      }
    }
  }
  return 0;
}