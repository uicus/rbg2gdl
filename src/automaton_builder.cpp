#include"automaton_builder.hpp"
#include"pure_sum.hpp"
#include"pure_concatenation.hpp"
#include"pure_bracketed_move.hpp"
#include"ons.hpp"
#include"shift.hpp"
#include"condition_check.hpp"
#include"offs.hpp"
#include"assignments.hpp"
#include"switch.hpp"
#include"modifier_block.hpp"
#include"sum.hpp"
#include"concatenation.hpp"
#include"bracketed_move.hpp"
#include<vector>

void automaton_builder::prepare_pure_move_automaton(const rbg_parser::pure_game_move* letter){
    final_result = pure_letter_automaton(letter);
}

void automaton_builder::prepare_modifier_automaton(const rbg_parser::game_move* letter){
    final_result = modifier_automaton(letter);
}

automaton automaton_builder::get_final_result(void){
    return std::move(final_result);
}

void automaton_builder::dispatch(const rbg_parser::pure_sum& m){
    prepare_pure_move_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::pure_concatenation& m){
    prepare_pure_move_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::pure_bracketed_move& m){
    prepare_pure_move_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::shift& m){
    prepare_pure_move_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::ons& m){
    prepare_pure_move_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::condition_check& m){
    prepare_pure_move_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::off& m){
    prepare_modifier_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::assignment& m){
    prepare_modifier_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::player_switch& m){
    prepare_modifier_automaton(&m);
}

void automaton_builder::dispatch(const rbg_parser::modifier_block& m){
    prepare_modifier_automaton(&m);
}

std::vector<automaton> automaton_builder::convert_elements_to_automatons(const std::vector<std::unique_ptr<rbg_parser::game_move>>& elements)const{
    std::vector<automaton> temporary_results;
    for(const auto& el: elements){
        automaton_builder element_builder;
        el->accept(element_builder);
        temporary_results.push_back(element_builder.get_final_result());
    }
    return temporary_results;
}

void automaton_builder::dispatch(const rbg_parser::sum& m){
    final_result = sum_of_automatons(convert_elements_to_automatons(m.get_content()));
}

void automaton_builder::dispatch(const rbg_parser::concatenation& m){
    final_result = concatenation_of_automatons(convert_elements_to_automatons(m.get_content()));
}

void automaton_builder::dispatch(const rbg_parser::bracketed_move& m){
    m.get_content()->accept(*this);
    if(m.is_star())
        final_result.starify_automaton();
    else if(m.get_number_of_repetitions() > 1)
        final_result.repeat_automaton(m.get_number_of_repetitions());
}
