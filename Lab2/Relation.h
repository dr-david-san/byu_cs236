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
  static bool joinable(const Scheme &leftScheme, const Scheme &rightScheme,
                       const Tuple &leftTuple, const Tuple &rightTuple) {
    // add code to test whether the tuples are joinable
    for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
      const string &leftName = leftScheme.at(leftIndex);
      const string &leftValue = leftTuple.at(leftIndex);
      //   cout << "left name: " << leftName << " value: " << leftValue << endl;
      for (unsigned rightIndex = 0; rightIndex < rightScheme.size();
           rightIndex++) {
        const string &rightName = rightScheme.at(rightIndex);
        const string &rightValue = rightTuple.at(rightIndex);
        // cout << "right name: " << rightName << " value: " << rightValue <<
        // endl;
        if (leftName == rightName) {
          if (leftTuple.at(leftIndex) != rightTuple.at(rightIndex)) {
            return false;
          }
        }
      }
    }
    return true;
  };
  Relation join(const Relation &right) {
    const Relation &left = *this;
    Relation result(left.name, left.scheme); // wrong for now
    // add code to complete the join operation
    for (auto &leftTuple : left.tuples) {
      cout << "left tuple: " << leftTuple.toString(left.scheme) << endl;
      for (auto &rightTuple : right.tuples) {
        // joinable isnt used for the lab
        // if (Relation::joinable(left.scheme, right.scheme, leftTuple,
        //                        rightTuple)) {
        //   cout << "right tuple: " << rightTuple.toString(right.scheme) <<
        //   endl;
        // }
        cout << "right tuple: " << rightTuple.toString(right.scheme) << endl;
      }
    }
    return result;
  }
};