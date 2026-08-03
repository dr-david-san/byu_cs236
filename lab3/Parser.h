#include "Token.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

#pragma once

using namespace std;

class Parameter {
private:
  string value;

public:
  bool isConstant;

  Parameter(string value, bool isConstant = false)
      : value(value), isConstant(isConstant) {}

  string toString() { return value; }
};

class Predicate {
private:
  string name;
  vector<Parameter> parameters;

public:
  Predicate(string name, vector<Parameter> parameters)
      : name(name), parameters(parameters) {}
  vector<Parameter> getParameters() { return parameters; }
  string getName() { return name; }
  vector<string> parametersAsStrings() {
    vector<string> strings;
    for (auto &parameter : parameters) {
      strings.push_back(parameter.toString());
    };
    return strings;
  };
  string toString() {
    stringstream out;
    out << name << "(";
    for (int i = 0; i < parameters.size(); i++) {
      if (i != 0) {
        out << ",";
      }
      out << parameters.at(i).toString();
    }
    out << ")";
    return out.str();
  }
};

class Rule {
private:
  Predicate headPredicate;
  vector<Predicate> bodyPredicate;

public:
  Rule(Predicate headPredicate, vector<Predicate> bodyPredicate)
      : headPredicate(headPredicate), bodyPredicate(bodyPredicate) {}

  string toString() {
    stringstream out;
    out << headPredicate.toString() << " :- ";
    for (int i = 0; i < bodyPredicate.size(); i++) {
      if (i != 0) {
        out << ",";
      }
      out << bodyPredicate.at(i).toString();
    }
    out << ".";
    return out.str();
  }
};

// class DatalogProgram
class DatalogProgram {
private:
  // 4 expected outs (schemes, facts, rules, queries)
  vector<Predicate> schemes;
  vector<Predicate> facts;
  vector<Rule> rules;
  vector<Predicate> queries;
  set<string> domain;
  bool ParseSuccess;

public:
  DatalogProgram(vector<Predicate> schemes, vector<Predicate> facts,
                 vector<Rule> rules, vector<Predicate> queries,
                 bool ParseSuccess)
      : schemes(schemes), facts(facts), rules(rules), queries(queries),
        ParseSuccess(ParseSuccess) {}

  bool wasSuccess() { return ParseSuccess; };

  void factToDomain() {
    for (Predicate fact : facts) {
      for (Parameter param : fact.getParameters()) {
        domain.insert(param.toString());
      }
    }
  }

  vector<Predicate> getSchemes() { return schemes; }
  vector<Predicate> getFacts() { return facts; }
  vector<Predicate> getQueries() { return queries; }

  string toString() {
    stringstream out;
    out << "Success!" << endl;
    out << "Schemes(" << schemes.size() << "):" << endl;
    for (Predicate scheme : schemes) {
      out << "  " << scheme.toString() << endl;
    }
    out << "Facts(" << facts.size() << "):" << endl;
    for (Predicate fact : facts) {
      out << "  " << fact.toString() << "." << endl;
    }
    out << "Rules(" << rules.size() << "):" << endl;
    for (Rule rule : rules) {
      out << "  " << rule.toString() << endl;
    }
    out << "Queries(" << queries.size() << "):" << endl;
    for (Predicate query : queries) {
      out << "  " << query.toString() << "?" << endl;
    }

    factToDomain();
    out << "Domain(" << domain.size() << "):" << endl;
    for (string d : domain) {
      out << "  " << d << endl;
    }
    return out.str();
  };
};

// rest of facts, rules, queries

class Parser {
private:
  vector<Token> tokens;
  vector<Predicate> schemes;
  vector<Predicate> facts;
  vector<Rule> rules;
  vector<Predicate> queries;

public:
  Parser(const vector<Token> &tokens) : tokens(tokens) {}

  TokenType tokenType() const { return tokens.at(0).getType(); }
  void advanceToken() { tokens.erase(tokens.begin()); }
  Token throwError() { throw tokens.at(0); }
  Token match(TokenType t) {
    if (tokenType() == t) {
      Token current = tokens.at(0);
      advanceToken();
      return current;
    } else {
      throw throwError();
    }
  }

  // FIRST CHUNK OF 4 FROM THE PROJECT 2 DESCRIPTION
  void schemeList() {
    if (tokenType() == ID) {
      scheme();
      schemeList();
    } else {
      // lambda
    }
  }
  void factList() {
    if (tokenType() == ID) {
      fact();
      factList();
    } else {
      // lambda
    }
  }
  void ruleList() {
    if (tokenType() == ID) {
      rule();
      ruleList();
    } else {
      // lambda
    }
  }
  void queryList() {
    if (tokenType() == ID) {
      query();
      queryList();
    } else {
      // lambda
    }
  }

  // LAST CHUNK OF 4 FROM THE PROJECT 2 DESCRIPTION
  void idList(vector<Parameter> &params) {
    if (tokenType() == COMMA) {
      match(COMMA);
      params.push_back(match(ID).getValue());
      idList(params);
    } else {
      // lambda
    }
  }
  void stringList(vector<Parameter> &params) {
    if (tokenType() == COMMA) {
      match(COMMA);
      params.push_back(match(STRING).getValue());
      stringList(params);
    } else {
      // lambda
    }
  }

  void parameterList(vector<Parameter> &params) {
    if (tokenType() == COMMA) {
      match(COMMA);
      Token t = parameter();
      Parameter p = Parameter(t.getValue(), t.getType() == STRING);
      params.push_back(p);
      parameterList(params);
    } else {
      // lambda
    }
  }

  Predicate predicate() {
    if (tokenType() == ID) {
      string name = match(ID).getValue();
      vector<Parameter> parameters;
      match(LEFT_PAREN);
      Token t = parameter();
      Parameter p = Parameter(t.getValue(), t.getType() == STRING);
      parameters.push_back(p);
      parameterList(parameters);
      match(RIGHT_PAREN);
      return Predicate(name, parameters);
    } else {
      throw throwError();
    }
  }

  void predicateList(vector<Predicate> &rules) {
    if (tokenType() == COMMA) {
      match(COMMA);
      rules.push_back(predicate());
      predicateList(rules);
    } else {
      // lambda
    }
  }

  // HEAD PREDICATE FUNCTION FROM PROJECT 2 DESCRIPTION
  Predicate headPredicate() {
    if (tokenType() == ID) {
      string name = match(ID).getValue();
      match(LEFT_PAREN);
      vector<Parameter> parameters;
      parameters.push_back(match(ID).getValue());
      idList(parameters);
      match(RIGHT_PAREN);
      return Predicate(name, parameters);
    } else {
      throw throwError();
    }
  }

  // CHUNK OF 4 FROM THE PROJECT 2 DESCRIPTION
  void scheme() {
    // add code for parsing a 'scheme'
    string name = match(ID).getValue();
    match(LEFT_PAREN);
    vector<Parameter> parameters;
    parameters.push_back(match(ID).getValue());
    idList(parameters);
    match(RIGHT_PAREN);
    schemes.push_back(Predicate(name, parameters));
  }

  void fact() {
    // add code for parsing a 'fact'
    string name = match(ID).getValue();
    match(LEFT_PAREN);
    vector<Parameter> parameters;
    parameters.push_back(match(STRING).getValue());
    stringList(parameters);
    match(RIGHT_PAREN);
    match(PERIOD);
    facts.push_back(Predicate(name, parameters));
  }

  void datalogProgram() {
    // add code for parsing a 'DatalogProgram'
    // SCHEMES COLON scheme schemeList
    //         FACTS COLON factList
    //         RULES COLON ruleList
    //         QUERIES COLON query queryList
    // 	END
    match(SCHEMES);
    match(COLON);
    scheme();
    schemeList();
    match(FACTS);
    match(COLON);
    factList();
    match(RULES);
    match(COLON);
    ruleList();
    match(QUERIES);
    match(COLON);
    query();
    queryList();
    match(END);
  }

  // THIS COULD ALSO VERY WELL NEED TO BE A CLASS OF ITS OWN, BUT I DONT KNOW
  // HOW TO DO THAT YET
  void rule() {
    // add code for parsing a 'rule'
    Predicate headPred = headPredicate();
    match(COLON_DASH);
    vector<Predicate> bodyPred;
    bodyPred.push_back(predicate());
    predicateList(bodyPred);
    match(PERIOD);
    rules.push_back(Rule(headPred, bodyPred));
  }
  void query() {
    // add code for parsing a 'query'
    queries.push_back(predicate());
    match(Q_MARK);
  }

  Token parameter() {
    if (tokenType() == ID) {
      return match(ID);
    } else if (tokenType() == STRING) {
      return match(STRING);
    } else {
      throw throwError();
    }
  }

  DatalogProgram parse() {
    bool parseSuccess;
    try {
      datalogProgram();
      parseSuccess = true;
    } catch (Token error) {
      cout << "Failure!" << endl;
      cout << "  " << error.toString() << endl;
      parseSuccess = false;
    }
    return DatalogProgram(schemes, facts, rules, queries, parseSuccess);
  }
};
