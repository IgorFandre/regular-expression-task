#include <gtest/gtest.h>

#include "../machine/machine.h"
#include "test_helper.h"

TEST(STRESS_TEST, deterministic_test) {
  std::mt19937 rnd(time(0));

  std::vector<char> alphabet = {
    'a', 'b', 'c', StateMachine::eps
  };

  for (int i = 0; i < 1000; i++) {
#ifdef DEBUG_
    std::cout << "i = " << i << std::endl;
#endif
    int state_num = rnd() % 5 + 1;
    StateMachine nondet(state_num);
    nondet.MakeStartState(0);

    StateMachine without_eps;
    StateMachine det;

    int trans_num = rnd() % 10 + 1;
    int from = 0;
    int to = rnd() % state_num;
    int alpha = rnd() % StateMachine::kAlphabetSize;
    for (int j = 0; j <= trans_num; j++) {
      nondet.MakeTransition(from, to, alphabet[alpha]);
      from = rnd() % state_num;
      to = rnd() % state_num;
      alpha = rnd() % StateMachine::kAlphabetSize;
    }

    nondet.MakeFinishStates({static_cast<int>(rnd() % state_num)});

    without_eps = nondet;
#ifdef DEBUG_
    std::cout << "Nondet " << "\n" << nondet << std::endl;
#endif
    without_eps.DeleteEpsTransitions();
#ifdef DEBUG_
    std::cout << "Without eps " << "\n" << without_eps << std::endl;
#endif
    det = without_eps.MakeDeterministicMachine();
#ifdef DEBUG_
    std::cout << "Det " << "\n" << det << std::endl;
#endif

    for (int j = 0; j < 100; ++j) {
      std::string word;
      int len = rnd() % 10;
      for (int k = 0; k < len; ++k) {
        word += alphabet[rnd() % StateMachine::kAlphabetSize];
      }
#ifdef DEBUG_
      std::cout << word << std::endl;
#endif
      bool b1 = nondet.FindWordInNonDeterministic(word);
#ifdef DEBUG_
      std::cout << "nondet.FindWordInNonDeterministic " << b1 << std::endl;
#endif
      bool b2 = without_eps.FindWordInNonDeterministic(word);
#ifdef DEBUG_
      std::cout << "without_eps.FindWordInNonDeterministic " << b2 << std::endl;
#endif
      bool b3 = det.FindWordInNonDeterministic(word);
#ifdef DEBUG_
      std::cout << "det.FindWordInNonDeterministic " << b3 << std::endl;
#endif
      bool b4 = det.FindWordInDeterministic(word);
#ifdef DEBUG_
      std::cout << "det.FindWordInDeterministic " << b4 << std::endl;
#endif
      if (!(b1 == b2 && b2 == b3 && b3 == b4)) {
        std::cout << "Nondet " << b1 << "\n" << nondet << std::endl;
        std::cout << "Without eps " << b2 << "\n" << without_eps << std::endl;
        std::cout << "Det " << b3 << " " << b4 << "\n" << det << std::endl;
        std::cout << "Word: " << word << std::endl;
        exit(1);
      }
    }
  }
}