#include"automaton.hpp"
#include"standalone_moves_printer.hpp"
#include"gdl_constants.hpp"
#include<cassert>
#include<map>
#include<set>

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

void automaton::set_endpoints(const std::pair<uint,uint>& new_endpoints){
    start_state = new_endpoints.first;
    accept_state = new_endpoints.second;
}

void automaton::starify_automaton(void){
    auto new_endpoints = prepare_new_endpoints();
    local_register[new_endpoints.first].connect_with_state(start_state);
    local_register[accept_state].connect_with_state(new_endpoints.second);
    local_register[new_endpoints.first].connect_with_state(new_endpoints.second);
    local_register[accept_state].connect_with_state(start_state);
    set_endpoints(new_endpoints);
}

automaton sum_of_automatons(std::vector<automaton>&& elements){
    automaton result;
    auto result_endpoints = result.prepare_new_endpoints();
    result.set_endpoints(result_endpoints);
    for(auto& el: elements){
        auto old_endpoints = result.place_side_by_side(std::move(el));
        result.local_register[result_endpoints.first].connect_with_state(old_endpoints.first);
        result.local_register[old_endpoints.second].connect_with_state(result_endpoints.second);
    }
    elements.clear();
    return result;
}

void automaton::concat_automaton_by_epsilon(automaton&& concatee){
    auto old_endpoints = place_side_by_side(std::move(concatee));
    local_register[accept_state].connect_with_state(old_endpoints.first);
    accept_state = old_endpoints.second;
}

void automaton::concat_automaton_by_start_state(automaton&& concatee){
    uint old_start = concatee.start_state;
    for(auto& el: concatee.local_register){
        el.inform_about_state_deletion(old_start);
        el.inform_about_being_appended(local_register.size());
    }
    local_register[accept_state].absorb(std::move(concatee.local_register[old_start]));
    if(concatee.accept_state>concatee.start_state)
        accept_state = concatee.accept_state+local_register.size()-1;
    else if(concatee.accept_state<concatee.start_state)
        accept_state = concatee.accept_state+local_register.size();
    local_register.reserve(local_register.size() + concatee.local_register.size()-1);
    std::move(std::begin(concatee.local_register), std::begin(concatee.local_register)+old_start, std::back_inserter(local_register));
    std::move(std::begin(concatee.local_register)+old_start+1, std::end(concatee.local_register), std::back_inserter(local_register));
}

void automaton::concat_automaton(automaton&& concatee){
    if(concatee.local_register[concatee.start_state].modifier() && local_register[accept_state].modifier())
        concat_automaton_by_epsilon(std::move(concatee));
    else
        concat_automaton_by_start_state(std::move(concatee));
}

automaton concatenation_of_automatons(std::vector<automaton>&& elements){
    auto result = modifier_automaton(nullptr);
    for(auto& el: elements)
        result.concat_automaton(std::move(el));
    return result;
}

void automaton::repeat_automaton(uint times){
    assert(times>1);
    for(uint i=0;i<times-1;++i)
        concat_automaton(automaton(*this));
}

std::string automaton::effects_to_gdl(void){
    std::string result;
    result += "("+accept+" "+std::to_string(local_register[accept_state].get_id())+")\n";
    result += "(<= ("+finisher+" ?q)\n    ("+next_player+" ?q ?player))\n";
    result += "(<= ("+finisher+" ?q)\n    ("+accept+" ?q))\n";
    result += "\n";
    for(const auto& el: local_register)
        result += el.write_effect();
    return result;
}

std::string automaton::transitions_to_gdl(void){
    std::string result;
    result += "(<= ("+transition+" ?s1 ?x ?y ?s2 ?x ?y)\n";
    result += "    (epsilon ?s1 ?s2)\n    (file ?x)\n    (rank ?y))\n\n";
    std::map<std::set<rbg_parser::token>,uint> legal_pieces_checkers_to_write;
    uint legal_pieces_checker_index = 0;
    std::vector<std::pair<const rbg_parser::pure_game_move*,uint>> moves_to_write;
    uint move_predicate_index = 0;
    std::vector<std::pair<const rbg_parser::condition*,uint>> conditions_to_write;
    uint condition_predicate_index = 0;
    for(const auto& el: local_register)
        result += el.write_transitions(
            local_register,
            legal_pieces_checkers_to_write,legal_pieces_checker_index,
            moves_to_write,move_predicate_index,
            conditions_to_write,condition_predicate_index) + "\n";
    result += '\n';
    result += write_all_helpers(
        legal_pieces_checkers_to_write,legal_pieces_checker_index,
        moves_to_write,move_predicate_index,
        conditions_to_write,condition_predicate_index);
    result += '\n';
    return result;
}

uint automaton::get_start_state(void){
    return local_register[start_state].get_id();
}
