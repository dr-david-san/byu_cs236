#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include "vector"
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
    std::string filename = argv[1];
    std::ifstream in;
    in.open(filename);
    std::stringstream ss;
    ss << in.rdbuf();
    std::string input = ss.str();
    in.close();
    Scanner s = Scanner(input);

    std::vector<Token> vecToken = s.scanToken();

    for (std::size_t i = 0; i < vecToken.size(); ++i)
    {
        std::cout << vecToken[i].toString() << std::endl;
    }
    std::cout << "Total Tokens = " << vecToken.size() << std::endl;
}
