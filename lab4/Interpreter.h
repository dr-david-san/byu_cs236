#include "Database.h"
#include "Parser.h"

class Interpreter {
private:
  Database database;
  DatalogProgram datalogProgram;

public:
  Interpreter(const DatalogProgram &program) : datalogProgram(program) {}

  void run() {
    evaluateSchemes();
    evaluteFacts();
    evaluateQueries();
  }

  void evaluateRules() {
    vector<Rule> rules = datalogProgram.getRules();
    // cn(c,n) :- snap(S,n,A,P),csg(c,S,G).
    Relation leftRelation = database.get_relation("snap");
    Relation rightRelation = database.get_relation("csg");
    Relation newRel = leftRelation.join(rightRelation);
    // mySet.insert(myTuple).second
    // get headPred
    // get bodyPred
    // loop through rules n times...?
    // cout example: Schemes populated after 2 passes through the Rules.
  }

  void evaluateSchemes() {
    vector<Predicate> schemes = datalogProgram.getSchemes();
    // for loop schemes -> scheme
    for (auto &scheme : schemes) {
      database.add_relation(scheme.getName(), scheme.parametersAsStrings());
    }
  }

  void evaluteFacts() {
    // add getFacts (similar to getSchemes) to datalogProgram
    vector<Predicate> facts = datalogProgram.getFacts();
    // for loop facts -> fact
    for (auto &fact : facts) {
      database.add_tuple(fact.getName(), fact.parametersAsStrings());
    }
  }

  void evaluateQueries() {
    vector<Predicate> queries = datalogProgram.getQueries();
    // for loop quieres -> query
    for (auto &query : queries) {
      string tableName = query.getName();
      Relation relation = database.get_relation(tableName);
      vector<Parameter> columnQueries = query.getParameters();

      // 3 vars initialized - a map for the strings and indexes, vector for
      // indexes, and vector for names
      map<string, int> firstSeen; // {'a', 1} first saw a at index 1
      vector<int> keepIndexes;
      vector<string> keepNames;

      // for loop using indexes for(i = 0, ...)
      for (int i = 0; i < columnQueries.size(); i++) {
        // this is for constant
        if (columnQueries[i].isConstant) {
          relation = relation.select(i, columnQueries[i].toString());
        }
        // else this is not a constant
        else {
          string name = columnQueries[i].toString();
          // check to see if we have seen the name before (in our map)
          if (firstSeen.count(name) > 0) {
            relation = relation.select(firstSeen[name], i);
          } else {
            // update firstSeen
            firstSeen[name] = i;
            keepIndexes.push_back(i);
            keepNames.push_back(name);
          }
        }
      }
      // update relation with project
      relation = relation.project(keepIndexes);

      // update relation with rename
      relation = relation.rename(Scheme(keepNames));

      // create the output
      cout << query.toString() << "? ";
      if (relation.size() == 0) {
        cout << "No" << endl;
      } else {
        cout << "Yes(" << relation.size() << ")" << endl;
        if (!keepNames.empty()) {
          cout << relation.toString();
        }
      }
    }
  }
};