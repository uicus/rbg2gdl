#ifndef AUTOMATON
#define AUTOMATON

#include<vector>

#include"state.hpp"

class automaton{
        std::vector<state> local_register;
        uint start_state;
        uint accept_state;
    public:
        automaton(void)=default; // TODO: one automaton for every type of letter
        ~automaton(void)=default;
};

#endif
