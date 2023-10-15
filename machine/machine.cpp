#include <algorithm>
#include <assert.h>
#include <string>
#include <map>
#include <queue>

#include "machine.h"

StateMachine::StateMachine() : states_(1),
                               transitions_(states_, std::vector<std::array<bool, kAlphabetSize>>(states_)),
                               start_state_(0) {
  transitions_[0][0].fill(false);
}
StateMachine::StateMachine(char a) : states_{2},
                                     transitions_(states_, std::vector<std::array<bool, kAlphabetSize>>(states_)),
                                     start_state_{0} {
  for (int i = 0; i < states_; ++i) {
    for (int j = 0; j < states_; ++j) {
      transitions_[i][j].fill(false);
    }
  }
  if (a == '0') {
    return;
  }
  if (a == '1') {
    transitions_[0][1][eps_idx] = true;
  } else {
    transitions_[0][1][a - 'a'] = true;
  }
  finish_states_.emplace(1);
}
StateMachine::StateMachine(int states) : states_(states),
                                         transitions_(states_, std::vector<std::array<bool, kAlphabetSize>>(states_)),
                                         start_state_{0} {
  for (int i = 0; i < states_; ++i) {
    for (int j = 0; j < states_; ++j) {
      transitions_[i][j].fill(false);
    }
  }
}

StateMachine operator+(const StateMachine& st1, const StateMachine& st2) {
  int new_st_states = 2 + st1.states_ + st2.states_;
  StateMachine new_st(new_st_states);
  new_st.finish_states_.emplace(new_st_states - 1);

  new_st.transitions_[0][1 + st1.start_state_][StateMachine::eps_idx] = true;
  new_st.transitions_[0][1 + st1.states_ + st2.start_state_][StateMachine::eps_idx] = true;

  for (int i = 0; i < st1.states_; ++i) {
    for (int j = 0; j < st1.states_; ++j) {
      for (size_t k = 0; k < StateMachine::kAlphabetSize; ++k) {
        new_st.transitions_[1 + i][j + 1][k] = st1.transitions_[i][j][k];
      }
    }
  }
  for (int i = 0; i < st2.states_; ++i) {
    for (int j = 0; j < st2.states_; ++j) {
      for (size_t k = 0; k < StateMachine::kAlphabetSize; ++k) {
        new_st.transitions_[1 + st1.states_ + i][1 + st1.states_ + j][k] = st2.transitions_[i][j][k];
      }
    }
  }

  for (int finish : st1.finish_states_) {
    new_st.transitions_[1 + finish][new_st_states - 1][StateMachine::eps_idx] = true;
  }
  for (int finish : st2.finish_states_) {
    new_st.transitions_[1 + st1.states_ + finish][new_st_states - 1][StateMachine::eps_idx] = true;
  }

  return new_st;
}

StateMachine operator*(const StateMachine& st1, const StateMachine& st2) {
  int new_st_states = st1.states_ + st2.states_;
  StateMachine new_st(new_st_states);

  new_st.start_state_ = st1.start_state_;
  for (int finish : st2.finish_states_) {
    new_st.finish_states_.emplace(st1.states_ + finish);
  }

  for (int finish : st1.finish_states_) {
    new_st.transitions_[finish][st1.states_ + st2.start_state_][StateMachine::eps_idx] = true;
  }

  for (int i = 0; i < st1.states_; ++i) {
    for (int j = 0; j < st1.states_; ++j) {
      for (size_t k = 0; k < StateMachine::kAlphabetSize; ++k) {
        new_st.transitions_[i][j][k] = st1.transitions_[i][j][k];
      }
    }
  }
  for (int i = 0; i < st2.states_; ++i) {
    for (int j = 0; j < st2.states_; ++j) {
      for (size_t k = 0; k < StateMachine::kAlphabetSize; ++k) {
        new_st.transitions_[st1.states_ + i][st1.states_ + j][k] = st2.transitions_[i][j][k];
      }
    }
  }

  return new_st;
}

StateMachine StateMachine::operator++(int) const {
  int new_st_states = states_;
  StateMachine new_st(1 + new_st_states);

  new_st.start_state_ = 0;
  new_st.finish_states_.emplace(new_st.start_state_);

  for (int i = 0; i < states_; ++i) {
    for (int j = 0; j < states_; ++j) {
      for (int k = 0; k < kAlphabetSize; ++k) {
        new_st.transitions_[i + 1][j + 1][k] = transitions_[i][j][k];
      }
    }
  }

  new_st.transitions_[new_st.start_state_][start_state_ + 1][eps_idx] = true;
  for (int finish : finish_states_) {
    new_st.transitions_[finish + 1][new_st.start_state_][eps_idx] = true;
  }

  return new_st;
}

void StateMachine::DeleteEpsTransitions() {
  struct Edge {
    int from;
    int to;
  };
  std::vector<Edge> eps_transitions;
  for (int i = 0; i < states_; ++i) {
    for (int j = 0; j < states_; ++j) {
      if (transitions_[i][j][eps_idx]) {
        eps_transitions.emplace_back(i, j);
      }
    }
  }

  for (int times = 0; times < states_; ++times) {
    for (const auto& edge : eps_transitions) {
      if (edge.from == edge.to) {
        continue;
      }
      if (finish_states_.find(edge.to) != finish_states_.end()) {
        finish_states_.emplace(edge.from);
      }
      for (int i = 0; i < states_; ++i) {
        for (int alpha = 0; alpha < kAlphabetSize - 1; ++alpha) {
          transitions_[i][edge.to][alpha] |= transitions_[i][edge.from][alpha];
          transitions_[edge.from][i][alpha] |= transitions_[edge.to][i][alpha];
        }
      }
    }
  }
  for (const auto& edge : eps_transitions) {
    transitions_[edge.from][edge.to][eps_idx] = false;
  }
}

void StateMachine::ResizeTransitions(int new_states) {
  assert(new_states >= states_);
  std::vector<std::vector<std::array<bool, kAlphabetSize>>> new_transitions(new_states, std::vector<std::array<bool, kAlphabetSize>>(new_states));
  for (int i = 0; i < std::min(states_, new_states); ++i) {
    for  (int j = 0; j < std::min(states_, new_states); ++j) {
      for (int alpha = 0; alpha < kAlphabetSize; ++alpha) {
        new_transitions[i][j][alpha] = transitions_[i][j][alpha];
      }
    }
  }
  states_ = new_states;
  transitions_ = new_transitions;
}

std::string vec_to_string(std::vector<int>& elem_vec) {
  std::string result;
  for (int idx = 0; idx < elem_vec.size(); ++idx) {
    result += std::to_string(elem_vec[idx]) + ",";
  }
#ifdef DEBUG_
  std::cout << result << std::endl;
#endif
  return result;
}

StateMachine StateMachine::MakeDeterministicMachine() const {
  StateMachine new_st;

  for (int finish : finish_states_) {
    if (finish == 0) {
      new_st.finish_states_.emplace(finish);
    }
  }

  std::queue<std::vector<int>> bfs;
  std::vector<int> start_state_new(1, start_state_);
  bfs.push(start_state_new);

  std::map<std::string, int> new_states_map;
  new_states_map[vec_to_string(start_state_new)] = new_st.start_state_;

  while (!bfs.empty()) {
    std::vector<int> prev_conds = bfs.front();
    bfs.pop();

    for (int alpha = 0; alpha < kAlphabetSize - 1; ++alpha) {
      std::set<int> new_conds;

      for (int cond : prev_conds) {
        for (int to = 0; to < states_; ++to) {
          if (transitions_[cond][to][alpha]) {
            new_conds.emplace(to);
          }
        }
      }
      if (new_conds.empty()) {
        continue;
      }

      std::vector<int> elem_vec;
      for (int elem : new_conds) {
        elem_vec.push_back(elem);
      }
      std::sort(elem_vec.begin(), elem_vec.end());
      std::string new_conds_str = vec_to_string(elem_vec);

      if (new_states_map.find(new_conds_str) == new_states_map.end()) {
        bfs.push(elem_vec);
        new_states_map[new_conds_str] = new_states_map.size();
        new_st.ResizeTransitions(new_st.states_ + 1);
        for (int cond : new_conds) {
          if (finish_states_.find(cond) != finish_states_.end()) {
            new_st.finish_states_.emplace(new_states_map[new_conds_str]);
            break;
          }
        }
      }

      new_st.transitions_[new_states_map[vec_to_string(prev_conds)]][new_states_map[new_conds_str]][alpha] = true;
    }
  }

  return new_st;
}

bool StateMachine::FindWordInDeterministicFromVert(const std::string& word, int vert) const {
  for (int i = 0; i < static_cast<int>(word.size()); ++i) {
    bool found = false;
    for (int to = 0; to < states_; ++to) {
      if (transitions_[vert][to][word[i] - 'a']) {
        vert = to;
        found = true;
        break;
      }
    }
    if (!found) {
      return false;
    }
  }
  return finish_states_.find(vert) != finish_states_.end();
}

bool StateMachine::FindWordInDeterministic(const std::string& word) const {
  return FindWordInDeterministicFromVert(word, start_state_);
}

bool StateMachine::FindWordInDeterministicInSuffixes(const std::string& word, int start_idx = 0) const {
  std::string suffix = word.substr(start_idx, static_cast<int>(word.size()) - start_idx);
  for (int vert = 0; vert < states_; ++vert) {
    bool find = FindWordInDeterministicFromVert(suffix, vert);
    if (find) {
      return true;
    }
  }
  return false;
}

bool StateMachine::DFSFind(int vert, const std::string& word, int word_idx, int depth, int max_depth, std::vector<bool>& used) const {
  if (depth > max_depth || used[vert]) {
    return false;
  }
  used[vert] = true;
#ifdef DEBUG_
  std::cout << "DFS: " << vert << " " << word_idx << " " << depth << " " << max_depth << std::endl;
#endif
  bool found = false;
  for (int i = 0; i < states_; ++i) {
    for (int alpha = 0; alpha < kAlphabetSize; ++alpha) {
      if (!transitions_[vert][i][alpha]) {
        continue;
      }
      if (alpha == eps_idx) {
        found = found || DFSFind(i, word, word_idx, depth + 1, max_depth, used);
      } else if (word_idx < static_cast<int>(word.size()) && alpha + 'a' == word[word_idx]) {
        std::vector<bool> new_used(states_, false);
        found = found || DFSFind(i, word, word_idx + 1, depth + 1, max_depth, new_used);
      }
    }
    if (found) {
      return true;
    }
  }
  if (word_idx == static_cast<int>(word.size())) {
    return finish_states_.find(vert) != finish_states_.end();
  }
  return false;
}

bool StateMachine::FindWordInNonDeterministicFromVert(const std::string& word, int vert) const {
  std::vector<bool> used(states_, false);
  return DFSFind(vert, word, 0, 0, (static_cast<int>(word.size()) + 1) * states_, used);
}

bool StateMachine::FindWordInNonDeterministic(const std::string& word) const {
  return FindWordInNonDeterministicFromVert(word, start_state_);
}

bool StateMachine::FindWordInNonDeterministicInSuffixes(const std::string& word, int start_idx = 0) const {
  std::string suffix = word.substr(start_idx, static_cast<int>(word.size()) - start_idx);
  for (int vert = 0; vert < states_; ++vert) {
    bool find = FindWordInNonDeterministicFromVert(suffix, vert);
    if (find) {
      return true;
    }
  }
  return false;
}

void StateMachine::MakeTransition(int from, int to, char alpha) {
  if (alpha == eps) {
    transitions_[from][to][eps_idx] = true;
  } else {
    transitions_[from][to][alpha - 'a'] = true;
  }
}
void StateMachine::MakeStartState(int new_start) {
  start_state_ = new_start;
}
void StateMachine::MakeFinishStates(const std::vector<int>& finish_states) {
  for (int finish : finish_states) {
    finish_states_.insert(finish);
  }
}

int StateMachine::GetStates() const {
  return states_;
}
bool StateMachine::CheckTransition(int from, int to, char alpha) const {
  return transitions_[from][to][alpha];
}
int StateMachine::GetStartState() const {
  return start_state_;
}
std::set<int> StateMachine::GetFinishStates() const {
  return finish_states_;
}

std::ostream& operator<<(std::ostream& os, const StateMachine& machine) {
  os << "States: " << machine.states_ << "\nStart state: " << machine.start_state_ << "\nFinish state: ";
  for (int finish : machine.finish_states_) {
    os << finish << " ";
  }
  os << "\nTransitions:\n";
  for (int i = 0; i < machine.states_; ++i) {
    for (int j = 0; j < machine.states_; ++j) {
      for (int k = 0; k < StateMachine::kAlphabetSize; ++k) {
        if (machine.transitions_[i][j][k]) {
          std::cout << "From " << i << " to " << j << " -- " << k << "\n";
        }
      }
    }
  }
  os << "------------------------------" << std::endl;
  return os;
}