#include <gtest/gtest.h>

#include "../regular/regular.h"
#include "test_helper.h"

TEST(BASIC_TEST, first) {
  RegularExpr reg("ab+c.aba.*.bac.+.+*");
  std::string word = "babc";
  ASSERT_EQ(reg.FindSuffixInRegularSuffixes(word), 2);
}

TEST(BASIC_TEST, second) {
  RegularExpr reg("acb..bab.c.*.ab.ba.+.+*a.");
  std::string word = "cbaa";
  ASSERT_EQ(reg.FindSuffixInRegularSuffixes(word), 4);
}

TEST(BASIC_TEST, third) {
  RegularExpr reg("ab.1.c.1.c.");
  std::string word = "bcc";
  ASSERT_EQ(reg.FindSuffixInRegularSuffixes(word), 3);
}