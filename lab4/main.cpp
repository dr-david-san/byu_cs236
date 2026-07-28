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

// int main() {

//   Scheme scheme1({"A", "B"});
//   Scheme scheme2({"B", "C"});

//   Tuple tuple1({"'1'", "'2'"});
//   Tuple tuple2({"'3'", "'4'"});

//   Scheme scheme3({"X", "Y"});
//   Scheme scheme4({"X", "Y", "Z"});

//   Tuple tuple3({"'1'", "'4'"});
//   Tuple tuple4({"'1'", "'2'", "'4'"});

//   cout << Relation::joinable(scheme1, scheme2, tuple1, tuple2) << endl;
//   cout << Relation::joinable(scheme2, scheme3, tuple1, tuple2) << endl;
//   cout << Relation::joinable(scheme3, scheme4, tuple1, tuple4) << endl;
//   cout << Relation::joinable(scheme3, scheme4, tuple3, tuple4) << endl;
// }

int main() {

  Relation studentRelation("students", Scheme({"ID", "Name", "Major"}));

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

  Relation studentCourseRelation = studentRelation.join(courseRelation);
  cout << studentCourseRelation.toString() << endl;
}
