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

  Scheme getScheme() { return scheme; }

  bool addTuple(const Tuple &tuple) { return tuples.insert(tuple).second; }

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
      for (unsigned rightIndex = 0; rightIndex < rightScheme.size();
           rightIndex++) {
        const string &rightName = rightScheme.at(rightIndex);
        const string &rightValue = rightTuple.at(rightIndex);
        if (leftName == rightName && leftValue != rightValue) {
          return false;
        }
      }
    }
    return true;
  }

  // make a new empty relation r using scheme s

  // for each tuple t1 in r1
  //     for each tuple t2 in r2

  // 	if t1 and t2 are joinable
  // 	    combine t1 and t2 to make tuple t
  // 	    add tuple t to relation r
  // 	end if

  //     end for
  // end for

  Scheme joinSchemes(const Scheme &leftScheme, const Scheme &rightScheme) {
    vector<string> allNames(leftScheme.begin(), leftScheme.end());
    for (auto &name : rightScheme) {
      if (find(allNames.begin(), allNames.end(), name) == allNames.end())
        allNames.push_back(name);
    };
    return Scheme(allNames);
  };

  // joinTuples: only runs when we know it is joinable
  Tuple joinTuples(const Tuple &leftTuple, const Tuple &rightTuple) {
    vector<string> allValues(leftTuple.begin(), leftTuple.end());
    for (auto &value : rightTuple) {
      if (find(allValues.begin(), allValues.end(), value) == allValues.end())
        allValues.push_back(value);
    };
    return Tuple(allValues);
  }

  Relation join(const Relation &right) {
    const Relation &left = *this;

    Scheme newScheme = joinSchemes(left.scheme, right.scheme);

    Relation result("newRel", newScheme);

    //  add code to complete the join operation
    for (auto &leftTuple : left.tuples) {
      for (auto &rightTuple : right.tuples) {
        if (joinable(left.scheme, right.scheme, leftTuple, rightTuple)) {
          Tuple newTuple = joinTuples(leftTuple, rightTuple);
          result.addTuple(newTuple);
        }
      }
    }
    return result;
  }

  set<Tuple> getTuples() { return tuples; }

  bool unite(Relation &incomingRelation) {
    bool ifAdded = false;
    Relation &current = *this;

    for (auto &tuple : incomingRelation.getTuples()) {
      if (current.addTuple(tuple)) {
        ifAdded = true;
      }
    }

    return ifAdded;
  };
};