#pragma once

#include <iostream>
#include <random>

#include "../machine/machine.h"

StateMachine MakeRandomMachine(int max_states = 5, int max_trans = 10);