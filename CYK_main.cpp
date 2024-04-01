#include "src/Parser.h"
#include "src/CYK/cyk_algo.h"

int main() {
  CYK parser;
  parser.ReadGrammar();
  parser.Fit();
  size_t test_num;
  std::cin >> test_num;
  std::string word;
  for (size_t i = 0; i < test_num; ++i) {
    std::cin >> word;
    word = (word == "&" ? "" : word);
    std::cout << (parser.Predict(word) ? "Yes" : "No") << '\n';
  }
}
