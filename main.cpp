#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include "graph.h"

using namespace std;


void readGraph(istream& infile, vector<vector<int>>& AK,
                map<int,int>& toNodes, map<int,int>& toIndex,
								map<pair<int,int>,double>& edgeLabels)
{

	string line;
	int nodesRead = 0;
	while (getline(infile, line)) {
		istringstream iss(line);
		int src, dest;
		double label;

		if (!(iss >> src >> dest >> label)) {
			cerr << "File format is incorrect." << endl;
			break;
		}

		// Mapping from edge(src,dest) -> label
		edgeLabels[{src,dest}] = label;

		// Mappings from external node label <-> internal node index
    if (toIndex.count(src) == 0) {
      toIndex[src] = nodesRead;
      toNodes[nodesRead] = src;
      nodesRead++;
    }
    if (toIndex.count(dest) == 0) {
      toIndex[dest] = nodesRead;
      toNodes[nodesRead] = dest;
      nodesRead++;
    }
  }

	// Populate AK adjacency vector with edges
  AK.resize(nodesRead);
  for (auto e : edgeLabels) {
		AK[toIndex[get<0>(e.first)]].push_back(toIndex[get<1>(e.first)]);
  }

}

int main(int argc, char **argv)
{
	if (argc < 3 || (string(argv[1]) != "--reverse" &&
									 string(argv[1]) != "--longestcycle" &&
									 string(argv[1]) != "--maxarbitrage"))
	{
			cerr << "Usage:" << endl;
			cerr << argv[0] << " --reverse --graph=FILENAME" << endl;
			cerr << argv[0] << " --longestcycle --graph=FILENAME" << endl;
			cerr << argv[0] << " --maxarbitrage --graph=FILENAME" << endl;
			return 1;
	}

	string filename = string(argv[2]).substr(8);
	ifstream infile(filename);
	if (!infile.is_open()) {
			cerr << "Could not open file.\n";
			return 1;
	}

	vector<vector<int>> AK; // Adjacency vector of edges
	map<int,int> toNodes; // filenode->nodeindex
	map<int,int> toIndex; // nodeindex->filenode
	map<pair<int,int>,double> edgeLabels;  // map<src,dest>->label

	readGraph(infile, AK, toNodes, toIndex, edgeLabels);
	Graph graph(AK, toNodes, toIndex, edgeLabels);

	if (string(argv[1]) == "--reverse") {
		graph.reverseGraph();
		graph.printGraph();
	} else if (string(argv[1]) == "--longestcycle") {
    graph.getJohnsonSimpleCycles();
	  graph.getLongestCycleLengthPerNode();
	} else if (string(argv[1]) == "--maxarbitrage") {
		graph.getJohnsonSimpleCycles();
	  graph.getMaxArbitrage();
	}

	return 0;
}
