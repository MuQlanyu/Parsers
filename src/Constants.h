#pragma once
#include <iostream>
#include <vector>

namespace Constants {
const std::pair<int, int> kNonTerminals = {65, 90};  // коды ascii для A-Z
const std::pair<int, int> kTerminals = {97, 122}; // коды ascii для a-z
const std::pair<int, int> kNumbers = {48, 57}; // коды ascii для 0-9
const std::vector<int> kServiceSymbols = {37, 40, 41, 42, 43, 45, 47, 91, 93, 123, 125};
const std::string SymbCombined = "_";
const std::string SymbLong = "L";
const std::string SymbStart = "'";
const std::string Arrow = "->";
}

namespace Strings {
const std::string ErrorInvalidNumberOfArg = "Количество введенных данных отличается от заявленного";
const std::string ErrorInvalidNonTerminal = "Введен некорректный нетерминал";
const std::string ErrorInvalidTerminal = "Введен некорректный терминал";
const std::string ErrorInvalidRule = "Введен некорректное правило";
const std::string ErrorInvalidStart = "Введен некорректный начальный символ";

const std::string EarleyStartSymbol = "@";
const std::string EarleyEndSituation = "$";
}
