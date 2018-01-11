#include"automaton.hpp"

automaton pure_letter_automaton(const rbg_parser::pure_game_move* label){
    automaton result;
    result.local_register.push_back(state());
    result.local_register.push_back(state());
    result.local_register[0].connect_with_state(1,label);
    result.start_state = 0;
    result.accept_state = 1;
    return result;
}

automaton modifier_automaton(const rbg_parser::game_move* action){
    automaton result;
    result.local_register.push_back(state(action));
    result.start_state = result.accept_state = 0;
    return result;
}

std::pair<uint,uint> automaton::place_side_by_side(automaton&& rhs){
    for(auto& el: rhs.local_register)
        el.inform_about_being_appended(local_register.size());
    uint appendee_start = local_register.size() + rhs.start_state;
    uint appendee_accept = local_register.size() + rhs.accept_state;
    local_register.reserve(local_register.size() + rhs.local_register.size());
    std::move(std::begin(rhs.local_register), std::end(rhs.local_register), std::back_inserter(local_register));
    rhs.local_register.clear();
    return std::make_pair(appendee_start,appendee_accept);
}

std::pair<uint,uint> automaton::prepare_new_endpoints(void){
    uint new_start_state = local_register.size();
    local_register.push_back(state(nullptr));
    uint new_accept_state = local_register.size();
    local_register.push_back(state(nullptr));
    return std::make_pair(new_start_state,new_accept_state);
}

void automaton::add_automaton(automaton&& rhs){
    auto new_endpoints = prepare_new_endpoints();
    auto old_automaton_endpoints = place_side_by_side(std::move(rhs));
    local_register[new_endpoints.first].connect_with_state(start_state);
    local_register[new_endpoints.first].connect_with_state(old_automaton_endpoints.first);
    local_register[accept_state].connect_with_state(new_endpoints.second);
    local_register[old_automaton_endpoints.second].connect_with_state(new_endpoints.second);
    start_state = new_endpoints.first;
    accept_state = new_endpoints.second;
}

void automaton::starify_automaton(void){
    auto new_endpoints = prepare_new_endpoints();
    local_register[new_endpoints.first].connect_with_state(start_state);
    local_register[accept_state].connect_with_state(new_endpoints.second);
    local_register[new_endpoints.first].connect_with_state(new_endpoints.second);
    local_register[accept_state].connect_with_state(start_state);
    start_state = new_endpoints.first;
    accept_state = new_endpoints.second;
}
