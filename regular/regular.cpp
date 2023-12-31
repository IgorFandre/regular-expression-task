#include "regular.h"

RegularExpr::RegularExpr(const std::string& regexpr) : regexpr_(regexpr) {}

StateMachine RegularExpr::ParseRegular() const {
  std::stack<StateMachine> st;
  for (int i = 0; i < static_cast<int>(regexpr_.size()); ++i) {
    if (regexpr_[i] == '.') {
      StateMachine st_last = st.top();
      st.pop();
      StateMachine st_prev = st.top();
      st.pop();
      st.push(st_prev * st_last);
    }
    else if (regexpr_[i] == '*') {
      StateMachine st_last = st.top();
      st.pop();
      st.push(st_last++);
    }
    else if (regexpr_[i] == '+') {
      StateMachine st_last = st.top();
      st.pop();
      StateMachine st_prev = st.top();
      st.pop();
      st.push(st_prev + st_last);
    }
    else if (regexpr_[i] == '1') {
      st.push(StateMachine(StateMachine::eps));
    }
    else {
      st.push(StateMachine(regexpr_[i]));
    }
  }
  return st.top();
}

// Функция проходится по всем суффиксам слова и запускает функцию поиска среди суффиксов регулярного выражения.
// Останавливается на первом найденном суффиксе.
int RegularExpr::FindSuffixInRegularSuffixes(const std::string& word) const {
  StateMachine regex_machine = ParseRegular();
  for (int i = 0; i < static_cast<int>(word.size()); ++i) {
    bool find = regex_machine.FindWordInNonDeterministicInSuffixes(word, i);
    if (find) {
        return static_cast<int>(word.size()) - i;
    }
  }
  return 0;
}

std::istream& operator>>(std::istream& in, RegularExpr& regex) {
  in >> regex.regexpr_;
  return in;
}