#include"state.hpp"
#include<cassert>

uint state::next_id = 0;

state::state(const rbg_parser::game_move* action):
action(action),
id(next_id++),
next_states(){
}

state::state(const state& s):
action(s.action),
id(next_id++),
next_states(s.next_states){
}

state& state::operator=(const state& s){
    action = s.action;
    id = next_id++;
    next_states = s.next_states;
    return *this;
}

uint state::get_id(void)const{
    return id;
}

void state::inform_about_being_appended(uint shift_value){
    for(auto& el: next_states)
        el.shift(shift_value);
}

void state::inform_about_state_deletion(uint deleted_index){
    for(auto& el: next_states)
        el.inform_abut_state_deletion(deleted_index);
}

void state::connect_with_state(uint index_in_local_register, const rbg_parser::pure_game_move* label_condition){
    next_states.push_back(edge(index_in_local_register,label_condition));
}

bool state::modifier(void)const{
    return action!=nullptr;
}

void state::absorb(state&& rhs){
    assert(!modifier() || !rhs.modifier());
    assert(next_states.empty());
    if(!rhs.next_states.empty()){
        next_states = std::move(rhs.next_states);
        rhs.next_states.clear();
    }
    if(action==nullptr)
        action = rhs.action;
}
