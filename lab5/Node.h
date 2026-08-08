#pragma once
#include "Parser.h"
#include "Scheme.h"
#include "Tuple.h"
#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <string>
#include <vector>

class Node {

private:
  set<int> adjacentNodeIDs;

  bool visited = false;

public:
  void addEdge(int adjacentNodeID) { adjacentNodeIDs.insert(adjacentNodeID); }

  set<int> getAdjNodeIDs() { return adjacentNodeIDs; }

  void setVisited(bool val) { visited = val; }

  bool getIsVisited() { return visited; }

  string toString() {
    bool first = true;
    stringstream out;
    for (int value : adjacentNodeIDs) {
      if (!first) {
        out << ",";
      }
      out << "R" << value;
      first = false;
    }
    return out.str();
  }
};