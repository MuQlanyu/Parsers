#pragma once
#include "earley.h"

void Earley::Fit() {
  if (is_fit) return;
  AddRule(Strings::EarleyStartSymbol, {grammar.start});
  grammar.start = Strings::EarleyStartSymbol;
  non_terminals.insert(Strings::EarleyStartSymbol);
  is_fit = true;
  D.clear();
}

bool operator<(const Situation& lhs, const Situation& rhs) {
  return lhs.read < rhs.read ||
      lhs.rule < rhs.rule ||
      lhs.dot < rhs.dot;
}

Rule& Earley::Rule(size_t ind) { return grammar.rules[ind]; }

std::string Earley::GetNext(size_t rule, size_t dot) {
  if ((Rule(rule).to.size() == 1 && Rule(rule).to[0] == "") ||
      dot >= Rule(rule).to.size()) {
    return Strings::EarleyEndSituation;
  }
  return Rule(rule).to[dot];
}

void Earley::AddSituation(size_t to, size_t read, size_t rule, size_t dot) {
  if (D[to][GetNext(rule, dot)].insert({read, rule, dot}).second) {
    new_situations[GetNext(rule, dot)].insert({read, rule, dot});
  }
}

bool operator==(const Situation& lhs, const Situation& rhs) {
  return lhs.read == rhs.read &&
      lhs.rule == rhs.rule &&
      lhs.dot == rhs.dot;
}

/// Earley ///

void Earley::Scan(size_t ind, char symb) {
  std::string str;
  str += symb;
  if (D[ind].find(str) != D[ind].end()) {
    for (auto& situation : D[ind][str]) {
      AddSituation(ind + 1, situation.read, situation.rule, situation.dot + 1);
    }
  }
}

void Earley::Predict(size_t ind, SitPair& copy) {
  for (size_t i = 0; i < grammar.rules.size(); ++i) {
    if (copy.find(Rule(i).from) != copy.end()) {
      for (auto& situation : copy[Rule(i).from]) {
        AddSituation(ind, ind, i);
      }
    }
  }
}

void Earley::Complete(size_t ind, SitPair& copy) {
  if (copy.find(Strings::EarleyEndSituation) == copy.end()) return;
  for (auto& sit_down : copy[Strings::EarleyEndSituation]) {
    if (D[sit_down.read].find(Rule(sit_down.rule).from) == D[sit_down.read].end()) continue;
    for (auto& sit_up : D[sit_down.read][Rule(sit_down.rule).from]) {
      AddSituation(ind, sit_up.read, sit_up.rule, sit_up.dot + 1);
    }
  }
}

bool Earley::PredictEarley(const std::string& word) {
  if (!is_fit) Fit();
  D.resize(word.size() + 1);

  AddSituation(0, 0, grammar.rules.size() - 1); // Добавили ситуацию (S' -> .S, 0)
  SitPair copy;
  for (size_t i = 0; i < word.size(); ++i) {
    while (!new_situations.empty()) {
      copy = new_situations;
      new_situations.clear();
      Predict(i, copy);
      Complete(i, copy);
      copy.clear();
    }
    Scan(i, word[i]);
  }
  while (!new_situations.empty()) {
    copy = new_situations;
    new_situations.clear();
    Predict(word.size(), copy);
    Complete(word.size(), copy);
    copy.clear();
  }

  Situation end = {0, grammar.rules.size() - 1, 1};
  for (auto& situation : D[word.size()][Strings::EarleyEndSituation]) {
    if (situation == end) {
      D.clear();
      return true;
    }
  }
  D.clear();
  return false;
}