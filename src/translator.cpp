#include"translator.hpp"
#include"gdl_constants.hpp"
#include"automaton_builder.hpp"

translator::translator(rbg_parser::parsed_game&& pg):
pg(std::move(pg)),
result(){
}

void translator::roles_to_gdl(void){
    result += section_title("Roles");
    for(const auto& el: pg.get_declarations().get_legal_players())
        result += "(role "+el.to_string()+")\n";
    result += '\n';
}

void translator::first_player_to_gdl(void){
    result += "(init "+control(pg.get_declarations().get_first_player().to_string())+")\n\n";
}

void translator::board_to_gdl(void){
    for(uint i=0;i<pg.get_board().get_height();++i)
        for(uint j=0;j<pg.get_board().get_width();++j)
            result += "(init "+cell(j,i,pg.get_board().get_square(j,i).to_string())+")\n";
    result += '\n';
}

void translator::init_to_gdl(void){
    result += section_title("Init");
    first_player_to_gdl();
    board_to_gdl();
    result += "(init ("+current_state+" "+std::to_string(moves_automaton.get_start_state())+"))\n";
    result += "(init ("+current_cell+" 0 0))\n";
    result += '\n';
}

void translator::arithmetics_to_gdl(void){
    result += section_title("Arithmetics");
    uint board_max = std::max(pg.get_board().get_height(),pg.get_board().get_width());
    result += succ(board_succ,board_max);
    result += arithmetics(board_succ,board_arithmetics);
}

void translator::build_automaton(void){
    automaton_builder b;
    pg.get_moves()->accept(b);
    moves_automaton = b.get_final_result();
}

void translator::automaton_to_gdl(void){
    result += section_title("Transitions");
    result += moves_automaton.transitions_to_gdl();
}

void translator::modifiers_to_gdl(void){
    result += section_title("Modifiers");
    result += moves_automaton.effects_to_gdl();
}

std::string translator::to_gdl(void){
    build_automaton();
    result += section_title(pg.get_name());
    roles_to_gdl();
    init_to_gdl();
    arithmetics_to_gdl();
    automaton_to_gdl();
    modifiers_to_gdl();
    return result;
}
