#include <iostream>

#include "machine/machine.h"
#include "regular/regular.h"

int main() {
  RegularExpr regex;
  std::string word;
  std::cin >> regex >> word;
  int result = regex.FindSuffixInRegularSuffixes(word);
  std::cout << result << " : " << word.substr(static_cast<int>(word.size()) - result, result) << "\n";
  return 0;
}