#include "Interpreter.h"
#include "Parser.h"
#include "Scanner.h"
#include "Token.h"
#include "vector"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {

  string filename = argv[1];
  ifstream in;
  in.open(filename);
  stringstream ss;
  ss << in.rdbuf();
  string input = ss.str();
  in.close();

  Scanner s = Scanner(input);
  vector<Token> vecToken = s.scanToken();
  Parser p = Parser(vecToken);
  DatalogProgram output = p.parse();

  if (output.wasSuccess()) {
    Interpreter interpreter(output);
    interpreter.run();
  }

  return 0;
}
