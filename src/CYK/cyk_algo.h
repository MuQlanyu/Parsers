#pragma once
#include "../Parser.h"

class CYK : public Parser {
 public:
  void Fit();
  bool Predict(const std::string& word);

  CYK() = default;
  CYK(const std::string& non_term, const std::string& term,
      const std::vector<std::string>& rules) : Parser(non_term, term, rules) {}

 private:
  std::vector<size_t> double_rules;

  void DeleteNonGenerating();
  void DeleteUnreachable();
  void DeleteCombined();
  void DeleteLong();
  bool DeleteEpsRules();
  void ProcessStart(bool eps);
  void DeleteSingle();

  void DfsUnreachable(NonTerm& cur, std::map<NonTerm, bool>& used);

  void Eliminate(std::queue<size_t>& eliminate);
};
