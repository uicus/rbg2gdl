#include"edge.hpp"
#include"state.hpp"

edge::edge(uint local_register_endpoint_index, const rbg_parser::pure_game_move* label_condition):
local_register_endpoint_index(local_register_endpoint_index),
label_condition(label_condition){
}

void edge::shift(uint shift_value){
    local_register_endpoint_index += shift_value;
}

void edge::inform_abut_state_deletion(uint deleted_index){
    if(local_register_endpoint_index>deleted_index)
        --local_register_endpoint_index;
}

const state& edge::get_next_state(const std::vector<state>& local_register)const{
    return local_register[local_register_endpoint_index];
}

const rbg_parser::pure_game_move* edge::get_label(void)const{
    return label_condition;
}
