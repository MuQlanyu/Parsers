#pragma once
#include "CYK/cyk_algo.h"

Parser::Parser(const std::string& non_term, const std::string& term,
               const std::vector<std::string>& rules) {
  CheckNonTerminals(non_term);
  CheckTerminals(term);
  CheckRules(rules);
}

void Parser::ReadGrammar(std::istream& istr) {
  size_t num_non_terminals;
  size_t num_terminals;
  size_t num_rules;
  istr >> num_non_terminals >> num_terminals >> num_rules;
  try {
    ReadNonTerminals(istr, num_non_terminals);
    ReadTerminals(istr, num_terminals);
    ReadRules(istr, num_rules);
  } catch (...) { throw; }
}

bool Parser::CheckBaseNonTerminal(char symb) {
  int symbol_ascii = static_cast<int>(symb);
  int ascii = Constants::kNonTerminals.first;
  for (; ascii <= Constants::kNonTerminals.second; ++ascii) {
    if (symbol_ascii == ascii) return true;
  }
  return false;
}

bool Parser::CheckBaseTerminal(char symb) {
  int symbol_ascii = static_cast<int>(symb);
  int ascii = Constants::kTerminals.first;
  for (; ascii <= Constants::kTerminals.second; ++ascii) {
    if (symbol_ascii == ascii) return true;
  }
  ascii = Constants::kNumbers.first;
  for (; ascii <= Constants::kNumbers.second; ++ascii) {
    if (symbol_ascii == ascii) return true;
  }
  auto begin = Constants::kServiceSymbols.begin();
  auto end = Constants::kServiceSymbols.end();
  return std::find(begin, end, symbol_ascii) != end;
}

bool Parser::CheckNonTerminal(const std::string& str) {
  return non_terminals.find(str) != non_terminals.end();
}

bool Parser::CheckTerminal(const std::string& str) {
  if (str.size() != 1) return false;
  return terminals.find(str[0]) != terminals.end();
}

bool Parser::CheckTerminal(char symbol) {
  return terminals.find(symbol) != terminals.end();
}

bool Parser::CheckRule(const std::string& str) {
  if (str.size() < 3 || str.find(Constants::Arrow) != 1) return false;
  if (!CheckBaseNonTerminal(str[0])) return false;
  for (size_t i = 3; i < str.size(); ++i) {
    if (!CheckNonTerminal(str.substr(i, 1)) && !CheckTerminal(str[i])) return false;
  }
  return true;
}

void Parser::CheckNonTerminals(const std::string& str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (!CheckBaseNonTerminal(str[i])) throw Strings::ErrorInvalidNonTerminal;
    non_terminals.insert(str.substr(i, 1));
  }
}

void Parser::ReadNonTerminals(std::istream& istr, size_t num) {
  std::string str;
  istr >> str;
  if (str.size() != num) throw Strings::ErrorInvalidNumberOfArg;
  CheckNonTerminals(str);
}

void Parser::CheckTerminals(const std::string& str) {
  for (char symb : str) {
    if (!CheckBaseTerminal(symb)) throw Strings::ErrorInvalidTerminal;
    terminals.insert(symb);
  }
}

void Parser::ReadTerminals(std::istream& istr, size_t num) {
  std::string str;
  istr >> str;
  if (str.size() != num) throw Strings::ErrorInvalidNumberOfArg;
  CheckTerminals(str);
}

void Parser::CheckRules(const std::vector<std::string>& rules) {
  if (rules.size() < 1) throw Strings::ErrorInvalidNumberOfArg;
  grammar.rules.resize(rules.size() - 1);
  for (size_t i = 0; i < rules.size() - 1; ++i) {
    if (!CheckRule(rules[i])) throw Strings::ErrorInvalidRule;
    grammar.rules[i].from = rules[i].substr(0, 1);
    grammar.rules[i].to.resize(std::max(size_t(1), rules[i].size() - 3));
    for (size_t j = 3; j < rules[i].size(); ++j) {
      grammar.rules[i].to[j - 3] = rules[i].substr(j, 1);
    }
    if (rules[i].size() == 3) grammar.rules[i].to[0] = "";
  }
  if (rules.back().size() != 1 || !CheckBaseNonTerminal(rules.back()[0])) {
    throw Strings::ErrorInvalidStart;
  }
  grammar.start = rules.back();
}

void Parser::ReadRules(std::istream& istr, size_t num) {
  std::vector<std::string> rules(num + 1);
  for (size_t i = 0; i < num + 1; ++i) {
    std::cin >> rules[i];
  }
  CheckRules(rules);
}

///---------------------PrintGrammar---------------------///

std::ostream& operator<<(std::ostream& ostr, const Rule& rule) {
  ostr << '\n' << rule.from << Constants::Arrow;
  for (auto& elem : rule.to) {
    ostr << elem << ' ';
  }
  return ostr;
}

void PrintContainer(std::ostream& ostr, auto container) {
  for (auto& elem : container) {
    ostr << elem;
  }
  ostr << '\n';
}

void Parser::PrintGrammar(std::ostream& ostr) {
  ostr << non_terminals.size() << ' ';
  ostr << terminals.size() << ' ';
  ostr << grammar.rules.size() << '\n';
  PrintContainer(ostr, non_terminals);
  PrintContainer(ostr, terminals);
  PrintContainer(ostr, grammar.rules);
}

Grammar Parser::GetGrammar() { return grammar; }

void Parser::AddRule(const NonTerm& from, std::vector<std::string> to) {
  grammar.rules.emplace_back();
  grammar.rules.back().to = std::move(to);
  grammar.rules.back().from = from;
}