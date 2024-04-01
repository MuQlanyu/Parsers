#pragma once
#include <algorithm>
#include <map>
#include <set>
#include <queue>

#include "Constants.h"

struct Rule {
  std::string from;
  std::vector<std::string> to;
};

struct Grammar {
  std::string start;
  std::vector<Rule> rules;
};


class Parser {
 public:
  Parser() = default;
  // последней строкой в векторе rules должен быть начальный символ
  Parser(const std::string& non_term, const std::string& term,
         const std::vector<std::string>& rules);

  void ReadGrammar(std::istream& istr = std::cin);
  void PrintGrammar(std::ostream& ostr = std::cout);

  Grammar GetGrammar();

 protected:
  using NonTerm = std::string;

  std::set<NonTerm> non_terminals;
  std::set<char> terminals;

  bool is_fit = false;
  Grammar grammar;

  bool static CheckBaseNonTerminal(char symb);
  bool static CheckBaseTerminal(char symb);

  bool CheckNonTerminal(const std::string& str);
  bool CheckTerminal(const std::string& str);
  bool CheckTerminal(char symbol);
  bool CheckRule(const std::string& str);

  void ReadNonTerminals(std::istream& istr, size_t num);
  void ReadTerminals(std::istream& istr, size_t num);
  void ReadRules(std::istream& istr, size_t num);

  void CheckNonTerminals(const std::string& str);
  void CheckTerminals(const std::string& str);
  void CheckRules(const std::vector<std::string>& rules);

  void AddRule(const NonTerm& from, std::vector<std::string> to);
};
