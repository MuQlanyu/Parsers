#include "src/Parser.h"
#include "src/Earley/earley.h"

int main() {
  Earley parser;
  parser.ReadGrammar();
  parser.Fit();
  size_t test_num;
  std::cin >> test_num;
  std::string word;
  for (size_t i = 0; i < test_num; ++i) {
    std::cin >> word;
    word = (word == "&" ? "" : word);
    std::cout << (parser.PredictEarley(word) ? "Yes" : "No") << '\n';
  }
}
