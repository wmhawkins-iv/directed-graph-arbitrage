#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

#include "graph.h"

using namespace std;


void Graph::printGraph() {
  for (const auto e : edgeLabels) {
    cout << get<0>(e.first) << " " << get<1>(e.first) << " " << e.second << endl;
  }
}

void Graph::printSimpleCycles() {
  // Copy
  priority_queue<vector<int>, vector<vector<int>>, CompareVectSize> sc(simpleCycles);

  cout << "\nSimple Cycles:" << endl;
  while (!sc.empty()) {
    vector<int> c = sc.top();
    sc.pop();
    for (const auto i : c) {
      cout << toNodes[i] << "->";
    }
    cout << endl;
  }
}

// Johnson's unblock() procedure
void Graph::unblock(int u)
{
  blocked[u] = false;

  while (!B[u].empty()) {
    int w = B[u].front();
    B[u].pop_front();

    if (blocked[w]) {
      unblock(w);
    }
  }
}

// Johnson's output-circuit procedure
void Graph::outputCycle()
{
  vector<int> cycle;
  for (auto i = stack.begin(); i != stack.end(); i++) {
    cycle.push_back(*i);
  }

  cycle.push_back(*stack.begin());
  simpleCycles.push(cycle);
}

// Johnson's circuit() procedure
bool Graph::cycle(int v)
{
  blocked[v] = true;
  bool F = false;
  stack.push_back(v);
  for (int w : AK[v]) {
    if (w == S) {
      outputCycle();
      F = true;
    } else if (w > S && !blocked[w]) {
      F = cycle(w);
    }
  }

  if (F) {
    unblock(v);
  } else {
    for (int w : AK[v]) {
      if (find(B[w].begin(), B[w].end(), v) == B[w].end()) {
        B[w].push_back(v);
      }
    }
  }

  stack.pop_back();
  return F;
}

// Start of Johnson's Circuit-Finding algorithm
void Graph::getJohnsonSimpleCycles()
{
  S = 0;
  stack.clear();
  while (S < N) {
    for (int i = S; i < N; i++) {
      B[i].clear();
      blocked[i] = false;
    }
    cycle(S);
    S++;
  }

  simpleCyclesUpdate = false; // Flag to update
}

// Reverse graph and update AK and edgeLabels objects
void Graph::reverseGraph() {
  vector<vector<int>> newAK;
  map<pair<int,int>,double> newEdgeLabels;

  newAK.resize(AK.size());
  for (auto e : edgeLabels) {
    int src = get<0>(e.first);
    int dest = get<1>(e.first);
    double label = e.second;

    newEdgeLabels[{dest,src}] = label;
    newAK[toIndex[dest]].push_back(toIndex[src]);
  }

  this->AK = newAK;
  this->edgeLabels = newEdgeLabels;
  simpleCyclesUpdate = true;
}

// Find and print the length of the longest cycle that each node belongs to
void Graph::getLongestCycleLengthPerNode() {
  vector<bool> hasCycle(N,false);
  vector<int> cycleLengths(N,0);

  if (simpleCyclesUpdate) {
    getJohnsonSimpleCycles();
  }

  // Copy priority queue of cycles
  priority_queue<vector<int>, vector<vector<int>>, CompareVectSize> sc(simpleCycles);

  // Get lengths of cycles and assign to nodes
  while (!sc.empty()) {
    vector<int> c = sc.top();
    sc.pop();
    for (auto &node : c) {
      if (!hasCycle[node]) {
        cycleLengths[node] = c.size() - 1;
        hasCycle[node] = true;
      }
    }
  }

	for (pair<int, int> element : toIndex) {
    cout << element.first << " " << cycleLengths[element.second] << endl;
	}

}

// Find and print best arbitrage opportunity via
// smallest negative log-sum cycle
void Graph::getMaxArbitrage()
{
  vector<int> bestCycle;
  double minCycleSum = 0;

  if (simpleCyclesUpdate) {
    getJohnsonSimpleCycles();
  }

  // Determine smallest negative log-sum cycle
  while (!simpleCycles.empty()) {
    vector<int> cycle = simpleCycles.top();
    simpleCycles.pop();

    double cycleSum = 0;
    for (int i = 0; i < cycle.size() - 1; i++) {
      pair<int,int> edge({toNodes[cycle[i]],toNodes[cycle[i+1]]});

      // Use -log(label) to minimize sum of cycle labels
      cycleSum += -log(edgeLabels[edge]);

    }

    if (cycleSum < minCycleSum) {
      minCycleSum = cycleSum;
      bestCycle = cycle;
    }
  }

  if (minCycleSum == 0) {
    cout << "No arbitrage opportunity." << endl;
    return;
  }

  // Print minimal subgraph of best arbitrage opportunity
  for (int i = 0; i < bestCycle.size() - 1; i++) {
    pair<int,int> edge({toNodes[bestCycle[i]],toNodes[bestCycle[i+1]]});
    cout << toNodes[bestCycle[i]] << " " << toNodes[bestCycle[i+1]] << " " << edgeLabels[edge] << endl;
  }

}
