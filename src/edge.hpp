#ifndef EDGE
#define EDGE

#include"pure_game_move.hpp"

class state;

class edge{
        uint local_register_endpoint_index;
        const rbg_parser::pure_game_move* label_condition; // doesn't own
    public:
        edge(uint local_register_endpoint_index, const rbg_parser::pure_game_move* label_condition);
        void shift(uint shift_value);
        void inform_abut_state_deletion(uint deleted_index);
        const state& get_next_state(const std::vector<state>& local_register)const;
        const rbg_parser::pure_game_move* get_label(void)const;
};

#endif
