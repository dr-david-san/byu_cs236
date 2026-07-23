#include <vector>
#include "Token.h"

class Parser
{
private:
    vector<Token> tokens;

public:
    Parser(const vector<Token> &tokens) : tokens(tokens)
    {

        TokenType tokenType() const
        {
            return tokens.at(0).getType();
        }
        void advanceToken()
        {
            tokens.erase(tokens.begin());
        }
        void throwError()
        {
            cout << "error" << endl;
        }
    }
};