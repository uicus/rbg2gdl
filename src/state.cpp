#include"state.hpp"
#include"gdl_constants.hpp"
#include"pure_moves_printer.hpp"
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

std::string state::write_if_turn_changer(void)const{
    if(action==nullptr)
        return "";
    if(!action->has_finisher())
        return "";
    const rbg_parser::token& next = action->finisher();
    if(next.get_type() == rbg_parser::dummy)
        return "("+next_player+" "+std::to_string(id)+" "+continue_move+")\n";
    else
        return "("+next_player+" "+std::to_string(id)+" "+next.to_string()+")\n";
}

std::string state::epsilon_transition(uint destination_id)const{
    return "(<= ("+transition+" "+std::to_string(id)+" ?x ?y "+std::to_string(destination_id)+" ?x ?y)\n"
         + "    (file ?x)\n    (rank ?y))\n";
}

std::string state::write_transitions(
    const std::vector<state>& local_register,
    std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index,
    std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write, uint& move_predicate_index,
    std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write, uint& condition_predicate_index)const{
    std::string result;
    result += epsilon_transition(id);
    for(const auto& el: next_states){
        uint destination_id = el.get_next_state(local_register).get_id();
        if(el.get_label()==nullptr)
            result += epsilon_transition(destination_id);
        else{
            uint x_index = 0, y_index = 0;
            result += "(<= ("+transition+" "+std::to_string(id)+" "+position_varaible("x",x_index)+" "+position_varaible("y",y_index)+" "+std::to_string(destination_id)+" ?x_last ?y_last)\n    ";
            pure_moves_printer pmp(
                "x","y",x_index,y_index,
                legal_pieces_checkers_to_write,legal_pieces_checker_index,
                moves_to_write,move_predicate_index,
                conditions_to_write,condition_predicate_index);
            el.get_label()->accept(pmp);
            result += pmp.get_final_result();
            result += "\n    ("+eq_name(board_arithmetics)+" "+position_varaible("x",x_index)+" ?x_last)\n";
            result += "\n    ("+eq_name(board_arithmetics)+" ?"+position_varaible("y",y_index)+" ?y_last))\n";
        }
    }
    return result;
}
