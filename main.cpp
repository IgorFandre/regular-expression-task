#include <iostream>

#include "machine/machine.h"
#include "regular/regular.h"

int DoTask(RegularExpr& regex, const std::string& word) {
  StateMachine regex_machine = regex.ParseRegular();
  std::cout << "regex_machine \n" << regex_machine << std::endl;
  regex_machine.DeleteEpsTransitions();
  StateMachine determ_regex = regex_machine.MakeDeterministicMachine();
  std::cout << "determ_regex \n" << determ_regex << std::endl;
  for (int i = 0; i < static_cast<int>(word.size()); ++i) {
    bool find = determ_regex.FindWordInDeterministicInSuffixes(word, i);
    if (find) {
        return static_cast<int>(word.size()) - i;
    }
  }
  return 0;
}

int main() {
  RegularExpr regex;
  std::string word;
  std::cin >> regex >> word;
  int result = DoTask(regex, word);
  std::cout << result << " : " << word.substr(static_cast<int>(word.size()) - result, result) << "\n";
  return 0;
}