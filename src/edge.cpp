#include"edge.hpp"

edge::edge(uint local_register_endpoint_index, const rbg_parser::pure_game_move* label_condition):
local_register_endpoint_index(local_register_endpoint_index),
label_condition(label_condition){
}

void edge::shift(uint shift_value){
    local_register_endpoint_index += shift_value;
}
