# Directed Graph Arbitrage

In this repo, you'll find a custom directed graph data structure that can do the following:
1. Reverse / transpose all edges
2. Find the longest cycle that includes that node, for each node.
3. Find the greatest arbitrage opportunity (product of labeled edges in cycle)

Most of the leg work for #2 and #3 was adapted from Johnson's circuit finding algorithm.

#### Getting Started

Compile with `make compile`, or call g++ like so:
```
g++ -std=c++11 main.cpp graph.cpp -o prog
```

Run commands with graph input files like so:
```
./prog --reverse --graph=g.in
./prog --longestcycle --graph=g.in
./prog --maxarbitrage --graph=g.in
```

#### Problem 1: Reverse

For this problem, I've created a function called Graph::reverseGraph() that accomplishes two things:
1. Replaces the `edgeLabels` mapping of `<src,dest>->label` with a new mapping `<dest,src>->label`.
2. Updates the `AK` graph adjacency vector that maps edges `node->vector(node)`.

Functions:
* `Graph::reverseGraph()`

#### Problem 2: Longest Cycle
For this problem, I rely on Johnson's circuit finding algorithm (pseudo-code below) to first find all simple cycles in the input graph and store each cycle in a priority queue ordered by length of cycle. Then we iteratively pop each cycle off the queue and assign each nodes with their largest cycle.

Functions:
* `Graph::getJohnsonSimpleCycles()`, O((|V| + |E|)(c + 1)) where c = # cycles
* `Graph::getLongestCycleLengthPerNode()`


#### Problem 3: Max Arbitrage

For this problem, I also rely on Johnson's circuit finding algorithm to first find all simple cycles of the input graph. Next, we iterate through these cycles and take the negative log sum of each cycle's labels to determine the cycle with the best arbitrage opportunity. We use the negative log sum of edge labels because it is equivalent to maximizing the product of edge labels.

```
Π(x_i)->max  <=>  Σ(-log(x_i))->min
```

Functions:
* `Graph::getJohnsonSimpleCycles()`, O((|V| + |E|)(c + 1)) where c = # cycles
* `Graph::getMaxArbitrage()`


#### Johnson's Circuit-Finding Algorithm

Here is the pseudocode of Johnson's circuit finding algorithm that I used to implement my solution. You can also read his paper describing the algorithm here: [Finding All The Elementary Circuits Of A Directed Graph (1975)](https://www.cs.tufts.edu/comp/150GA/homeworks/hw1/Johnson%2075.PDF)

```
begin
  integer list array A_k(n), B(n);
  logical array blocked(n);
  integer s;
  logical procedure CIRCUIT (integer value v);
    begin logical f;
      procedure UNBLOCK (integer value u);
        begin
          blocked(u) := false;
          for w in B(u) do
            begin
              delete w from B(u);
              if blocked(w) then UNBLOCK(w);
            end
        end UNBLOCK;
      f := false;
      stack v;
      blocked(v) := true;
      for w in A_k(v) do
        if w=s then
          begin
            OUTPUT-CIRCUIT (composed of stack followed by s);
            f := true;
          end
        else if !blocked(w) then
            if CIRCUIT(w) then f := true;
      if f then UNBLOCK(v)
      else for w in A_k(v) do
            if v not in B(w) then put v on B(w);
      unstack v;
      CIRCUIT := f;
    end CIRCUIT;
  empty stack;
  s := 1;
  while s < n do
    begin
      A_k := adjacency structure of strong component K with
             least vertex in subgraph induced by {s,s+1,...,n};
      if A_k != {} then
        begin
          s := least vertex in V_k;
          for i in V_k do
            begin
              blocked(i) := false;
              B(i) := {}
            end
          dummy := CIRCUITS(s);
          s++;
        end
      else s := n;
    end
end
```
