#pragma once
#include "Node.h"
#include <iterator>
#include <stack>
#include <vector>

class Graph {

private:
  map<int, Node> nodes;

public:
  Graph(int size) {
    for (int nodeID = 0; nodeID < size; nodeID++)
      nodes[nodeID] = Node();
  }

  void addEdge(int fromNodeID, int toNodeID) {
    nodes[fromNodeID].addEdge(toNodeID);
  }

  bool hasEdge(int fromID, int toID) {
    if (nodes.find(fromID) != nodes.end()) {

      const std::set<int> &neighbors = nodes[fromID].getAdjNodeIDs();

      return neighbors.find(toID) != neighbors.end();
    }
    return false;
  }

  void dfsPostorder(int nodeID, stack<int> &finishStack) {
    nodes[nodeID].setVisited(true);
    for (int adjID : nodes[nodeID].getAdjNodeIDs()) {
      if (!nodes[adjID].getIsVisited()) {
        dfsPostorder(adjID, finishStack);
      }
    }
    finishStack.push(nodeID);
  }

  // here is code that I added and prolly need to review
  stack<int> dfsForestPostorder() {
    stack<int> finishstack;
    for (auto &pair : nodes) {
      pair.second.setVisited(false);
    }

    for (auto &pair : nodes) {
      int nodeID = pair.first;
      if (!nodes[nodeID].getIsVisited()) {
        dfsPostorder(nodeID, finishstack);
      }
    }
    return finishstack;
  }

  void dfsCollect(int nodeID, vector<int> &component) {
    nodes[nodeID].setVisited(true);
    component.push_back(nodeID);

    for (int adjID : nodes[nodeID].getAdjNodeIDs()) {
      if (!nodes[adjID].getIsVisited()) {
        dfsCollect(adjID, component);
      }
    }
  }

  vector<vector<int>> getSCCs() {
    Graph reversed = reverseGraph();
    stack<int> finishStack = reversed.dfsForestPostorder();
    for (auto &pair : nodes) {
      pair.second.setVisited(false);
    }
    vector<vector<int>> sccs;

    while (!finishStack.empty()) {
      int nodeID = finishStack.top();
      finishStack.pop();

      if (!nodes[nodeID].getIsVisited()) {
        vector<int> component;

        dfsCollect(nodeID, component);
        sort(component.begin(), component.end());
        sccs.push_back(component);
      }
    }
    return sccs;
  }

  Graph reverseGraph() {

    Graph revGraph = Graph(nodes.size());
    for (auto &pair : nodes) {
      int fromId = pair.first;
      Node &node = pair.second;
      for (auto &adjID : node.getAdjNodeIDs()) {
        revGraph.addEdge(adjID, fromId);
      }
    }
    return revGraph;
  }

  string toString() {
    stringstream out;
    for (auto &pair : nodes) {
      int nodeID = pair.first;
      Node node = pair.second;
      out << "R" << nodeID << ":" << node.toString() << endl;
    }
    return out.str();
  }
};