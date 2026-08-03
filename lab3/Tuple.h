#pragma once
#include "Parser.h"
#include "Scheme.h"
#include <cctype>
#include <set>
#include <string>
#include <vector>

class Tuple : public vector<string> {

public:
  Tuple(vector<string> values) : vector<string>(values) {}

  string toString(const Scheme &scheme) const {
    const Tuple &tuple = *this;
    stringstream out;
    // fix the code to print "name=value" pairs
    size_t count = min(scheme.size(), tuple.size());
    for (size_t i = 0; i < count; ++i) {
      out << scheme.at(i) << "=" << tuple.at(i);
      if (i < count - 1) {
        out << ", ";
      }
    }
    return out.str();
  }
};