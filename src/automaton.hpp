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
        void set_endpoints(const std::pair<uint,uint>& new_endpoints);
        void concat_automaton(automaton&& concatee);
        void concat_automaton_by_epsilon(automaton&& concatee);
        void concat_automaton_by_start_state(automaton&& concatee);
        std::string turn_changers_to_gdl(void)const;
    public:
        void starify_automaton(void);
        void repeat_automaton(uint times);
        std::string transitions_to_gdl(void);
        friend automaton pure_letter_automaton(const rbg_parser::pure_game_move* label);
        friend automaton modifier_automaton(const rbg_parser::game_move* action);
        friend automaton sum_of_automatons(std::vector<automaton>&& elements);
        friend automaton concatenation_of_automatons(std::vector<automaton>&& elements);
};

automaton pure_letter_automaton(const rbg_parser::pure_game_move* label);
automaton modifier_automaton(const rbg_parser::game_move* action);
automaton sum_of_automatons(std::vector<automaton>&& elements);
automaton concatenation_of_automatons(std::vector<automaton>&& elements);

#endif
