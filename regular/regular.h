#pragma once

#include <iostream>
#include <stack>
#include <string>

#include "regular.h"
#include "../machine/machine.h"

class RegularExpr {
 public:
  RegularExpr() = default;
  RegularExpr(const std::string&);

  StateMachine ParseRegular() const;
  int FindSuffixInRegularSuffixes(const std::string&) const;

  friend std::istream& operator>>(std::istream&, RegularExpr&);

 private:
  std::string regexpr_;
};