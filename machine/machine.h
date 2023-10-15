#pragma once

#include <array>
#include <iostream>
#include <set>
#include <vector>

// #define DEBUG_

class StateMachine {
 public:
  constexpr static size_t kAlphabetSize = 4; // {a : 0, b : 1, c : 2, eps : 3}
  static constexpr char eps = 'a' + kAlphabetSize - 1;
  static constexpr char eps_idx = kAlphabetSize - 1;

  StateMachine();

  explicit StateMachine(char);

  StateMachine(int);

  StateMachine& operator=(const StateMachine&) = default;

  friend StateMachine operator+(const StateMachine&, const StateMachine&);

  friend StateMachine operator*(const StateMachine&, const StateMachine&);

  StateMachine operator++(int) const;

  void DeleteEpsTransitions();

  StateMachine MakeDeterministicMachine() const;

  bool FindWordInDeterministic(const std::string&) const;
  bool FindWordInDeterministicInSuffixes(const std::string&, int) const;

  bool FindWordInNonDeterministic(const std::string&) const;
  bool FindWordInNonDeterministicInSuffixes(const std::string&, int) const;

  void MakeTransition(int, int, char);
  void MakeStartState(int);
  void MakeFinishStates(const std::vector<int>&);

  int GetStates() const;
  bool CheckTransition(int, int, char) const;
  int GetStartState() const;
  std::set<int> GetFinishStates() const;

  friend std::ostream& operator<<(std::ostream&, const StateMachine&);

 private:
  bool FindWordInDeterministicFromVert(const std::string&, int) const;
  bool FindWordInNonDeterministicFromVert(const std::string&, int) const;

  void ResizeTransitions(int);

  bool DFSFind(int, const std::string&, int, int, int, std::vector<bool>&) const;

  int states_;
  std::vector<std::vector<std::array<bool, kAlphabetSize>>> transitions_;
  int start_state_;
  std::set<int> finish_states_;
};
