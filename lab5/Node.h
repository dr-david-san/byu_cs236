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

public:
  void addEdge(int adjacentNodeID) { adjacentNodeIDs.insert(adjacentNodeID); }

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