#pragma once
#include "cyk_algo.h"

void CYK::Fit() {
  DeleteNonGenerating();
  DeleteUnreachable();
  DeleteCombined();
  DeleteLong();
  bool eps = DeleteEpsRules();
  ProcessStart(eps);
  DeleteSingle();

  is_fit = true;
  size_t ind = 0;
  double_rules.resize(grammar.rules.size());
  for (size_t i = 0; i < grammar.rules.size(); ++i) {
    if (grammar.rules[i].to.size() >= 2) {
      double_rules[ind++] = i;
    }
  }
  double_rules.resize(ind);
}

bool CYK::Predict(const std::string& word) {
  if (!is_fit) {
    Fit();
  }
  size_t sz = word.size();
  if (sz == 0) {
    for (auto& rule : grammar.rules) {
      if (rule.from == grammar.start &&
          rule.to.size() == 1 && rule.to[0].empty()) {
        return true;
      }
    }
    return false;
  }

  std::map<NonTerm, std::vector<std::vector<bool>>> dp;
  for (auto& non_term : non_terminals) {
    dp[non_term] = std::vector<std::vector<bool>>(sz, std::vector<bool>(sz, false));
  }

  for (size_t i = 0; i < sz; ++i) {
    for (auto& rule : grammar.rules) {
      if (rule.to.size() >= 2) continue;
      if (word.substr(i, 1) == rule.to[0]) {
        dp[rule.from][i][i] = true;
      }
    }
  }

  for (size_t len = 1; len < sz; ++len) {
    for (auto& rule : double_rules) {
      for (size_t i = 0; i + len < sz; ++i) {
        if (dp[grammar.rules[rule].from][i][i + len]) continue;
        for (size_t j = i; j < i + len; ++j) {
          if (dp[grammar.rules[rule].to[0]][i][j] &&
              dp[grammar.rules[rule].to[1]][j + 1][i + len]) {
            dp[grammar.rules[rule].from][i][i + len] = true;
            break;
          }
        }
      }
    }
  }
  return dp[grammar.start][0][sz - 1];
}

void CYK::DeleteNonGenerating() {
  std::vector<std::set<std::string>> non_term(grammar.rules.size());
  std::queue<size_t> queue;
  for (size_t i = 0; i < grammar.rules.size(); ++i) {
    for (size_t j = 0; j < grammar.rules[i].to.size(); ++j) {
      if (CheckNonTerminal(grammar.rules[i].to[j])) {
        non_term[i].insert(grammar.rules[i].to[j]);
      }
    }
    if (non_term[i].empty()) queue.push(i);
  }

  size_t current;
  while (!queue.empty()) {
    current = queue.front();
    queue.pop();

    for (auto& set_non_term : non_term) {
      set_non_term.erase(grammar.rules[current].from);
    }
  }
}

void CYK::DfsUnreachable(NonTerm& cur, std::map<NonTerm, bool>& used) {
  used[cur] = true;
  for (auto& rule : grammar.rules) {
    if (rule.from != cur) continue;
    for (auto to : rule.to) {
      if (CheckNonTerminal(to) && !used[to]) DfsUnreachable(to, used);
    }
  }
}

void CYK::Eliminate(std::queue<size_t>& eliminate) {
  if (eliminate.empty()) return;

  size_t ind = 0;
  for (size_t i = 0; i < grammar.rules.size(); ++i) {
    if (!eliminate.empty() && i == eliminate.front()) {
      eliminate.pop();
      continue;
    }
    grammar.rules[ind++] = grammar.rules[i];
  }
  grammar.rules.resize(ind);
}

void CYK::DeleteUnreachable() {
  std::map<NonTerm, bool> used;
  for (auto& non_term : non_terminals) {
    used[non_term] = false;
  }

  NonTerm cur = grammar.start;
  DfsUnreachable(cur, used);

  for (auto& non_term : used) {
    if (!non_term.second) {
      non_terminals.erase(non_term.first);
    }
  }

  std::queue<size_t> eliminate;
  for (size_t i = 0; i < grammar.rules.size(); ++i) {
    if (!used[grammar.rules[i].from]) {
      eliminate.push(i);
    }
  }

  Eliminate(eliminate);
}

void CYK::DeleteCombined() {
  size_t sz = grammar.rules.size();
  std::set<char> processed;
  for (size_t i = 0; i < sz; ++i) {
    for (auto& elem : grammar.rules[i].to) {
      if (!CheckTerminal(elem)) continue;
      if (processed.find(elem[0]) == processed.end()) {
        AddRule(Constants::SymbCombined + elem, {elem});
        processed.insert(elem[0]);
      }
      elem = Constants::SymbCombined + elem;
      non_terminals.insert(elem);
    }
  }
}

void CYK::DeleteLong() {
  size_t sz = grammar.rules.size();
  size_t tmp;
  size_t in_sz;
  size_t cnt = 1;
  for (size_t i = 0; i < sz; ++i) {
    if (grammar.rules[i].to.size() <= 2) continue;
    tmp = cnt;
    in_sz = grammar.rules[i].to.size();
    for (size_t j = 1; j < in_sz - 2; ++j) {
      AddRule(Constants::SymbLong + std::to_string(cnt),
              {grammar.rules[i].to[j], Constants::SymbLong + std::to_string(cnt + 1)});
      non_terminals.insert(Constants::SymbLong + std::to_string(cnt));
      ++cnt;
    }
    non_terminals.insert(Constants::SymbLong + std::to_string(cnt));
    AddRule(Constants::SymbLong + std::to_string(cnt),
            {grammar.rules[i].to[in_sz - 2], grammar.rules[i].to[in_sz - 1]});

    grammar.rules[i].to.resize(2);
    grammar.rules[i].to[1] = Constants::SymbLong + std::to_string(tmp);
  }
}

bool CYK::DeleteEpsRules() {
  bool eps;
  std::queue<NonTerm> queue;
  std::queue<size_t> eliminate;
  std::map<NonTerm, bool> used;
  for (size_t i = 0; i < grammar.rules.size(); ++i) {
    if (grammar.rules[i].to.size() == 1 && grammar.rules[i].to[0].empty()) {
      queue.push(grammar.rules[i].from);
      eliminate.push(i);
      used[grammar.rules[i].from] = false;
    }
  }

  Eliminate(eliminate);

  NonTerm cur;
  while (!queue.empty()) {
    cur = queue.front();
    queue.pop();
    used[cur] = true;
    if (cur == grammar.start) eps = true;

    for (auto rule = grammar.rules.begin(); rule != grammar.rules.end(); ++rule) {
      if (rule->to.size() == 1 && rule->to[0] == cur) {
        if (rule->from == grammar.start) {
          eps = true;
        } else if (used.find(rule->from) == used.end()) {
          queue.push(rule->from);
          used[rule->from] = false;
        }
      } else if (rule->to.size() == 2) {
        if (rule->to[0] == cur) {
          AddRule(rule->from, {rule->to[1]});
        } else if (rule->to[1] == cur) {
          AddRule(rule->from, {rule->to[0]});
        }
      }
    }
  }
  return eps;
}

void CYK::ProcessStart(bool eps) {
  AddRule(grammar.start + Constants::SymbStart, {grammar.start});
  grammar.start = grammar.start + Constants::SymbStart;
  non_terminals.insert(grammar.start);
  if (eps) {
    AddRule(grammar.start, {""});
  }
}

void CYK::DeleteSingle() {
  std::map<NonTerm, std::set<NonTerm>> closing;
  std::queue<size_t> eliminate;
  for (size_t i = 0; i < grammar.rules.size(); ++i) {
    if (grammar.rules[i].to.size() != 1 || !CheckNonTerminal(grammar.rules[i].to[0])) continue;
    closing[grammar.rules[i].from].insert(grammar.rules[i].to[0]);
    eliminate.push(i);
  }

  std::queue<NonTerm> newly_added;
  NonTerm inner;
  for (auto& cur : closing) {
    for (auto& added : cur.second) {
      newly_added.push(added);
    }
    while (!newly_added.empty()) {
      inner = newly_added.front();
      newly_added.pop();
      if (closing.find(inner) == closing.end()) break;

      for (auto& to : closing[inner]) {
        if (cur.second.find(to) == cur.second.end()) {
          cur.second.insert(to);
          newly_added.push(to);
        }
      }
    }
  }

  for (auto& cur : closing) {
    for (auto& rule : grammar.rules) {
      if (rule.to.size() == 1 && CheckNonTerminal(rule.to[0])) continue;
      if (cur.second.find(rule.from) == cur.second.end()) continue;
      if (rule.from == cur.first) continue;
      AddRule(cur.first, rule.to);
    }
  }

  Eliminate(eliminate);
  DeleteNonGenerating();
  DeleteUnreachable();
}