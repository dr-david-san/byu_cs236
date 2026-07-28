#pragma once
#include "Parser.h"
#include <cctype>
#include <set>
#include <string>
#include <vector>

class Scheme : public vector<string> {
private:
public:
  Scheme(vector<string> names) : vector<string>(names) {}
};