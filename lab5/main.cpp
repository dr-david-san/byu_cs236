#include "Graph.h"
#include "Interpreter.h"
#include "Node.h"
#include "Parser.h"
#include "Scanner.h"
#include "Token.h"
#include "vector"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// int main(int argc, char *argv[]) {

//   string filename = argv[1];
//   ifstream in;
//   in.open(filename);
//   stringstream ss;
//   ss << in.rdbuf();
//   string input = ss.str();
//   in.close();

//   Scanner s = Scanner(input);
//   vector<Token> vecToken = s.scanToken();
//   Parser p = Parser(vecToken);
//   DatalogProgram output = p.parse();

//   if (output.wasSuccess()) {
//     Interpreter interpreter(output);
//     interpreter.run();
//   }

//   return 0;
// }

// int main() {
//   Node node;
//   node.addEdge(4);
//   node.addEdge(8);
//   node.addEdge(2);
//   cout << node.toString() << endl;
// }

// int main() {
//   Graph graph(3);
//   graph.addEdge(1, 2);
//   graph.addEdge(1, 0);
//   graph.addEdge(0, 1);
//   graph.addEdge(1, 1);
//   cout << graph.toString();
// }

int main() {

  // predicate names for fake rules
  // first is name for head predicate
  // second is names for body predicates
  pair<string, vector<string>> ruleNames[] = {
      {"A", {"B", "C"}}, {"B", {"A", "D"}}, {"B", {"B"}},
      {"E", {"F", "G"}}, {"E", {"E", "F"}},
  };

  vector<Rule> rules;

  for (auto &rulePair : ruleNames) {
    string headName = rulePair.first;
    vector<Parameter> emptyParams;
    Rule rule = Rule(Predicate(headName));
    vector<string> bodyNames = rulePair.second;
    for (auto &bodyName : bodyNames)
      rule.addBodyPredicate(Predicate(bodyName));
    rules.push_back(rule);
  }

  Graph graph = Interpreter::makeGraph(rules);
  cout << graph.toString();
}
