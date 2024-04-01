#pragma once
#include "Earley_tests.h"
#include "../src/Earley/earley.h"

TEST(EarleyTests, ScenarioTests) {
  // Правильные скобочные последовательности
  Earley parser("S", "()", {"S->(S)S", "S->", "S"});
  ASSERT_TRUE(parser.PredictEarley("()"));
  ASSERT_TRUE(parser.PredictEarley("((()()()))()()(((())))"));
  ASSERT_TRUE(parser.PredictEarley("()()()()()()()()()()()()()()()()"));
  ASSERT_TRUE(parser.PredictEarley(""));
  ASSERT_FALSE(parser.PredictEarley("("));
  ASSERT_FALSE(parser.PredictEarley("())"));
  ASSERT_FALSE(parser.PredictEarley("()()(()))("));
  ASSERT_FALSE(parser.PredictEarley(")("));

  // из одной из домашек
  parser = Earley("SA", "ab", {"S->AS", "S->b", "A->SA", "A->a", "S"});
  ASSERT_TRUE(parser.PredictEarley("bbaab"));
  ASSERT_TRUE(parser.PredictEarley("ababab"));
  ASSERT_FALSE(parser.PredictEarley("aabba"));

  // язык вида a^n*b^n
  parser = Earley("S", "ab", {"S->aSb", "S->", "S"});
  ASSERT_TRUE(parser.PredictEarley(""));
  ASSERT_TRUE(parser.PredictEarley("aabb"));
  ASSERT_TRUE(parser.PredictEarley("aaaabbbb"));
  ASSERT_FALSE(parser.PredictEarley("aabbb"));
  ASSERT_FALSE(parser.PredictEarley("aaaaabbbbba"));
  ASSERT_FALSE(parser.PredictEarley("aba"));
  ASSERT_FALSE(parser.PredictEarley("aaaabbb"));
}

std::string MakeBA(size_t num) {
  std::string word_a = "";
  std::string word_b = "";
  for (size_t i = 0; i < num; ++i) {
    word_a += 'a';
    word_b += 'b';
  }
  return word_b + word_a;
}

TEST(EarleyTests, StressTests) {
  const std::pair<int, int> range = {100, 150};
  const int big_test = 100000;

  // язык вида a^n*b^n
  Earley parser("S", "ab", {"S->bSa", "S->", "S"});
  // 50 тестов вида a^i*b^i, где i пробегает [range.first, range.second]
  for (size_t test_size = range.first; test_size <= range.second; ++test_size) {
    ASSERT_TRUE(parser.PredictEarley(MakeBA(test_size)));
  }

  // Большой тест a^i*b^i, где i=big_test
  ASSERT_TRUE(parser.PredictEarley(MakeBA(big_test)));
}