#include <gtest/gtest.h>

#include "../machine/machine.h"
#include "test_helper.h"

TEST(STRESS_TEST, operation_plus_test) {
  std::mt19937 rnd(time(0));

  std::vector<char> alphabet = {
    'a', 'b', 'c', StateMachine::eps
  };

  for (int i = 0; i < 1000; i++) {
#ifdef DEBUG_
    std::cout << "i = " << i << std::endl;
#endif
    StateMachine machine_1 = MakeRandomMachine();
    StateMachine machine_2 = MakeRandomMachine();

    StateMachine machine_3 = machine_1 + machine_2;
    StateMachine machine_4 = machine_2 + machine_1;

    for (int j = 0; j < 300; ++j) {
      std::string word;
      int len = rnd() % 10;
      for (int k = 0; k < len; ++k) {
        word += alphabet[rnd() % StateMachine::kAlphabetSize];
      }
#ifdef DEBUG_
      std::cout << word << std::endl;
#endif
      bool b1 = machine_1.FindWordInNonDeterministic(word);
#ifdef DEBUG_
      std::cout << "machine_1.FindWordInNonDeterministic " << b1 << std::endl;
#endif
      bool b2 = machine_2.FindWordInNonDeterministic(word);
#ifdef DEBUG_
      std::cout << "machine_2.FindWordInNonDeterministic " << b2 << std::endl;
#endif
      bool b3 = machine_3.FindWordInNonDeterministic(word);
#ifdef DEBUG_
      std::cout << "machine_3.FindWordInNonDeterministic " << b3 << std::endl;
#endif
      bool b4 = machine_4.FindWordInNonDeterministic(word);
#ifdef DEBUG_
      std::cout << "machine_4.FindWordInDeterministic " << b4 << std::endl;
#endif
      if (!((b1 || b2) == b3 && (b1 || b2) == b4)) {
        std::cout << "machine_1 " << b1 << "\n" << machine_1 << std::endl;
        std::cout << "machine_2 " << b2 << "\n" << machine_2 << std::endl;
        std::cout << "machine_1 + machine_2 " << b3 << "\n" << machine_3 << std::endl;
        std::cout << "machine_2 + machine_1 " << b4 << "\n" << machine_4 << std::endl;
        std::cout << "Word: " << word << std::endl;
        exit(1);
      }
    }
  }
}

TEST(STRESS_TEST, operation_mult_test) {
  std::mt19937 rnd(time(0));

  std::vector<char> alphabet = {
    'a', 'b', 'c', StateMachine::eps
  };

  for (int i = 0; i < 1000; i++) {
#ifdef DEBUG_
    std::cout << "i = " << i << std::endl;
#endif
    StateMachine machine_1 = MakeRandomMachine();
    StateMachine machine_2 = MakeRandomMachine();

    StateMachine machine_3 = machine_1 * machine_2;
    StateMachine machine_4 = machine_2 * machine_1;

    for (int j = 0; j < 300; ++j) {
      std::string word;
      int len = rnd() % 10;
      for (int k = 0; k < len; ++k) {
        word += alphabet[rnd() % StateMachine::kAlphabetSize];
      }

      bool b1 = false;
      bool b2 = false;
      for (int j = 0; j <= len; ++j) {
        b1 = b1 || (machine_1.FindWordInNonDeterministic(word.substr(0, j)) && machine_2.FindWordInNonDeterministic(word.substr(j, word.size() - j)));
        b2 = b2 || (machine_2.FindWordInNonDeterministic(word.substr(0, j)) && machine_1.FindWordInNonDeterministic(word.substr(j, word.size() - j)));
      }

      bool b3 = machine_3.FindWordInNonDeterministic(word);
      bool b4 = machine_4.FindWordInNonDeterministic(word);

      if (!(b1 == b3 && b2 == b4)) {
        std::cout << "machine_1 * machine_2 (loop) " << b1 << "\n" << machine_1 << std::endl;
        std::cout << "machine_2 * machine_1 (loop) " << b2 << "\n" << machine_2 << std::endl;
        std::cout << "machine_1 * machine_2 " << b3 << "\n" << machine_3 << std::endl;
        std::cout << "machine_2 * machine_1 " << b4 << "\n" << machine_4 << std::endl;
        std::cout << "Word: " << word << std::endl;
        exit(1);
      }
    }
  }
}

TEST(STRESS_TEST, operation_klini_test) {
  std::mt19937 rnd(time(0));

  std::vector<char> alphabet = {
    'a', 'b', 'c', StateMachine::eps
  };

  for (int i = 0; i < 1000; i++) {
#ifdef DEBUG_
    std::cout << "i = " << i << std::endl;
#endif
    StateMachine machine_1 = MakeRandomMachine();

    StateMachine machine_2 = machine_1++;

    for (int j = 0; j < 300; ++j) {
      std::string word;
      int len = 1;
      for (int k = 0; k < len; ++k) {
        word += alphabet[rnd() % StateMachine::kAlphabetSize];
      }

      bool b1 = machine_1.FindWordInNonDeterministic(word);
      bool b2 = machine_2.FindWordInNonDeterministic("");
      bool b3 = machine_2.FindWordInNonDeterministic(word);
      bool b4 = machine_2.FindWordInNonDeterministic(word + word);
      bool b5 = machine_2.FindWordInNonDeterministic(word + word + word);

      if (b1 != b3 || !b2) {
        std::cout << "machine_1 " << b1 << "\n" << machine_1 << std::endl;
        std::cout << "machine_2 " << b2 << " " << b3 << "\n" << machine_2 << std::endl;
        std::cout << "Word: " << word << std::endl;
      }

      ASSERT_TRUE(b2);
      ASSERT_EQ(b1, b3);
      if (b3) {
        ASSERT_TRUE(b4);
        ASSERT_TRUE(b5);
      }
    }
  }
}