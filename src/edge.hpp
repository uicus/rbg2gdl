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
};

#endif
