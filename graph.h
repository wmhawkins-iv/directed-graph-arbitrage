#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <vector>
#include <queue>
#include <map>

using namespace std;


struct CompareVectSize {
    bool operator()(vector<int> const& l1, vector<int> const& l2) {
        return l1.size() < l2.size();
    }
};

class Graph {
  vector<vector<int>> AK;
  vector<list<int>> B;
  vector<bool> blocked;
  vector<int> stack;
  int S;
  int N;
  bool simpleCyclesUpdate;
  map<int,int> toNodes;
  map<int,int> toIndex;
  map<pair<int,int>,double> edgeLabels;
  priority_queue<vector<int>, vector<vector<int>>, CompareVectSize> simpleCycles;

  void unblock(int U);
  bool cycle(int V);
  void outputCycle();

public:
  void printGraph();
  void printSimpleCycles();
  void reverseGraph();
  void getJohnsonSimpleCycles();
  void getLongestCycleLengthPerNode();
  void getMaxArbitrage();

  Graph(vector<vector<int>> AK, map<int,int>& toNodes, map<int,int>& toIndex,
        map<pair<int,int>,double>& edgeLabels)
  {
    this->AK = AK;
    this->toNodes = toNodes;
    this->toIndex = toIndex;
    this->N = AK.size();
    this->edgeLabels = edgeLabels;
    this->simpleCyclesUpdate = true;
    blocked.resize(N);
    B.resize(N);
  }
};

#endif
