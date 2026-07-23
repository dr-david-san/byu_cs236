#include "Relation.h"
#include "Scheme.h"
#include "Tuple.h"
#include <map>

using namespace std;

class Database {
private:
  map<string, class Relation> relations;

public:
  // Predicate('SK', ['A', 'B']) <- schemePredicate(scheme_name, scheme_columns)
  void add_relation(string relation_name, vector<string> scheme_names) {
    Scheme scheme(scheme_names);
    Relation relation(relation_name, scheme);
    relations.insert({relation_name, relation});
  };

  // Predicate('SK', ['a', 'c']) <- factPredicate(scheme_name, scheme_values)
  void add_tuple(string relation_name, vector<string> value) {
    relations.at(relation_name).addTuple(Tuple(value));
  };

  Relation get_relation(string tableName) { return relations.at(tableName); }
};

// later: in the interpreter class, have evaluating schemes, which loops through
// schemes and calls add_relation on the database class