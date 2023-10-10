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

std::istream& operator>>(std::istream& in, RegularExpr& regex) {
  in >> regex.regexpr_;
  return in;
}