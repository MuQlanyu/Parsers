#pragma once
#include "CYK_tests.h"
#include "../src/CYK/cyk_algo.h"

TEST(CYKTests, ScenarioTests) {
  // Правильные скобочные последовательности
  CYK parser("S", "()", {"S->(S)S", "S->", "S"});
  ASSERT_TRUE(parser.Predict("()"));
  ASSERT_TRUE(parser.Predict("((()()()))()()(((())))"));
  ASSERT_TRUE(parser.Predict("()()()()()()()()()()()()()()()()"));
  ASSERT_TRUE(parser.Predict(""));
  ASSERT_FALSE(parser.Predict("("));
  ASSERT_FALSE(parser.Predict("())"));
  ASSERT_FALSE(parser.Predict("()()(()))("));
  ASSERT_FALSE(parser.Predict(")("));

  // из одной из домашек
  parser = CYK("SA", "ab", {"S->AS", "S->b", "A->SA", "A->a", "S"});
  ASSERT_TRUE(parser.Predict("bbaab"));
  ASSERT_TRUE(parser.Predict("ababab"));
  ASSERT_FALSE(parser.Predict("aabba"));

  // язык вида a^n*b^n
  parser = CYK("S", "ab", {"S->aSb", "S->", "S"});
  ASSERT_TRUE(parser.Predict(""));
  ASSERT_TRUE(parser.Predict("aabb"));
  ASSERT_TRUE(parser.Predict("aaaabbbb"));
  ASSERT_FALSE(parser.Predict("aabbb"));
  ASSERT_FALSE(parser.Predict("aaaaabbbbba"));
  ASSERT_FALSE(parser.Predict("aba"));
  ASSERT_FALSE(parser.Predict("aaaabbb"));
}

std::string MakeAB(size_t num) {
  std::string word_a = "";
  std::string word_b = "";
  for (size_t i = 0; i < num; ++i) {
    word_a += 'a';
    word_b += 'b';
  }
  return word_a + word_b;
}

TEST(CYKTests, StressTests) {
  const std::pair<int, int> range = {81, 85};
  const int big_test = 100;

  // язык вида a^n*b^n
  CYK parser("S", "ab", {"S->aSb", "S->", "S"});
  // 50 тестов вида a^i*b^i, где i пробегает [range.first, range.second]
  for (size_t test_size = range.first; test_size <= range.second; ++test_size) {
    ASSERT_TRUE(parser.Predict(MakeAB(test_size)));
  }

  // Большой тест a^i*b^i, где i=big_test
  ASSERT_TRUE(parser.Predict(MakeAB(big_test)));
}

bool CheckChomsky(CYK parser) {
  Grammar grammar = parser.GetGrammar();
  for (auto& rule : grammar.rules) {
    if (rule.to.size() > 2 || rule.to.size() == 0) return false;
    if (rule.to.size() == 1 && rule.to[0].empty()) {
      if (rule.from != grammar.start) return false;
    } else if (rule.to.size() == 2 && rule.to[0].empty()) return false;
  }
  return true;
}

TEST(CYKTests, UnitFit) {
  CYK parser("S", "()", {"S->(S)S", "S->", "S"});
  parser.Fit();
  ASSERT_TRUE(CheckChomsky(parser));

  parser = CYK("SA", "ab", {"S->AS", "S->b", "A->SA", "A->a", "S"});
  parser.Fit();
  ASSERT_TRUE(CheckChomsky(parser));

  parser = CYK("S", "ab", {"S->aSb", "S->", "S"});
  parser.Fit();
  ASSERT_TRUE(CheckChomsky(parser));
}