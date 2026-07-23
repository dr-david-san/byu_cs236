#pragma once
#include "Token.h"
#include <cctype>
#include <string>
#include <vector>

class Scanner {
private:
  std::string input;
  int pos = 0;
  std::vector<Token> vecToken;
  int line = 1;
  int initInputSize;

public:
  Scanner(const std::string &input) : input(input) {
    initInputSize = input.size();
  }

  void skipWhitespace() {
    while (pos < initInputSize && isspace(input.at(0))) {
      if (checkNewLine()) {
        line++;
      }
      input = input.substr(1);
      pos++;
    };
  }

  void parseNewLine() {
    pos++;
    input = input.substr(1);
    line++;
  }

  bool checkNewLine() {
    if (!endInput() && input.at(0) == '\n') {
      return true;
    }

    return false;
  }

  bool endInput() { return pos >= initInputSize; }

  void inLineComment() {
    std::string comment;
    // Remove comments for parser project
    // int startCommentLine = line;

    while (!endInput() && !checkNewLine()) {
      comment.push_back(input.at(0));
      input = input.substr(1);
      pos++;
    }
    // vecToken.push_back(Token(COMMENT, comment, startCommentLine));
  }

  bool endBlockComment() {
    if (input.at(0) == '|') {
      if (!endInput() && input.at(1) == '#') {
        return true;
      }
    }
    return false;
  }

  void blockComment() {
    std::string iComment;
    int startCommentLine = line;

    while (!endInput() && !endBlockComment()) {
      iComment.push_back(input.at(0));
      input = input.substr(1);
      pos++;
      if (checkNewLine()) {
        line++;
      }
    }
    if (!endInput() && endBlockComment()) {
      pos++;
      pos++;
      iComment.push_back(input.at(0));
      input = input.substr(1);
      iComment.push_back(input.at(0));
      input = input.substr(1);
      // vecToken.push_back(Token(COMMENT, iComment, startCommentLine));
    } else if (endInput()) {
      vecToken.push_back(Token(UNDEFINED, iComment, startCommentLine));
    }
  }

  void specChar() {
    switch (input.at(0)) {
    case ',':
      vecToken.push_back(Token(COMMA, ",", line));
      input = input.substr(1);
      pos++;
      break;
    case '.':
      vecToken.push_back(Token(PERIOD, ".", line));
      input = input.substr(1);
      pos++;
      break;
    case '?':
      vecToken.push_back(Token(Q_MARK, "?", line));
      input = input.substr(1);
      pos++;
      break;
    case '(':
      vecToken.push_back(Token(LEFT_PAREN, "(", line));
      input = input.substr(1);
      pos++;
      break;
    case ')':
      vecToken.push_back(Token(RIGHT_PAREN, ")", line));
      input = input.substr(1);
      pos++;
      break;
    case ':':
      if (input.at(1) == '-') {
        vecToken.push_back(Token(COLON_DASH, ":-", line));
        input = input.substr(2);
        pos++;
        pos++;
        break;
      } else {
        vecToken.push_back(Token(COLON, ":", line));
        input = input.substr(1);
        pos++;
        break;
      }
    case '*':
      vecToken.push_back(Token(MULTIPLY, "*", line));
      input = input.substr(1);
      pos++;
      break;
    case '+':
      vecToken.push_back(Token(ADD, "+", line));
      input = input.substr(1);
      pos++;
      break;
    case '#':
      if (!endInput() && input.at(1) == '|') {
        blockComment();
      } else {
        inLineComment();
      }
      break;
    case '\'':
      parseString();
      break;
    default:
      if (isalpha(input.at(0))) {
        parseSpecialString();
      } else if (isdigit(input.at(0))) {
        std::string s(1, input.at(0));
        vecToken.push_back(Token(UNDEFINED, s, line));
        input = input.substr(1);
        pos++;
      } else {
        std::string s;
        s.push_back(input.at(0));
        vecToken.push_back(Token(UNDEFINED, s, line));
        input = input.substr(1);
        pos++;
      }
      break;
    }
  }

  void validId(std::string id) {
    if (isalpha(id.at(0))) {
      vecToken.push_back(Token(ID, id, line));
    } else {
      vecToken.push_back(Token(UNDEFINED, id, line));
    }
  }

  void parseSpecialString() {
    std::string sString;

    while (!endInput() && isalnum(input.at(0))) {
      sString.push_back(input.at(0));
      input = input.substr(1);
      pos++;
    }
    if (sString == "Schemes") {
      vecToken.push_back(Token(SCHEMES, sString, line));
    } else if (sString == "Facts") {
      vecToken.push_back(Token(FACTS, sString, line));
    } else if (sString == "Rules") {
      vecToken.push_back(Token(RULES, sString, line));
    } else if (sString == "Queries") {
      vecToken.push_back(Token(QUERIES, sString, line));
    } else {
      validId(sString);
    }
  };

  void parseString() {
    std::string pString;
    int startLine = line;
    bool inString = false;

    while (!endInput() && !detectEndOfString(inString)) {
      if (checkNewLine()) {
        line++;
      }
      inString = true;
      pString.push_back(input.at(0));
      input = input.substr(1);
      pos++;
    }
    if (!endInput() && detectEndOfString(inString)) {
      pString.push_back(input.at(0));
      pos++;
      input = input.substr(1);
      vecToken.push_back(Token(STRING, pString, startLine));
    } else {
      vecToken.push_back(Token(UNDEFINED, pString, startLine));
    }
  }

  bool detectEndOfString(bool inString) {
    if (inString) {
      if (!endInput() && input.at(0) == '\'') {
        return true;
      }
    }
    return false;
  }

  std::vector<Token> scanToken() {

    while (pos < initInputSize) {
      skipWhitespace();
      if (endInput()) {
        break;
      }
      if (checkNewLine()) {
        parseNewLine();
        continue;
      }

      specChar();
    }

    Token endToken = Token(END, "", line);

    vecToken.push_back(endToken);
    return vecToken;
  }
};
