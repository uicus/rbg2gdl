#include"modifiers_printer.hpp"
#include"gdl_constants.hpp"
#include"switch.hpp"
#include"offs.hpp"
#include"modifier_block.hpp"
#include"assignments.hpp"

modifiers_printer::modifiers_printer(uint state_id):
state_id(state_id){
}

std::string modifiers_printer::get_final_result(void){
    return std::move(final_result);
}

void modifiers_printer::dispatch(const rbg_parser::player_switch& m){
    if(m.changes_player())
        final_result += "("+next_player+" "+std::to_string(state_id)+" "+m.get_player().to_string()+")\n";
    else
        final_result += "("+next_player+" "+std::to_string(state_id)+" "+continue_move+")\n";
}

void modifiers_printer::dispatch(const rbg_parser::modifier_block& m){
    for(const auto& el: m.get_content()){
        modifiers_printer mp(state_id);
        el->accept(mp);
        final_result += mp.get_final_result();
    }
}

void modifiers_printer::dispatch(const rbg_parser::off& m){
    final_result += "("+cell_change+" "+std::to_string(state_id)+" "+m.get_piece().to_string()+")\n";
}

void modifiers_printer::dispatch(const rbg_parser::assignment& m){
    final_result += "("+var_assignment+" "+std::to_string(state_id)+" "+m.get_left_side().to_string()+" "+m.get_right_side().to_string()+")\n";
}
