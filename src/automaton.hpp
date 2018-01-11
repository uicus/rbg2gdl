#ifndef AUTOMATON
#define AUTOMATON

#include<vector>

#include"state.hpp"
#include"pure_game_move.hpp"
#include"types.hpp"

class automaton{
        std::vector<state> local_register;
        uint start_state;
        uint accept_state;
        std::pair<uint,uint> place_side_by_side(automaton&& rhs);
        std::pair<uint,uint> prepare_new_endpoints(void);
    public:
        void add_automaton(automaton&& rhs);
        void starify_automaton(void);
        friend automaton pure_letter_automaton(const rbg_parser::pure_game_move* label);
        friend automaton modifier_automaton(const rbg_parser::game_move* action);
};

automaton pure_letter_automaton(const rbg_parser::pure_game_move* label);
automaton modifier_automaton(const rbg_parser::game_move* action);

#endif
