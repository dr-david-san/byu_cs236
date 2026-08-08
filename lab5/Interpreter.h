#include "Database.h"
#include "Graph.h"
#include "Parser.h"
#include "Relation.h"
#include <sstream>

class Interpreter {
private:
  Database database;
  DatalogProgram datalogProgram;

public:
  Interpreter(const DatalogProgram &program) : datalogProgram(program) {}

  void run() {
    evaluateSchemes();
    evaluteFacts();
    evaluateRules();
    evaluateQueries();
  }

  static Graph makeGraph(const vector<Rule> &rules) {

    Graph graph(rules.size());
    // add code to add edges to the graph for the rule dependencies
    for (size_t i = 0; i < rules.size(); i++) {
      stringstream out;
      Rule rule = rules.at(i);
      string ruleString = rule.toString();
      if (!ruleString.empty()) {
        ruleString.pop_back();
      }
      out << "from rule R" << i << ": " << ruleString << endl;
      for (auto &pred :
           rule.getBodyPred()) { // predicate name in the body of the from
        // from body predicate: B()
        out << "from body predicate: " << pred.toString() << endl;
        for (size_t j = 0; j < rules.size(); j++) {
          Rule innerRule = rules.at(j);
          string innerRuleString = innerRule.toString();
          if (!innerRuleString.empty()) {
            innerRuleString.pop_back();
          }
          out << "to rule R" << j << ": " << innerRuleString << endl;

          if (pred == innerRule.getHeadPred()) {
            graph.addEdge(i, j);
            out << "dependency found: (" << "R" << i << ",R" << j << ")"
                << endl;
          }
        }
      }

      // cout << out.str();
    }
    return graph;
  }

  string formatSCC(vector<int> &scc) {
    stringstream out;
    bool first = true;
    for (int ruleID : scc) {
      if (!first) {
        out << ",";
      }
      out << "R" << ruleID;
      first = false;
    }
    return out.str();
  }

  void evaluateRules() {
    Graph graph = makeGraph(datalogProgram.getRules());
    cout << "Dependency Graph\n" << graph.toString() << endl;
    cout << "Rule Evaluation" << endl;
    vector<vector<int>> sccs = graph.getSCCs();

    for (auto &scc : sccs) {
      cout << "SCC: " << formatSCC(scc) << endl;

      bool isTrivial = false;
      if (scc.size() == 1) {
        int singleRuleID = *scc.begin();
        if (!graph.hasEdge(singleRuleID, singleRuleID)) {
          isTrivial = true;
        }
      }

      int passes = 1;
      if (isTrivial) {
        int ruleID = *scc.begin();
        Rule rule = datalogProgram.getRules()[ruleID];
        cout << rule.toString() << endl;
        evaluateRule(rule);
      } else {
        passes = 0;
        bool addedSomething = true;
        while (addedSomething) {
          addedSomething = false;
          passes = passes + 1;
          for (auto &ruleID : scc) {
            Rule rule = datalogProgram.getRules()[ruleID];
            cout << rule.toString() << endl;
            if (evaluateRule(rule)) {
              addedSomething = true;
            }
          }
        }
      }
      cout << passes << " passes: " << formatSCC(scc) << endl;
    }
  }

  bool evaluateRule(Rule &rule) {
    // rule =
    //  :- parent(x, y), ancestor(y, z)
    vector<Predicate> bodyPreds = rule.getBodyPred();
    vector<Relation> bodyResults;
    for (auto &body : bodyPreds) {
      bodyResults.push_back(evaluatePredicate(body));
    }
    Relation relation = bodyResults[0];
    for (size_t i = 1; i < bodyResults.size(); i++) {
      relation = relation.join(bodyResults[i]);
    }
    vector<int> indexes = getIndexes(relation, rule.getHeadPred());
    relation = relation.project(indexes);

    // rename
    Relation &targetRelation =
        database.get_relation(rule.getHeadPred().getName());
    Scheme scheme = Scheme(targetRelation.getScheme());
    relation = relation.rename(scheme);

    // union
    bool added = targetRelation.unite(relation);

    return added;
  }

  vector<int> getIndexes(Relation relation, Predicate headPredicate) {
    Scheme relScheme = relation.getScheme();
    vector<int> indexes;
    for (auto &value : headPredicate.parametersAsStrings()) {
      auto it = find(relScheme.begin(), relScheme.end(), value);
      if (it != relScheme.end()) {
        indexes.push_back(it - relScheme.begin());
      }
    }
    return indexes;
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

  // Refactor Queries which returns a relation (ie pass in each query and run
  // the select, project, rename)

  Relation evaluatePredicate(Predicate &query) {
    string tableName = query.getName();
    Relation relation = database.get_relation(tableName);
    vector<Parameter> columnQueries = query.getParameters();

    // 3 vars initialized - a map for the strings and indexes, vector for
    // indexes, and vector for names
    map<string, int> firstSeen; // {'a', 1} first saw a at index 1
    vector<int> keepIndexes;
    vector<string> keepNames;

    // for loop using indexes for(i = 0, ...)
    for (size_t i = 0; i < columnQueries.size(); i++) {
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
    return relation;
  }

  void evaluateQueries() {
    vector<Predicate> queries = datalogProgram.getQueries();
    // for loop quieres -> query
    cout << endl << "Query Evaluation" << endl;
    for (auto &query : queries) {

      Relation relation = evaluatePredicate(query);
      // create the output
      cout << query.toString() << "? ";
      if (relation.size() == 0) {
        cout << "No" << endl;
      } else {
        cout << "Yes(" << relation.size() << ")" << endl;
        if (!relation.getScheme().empty()) {
          cout << relation.toString();
        }
      }
    }
  }
};