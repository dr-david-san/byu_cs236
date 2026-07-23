#include "Interpreter.h"
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
int main() {

  Relation studentRelation("students", Scheme({"ID", "Name", "Major"}));
  // studentRelation

  vector<string> studentValues[] = {
      {"'42'", "'Ann'", "'CS'"},
      {"'64'", "'Ned'", "'EE'"},
  };

  for (auto &value : studentValues)
    studentRelation.addTuple(Tuple(value));

  Relation courseRelation("courses", Scheme({"ID", "Course"}));

  vector<string> courseValues[] = {
      {"'42'", "'CS 100'"},
      {"'32'", "'CS 232'"},
  };

  for (auto &value : courseValues)
    courseRelation.addTuple(Tuple(value));

  studentRelation.join(courseRelation);
}