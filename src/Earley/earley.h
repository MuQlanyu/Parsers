#pragma once
#include "../Parser.h"

struct Situation {
  size_t read;
  size_t rule;  // Индекс правила в grammar.rules
  size_t dot;  // Индекс символа перед которым стоит точка в массиве
};

class Earley : public Parser {
 public:
  void Fit();
  bool PredictEarley(const std::string& word);

  Earley() = default;
  Earley(const std::string& non_term, const std::string& term,
         const std::vector<std::string>& rules) : Parser(non_term, term, rules) {}

 private:
  using SitPair = std::map<std::string, std::set<Situation>>;
  std::vector<SitPair> D;
  SitPair new_situations;

  Rule& Rule(size_t ind);

  std::string GetNext(size_t rule, size_t dot);

  void AddSituation(size_t to, size_t read, size_t rule, size_t dot = 0);

  void Scan(size_t ind, char symb);
  void Predict(size_t ind, SitPair& copy);
  void Complete(size_t ind, SitPair& copy);
};