#include "../machine/machine.h"
#include "test_helper.h"

StateMachine MakeRandomMachine(int max_states, int max_trans) {
  std::mt19937 rnd(time(0));

  std::vector<char> alphabet = {
    'a', 'b', 'c', StateMachine::eps
  };

  int state_num = rnd() % max_states + 1;
  StateMachine machine(state_num);
  machine.MakeStartState(0);

  int trans_num = rnd() % 10 + 1;
  int from = 0;
  int to = rnd() % state_num;
  int alpha = rnd() % StateMachine::kAlphabetSize;
  for (int j = 0; j <= trans_num; j++) {
    machine.MakeTransition(from, to, alphabet[alpha]);
    from = rnd() % state_num;
    to = rnd() % state_num;
    alpha = rnd() % StateMachine::kAlphabetSize;
  }

  int finish_num = rnd() % state_num + 1;
  std::vector<int> finish_conds;
  for (int j = 0; j < finish_num; ++j) {
    finish_conds.push_back(rnd() % state_num);
  }
  machine.MakeFinishStates(finish_conds);
  return machine;
}