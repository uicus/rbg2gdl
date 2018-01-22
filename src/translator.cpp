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
    for(uint i=0;i<pg.get_board().get_height();++i)
        result += "(rank "+std::to_string(i)+")\n";
    result += '\n';
    for(uint i=0;i<pg.get_board().get_width();++i)
        result += "(file "+std::to_string(i)+")\n";
    result += '\n';
}

void translator::init_to_gdl(void){
    result += section_title("Init");
    first_player_to_gdl();
    board_to_gdl();
    result += "(init ("+current_state+" "+std::to_string(moves_automaton.get_start_state())+"))\n";
    result += "(init ("+current_cell+" 0 0))\n";
    result += "(init ("+turn_name+" 0))\n";
    result += "(<= (init ("+variable_count+" ?variable 0))\n    "+variable_type("?variable")+")\n";
    result += '\n';
}

void translator::arithmetics_to_gdl(void){
    result += section_title("Board arithmetics");
    uint board_max = std::max(pg.get_board().get_height(),pg.get_board().get_width());
    result += succ(board_succ,board_max);
    result += arithmetics(board_succ,board_arithmetics);
    result += pairwise_succ(pairwise_board_succ,pg.get_board().get_height(),pg.get_board().get_width());
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
    result += modifier_because_turn_change;
    result += modifier_because_off;
    result += modifier_because_assignment;
    result += "\n\n";
    result += moves_automaton.effects_to_gdl();
}

void translator::legal_to_gdl(void){
    result += section_title("Legal");
    result += legal(pg.get_straightness()+1);
}

void translator::next_state(void){
    result += section_title("Next state");
    result += affected(pg.get_straightness()+1);
    result += next_control(pg.get_straightness()+1);
    result += next_turn(pg.get_straightness()+1,variables_succ);
    result += next_state_number(pg.get_straightness()+1);
    result += next_non_affected_cells + next_non_affected_vars + '\n';
    result += next_cell(pg.get_straightness()+1);
    result += next_variable(pg.get_straightness()+1);
}

void translator::variables_logic(void){
    result += section_title("Variables logic");
    result += capture_count(pairwise_board_succ,variables_succ,pg.get_board().get_height(),pg.get_board().get_width());
    result += succ(variables_succ,200); // TODO: clearly just a placeholder
    result += arithmetics(variables_succ,variables_arithmetics);
    result += comaprisons(variables_arithmetics,variables_succ);
    result += "(<= "+variable_value("turn","?val")+"\n    (true ("+turn_name+" ?val)))\n";
    result += "(<= "+variable_value("?piece","?val")+"\n    "+piece_type("?piece")+"\n    ("+capture_count_name+" ?piece ?val))\n";
    result += "(<= "+variable_value("?variable","?val")+"\n    "+variable_type("?variable")+"\n    (true ("+variable_count+" ?piece ?val)))\n";
    result += '\n';
}

void translator::identifiers_recognition(void){
    result += section_title("Pieces");
    for(const auto& el: pg.get_declarations().get_legal_pieces())
        result += piece_type(el.to_string()) + '\n';
    result += '\n';
    result += section_title("Variables");
    for(const auto& el: pg.get_declarations().get_legal_variables())
        result += variable_type(el.to_string()) + '\n';
    result += "(<= "+variable_type("?player")+"\n    (role ?player))\n";
    result += '\n';
}

void translator::end_game_logic(void){
    result += section_title("End game");
    result += terminal_because_no_legal(pg.get_straightness()+1);
    result += terminal_because_accept_state;
    result += goal_if_end_game;
    result += goal_if_ongoing;
}

std::string translator::to_gdl(void){
    build_automaton();
    result += section_title(pg.get_name());
    roles_to_gdl();
    identifiers_recognition();
    init_to_gdl();
    arithmetics_to_gdl();
    variables_logic();
    automaton_to_gdl();
    modifiers_to_gdl();
    legal_to_gdl();
    next_state();
    end_game_logic();
    return result;
}
