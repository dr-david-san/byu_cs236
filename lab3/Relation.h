#pragma once
#include "Parser.h"
#include "Scheme.h"
#include "Tuple.h"
#include <cctype>
#include <set>
#include <string>
#include <vector>

class Relation {

private:
  string name;
  Scheme scheme;
  set<Tuple> tuples;

public:
  Relation(const string &name, const Scheme &scheme)
      : name(name), scheme(scheme) {}

  // func size returns size
  int size() const { return tuples.size(); }

  void addTuple(const Tuple &tuple) { tuples.insert(tuple); }

  string toString() const {
    stringstream out;
    // add code to print the Tuples, one per line
    for (const auto &tuple : tuples) {
      out << "  " << tuple.toString(scheme) << "\n";
    }
    return out.str();
  }

  Relation select(int index, const string &value) const {
    Relation result(name, scheme);
    // add tuples to the result if they meet the condition
    for (const auto &tuple : tuples) {
      if (tuple.at(index) == value) {
        result.addTuple(tuple);
      }
    }
    return result;
  }

  Relation select(int index1, int index2) const {
    Relation result(name, scheme);
    for (const auto &tuple : tuples) {
      if (tuple.at(index1) == tuple.at(index2)) {
        result.addTuple(tuple);
      }
    }
    return result;
  }

  // keep columns at given indexes (project)
  Relation project(const vector<int> &indexes) const {
    // create a vector of names
    // loop through all indexes and grab said names
    vector<string> names;
    for (int index : indexes) {
      // pushback scheme for each index
      names.push_back(scheme.at(index));
    }
    Relation result(name, Scheme(names));
    for (const auto &tuple : tuples) {
      vector<string> values;
      for (int index : indexes) {
        values.push_back(tuple.at(index));
      }
      result.addTuple(Tuple(values));
    }
    return result;
  }

  // rename scheme attribute, tuples rename the same
  Relation rename(const Scheme &newScheme) const {
    Relation result(name, newScheme);
    for (const auto &tuple : tuples) {
      result.addTuple(tuple);
    }
    return result;
  }
};
