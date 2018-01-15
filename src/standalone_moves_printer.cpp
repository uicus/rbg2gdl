#include"standalone_moves_printer.hpp"
#include"pure_moves_printer.hpp"
#include"gdl_constants.hpp"
#include"pure_sum.hpp"
#include"pure_bracketed_move.hpp"

standalone_moves_printer::standalone_moves_printer(
    uint current_index,
    std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index,
    std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write, uint& move_predicate_index,
    std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write, uint& condition_predicate_index):
current_index(current_index),
legal_pieces_checkers_to_write(legal_pieces_checkers_to_write),
legal_pieces_checker_index(legal_pieces_checker_index),
moves_to_write(moves_to_write),
move_predicate_index(move_predicate_index),
conditions_to_write(conditions_to_write),
condition_predicate_index(condition_predicate_index){
}

std::string standalone_moves_printer::move_header(const std::string& x_name, const std::string& y_name,uint x_index,uint y_index)const{
    return "(<= ("+moves_helper_name+"_"+std::to_string(current_index)+" ?"+x_name+"_"+std::to_string(x_index)+" ?"+y_name+"_"+std::to_string(y_index)+" ?"+x_name+"_last ?"+y_name+"_last)\n";
}

std::string standalone_moves_printer::move_footer(const std::string& x_name, const std::string& y_name,uint x_index,uint y_index)const{
    return "    ("+eq_name(board_arithmetics)+" ?"+x_name+"_"+std::to_string(x_index)+" ?"+x_name+"_last)\n";
         + "    ("+eq_name(board_arithmetics)+" ?"+y_name+"_"+std::to_string(y_index)+" ?"+y_name+"_last))\n\n";
}

void standalone_moves_printer::dispatch(const rbg_parser::pure_sum& m){
    for(const auto& el: m.get_content()){
        uint x_index = 0,y_index = 0;
        final_result += move_header("x","y",x_index,y_index);
        pure_moves_printer pmp(
            "x","y",x_index,y_index,
            legal_pieces_checkers_to_write,legal_pieces_checker_index,
            moves_to_write,move_predicate_index,
            conditions_to_write,condition_predicate_index);
        el->accept(pmp);
        final_result += pmp.get_final_result();
        final_result += move_footer("x","y",x_index,y_index);
    }
}

void standalone_moves_printer::dispatch(const rbg_parser::pure_bracketed_move& m){
    // TODO: prolog safety!
    if(m.is_star()){
        final_result += "(<= ("+moves_helper_name+"_"+std::to_string(current_index)+" ?x ?y ?x ?y)\n";
        final_result += "    (file ?x)\n";
        final_result += "    (rank ?y))\n";
        uint x_index = 0, y_index = 0;
        final_result += move_header("x","y",x_index,y_index);
        pure_moves_printer pmp(
            "x","y",x_index,y_index,
            legal_pieces_checkers_to_write,legal_pieces_checker_index,
            moves_to_write,move_predicate_index,
            conditions_to_write,condition_predicate_index);
        m.get_content()->accept(pmp);
        final_result += pmp.get_final_result();
        final_result += "    ("+moves_helper_name+"_"+std::to_string(current_index)+" ?x_"+std::to_string(x_index)+" ?y_"+std::to_string(y_index)+" ?x_last ?y_last))\n\n";
    }
    else{
        assert(m.get_number_of_repetitions()>1);
        final_result += "(<= ("+moves_helper_name+"_"+std::to_string(current_index)+" ?x ?y ?x_last ?y_last)\n";
        final_result += "    ("+counter_move_helper_name+"_"+std::to_string(current_index)+" ?x ?y ?x_last ?y_last "+std::to_string(m.get_number_of_repetitions())+"))\n";
        final_result += "(<= ("+counter_move_helper_name+"_"+std::to_string(current_index)+" ?x ?y ?x ?y 0)\n";
        final_result += "    (file ?x)\n";
        final_result += "    (rank ?y))\n";
        uint x_index = 0, y_index = 0;
        final_result += "(<= ("+counter_move_helper_name+"_"+std::to_string(current_index)+" ?x_0 ?y_0 ?x_last ?y_last n)\n";
        pure_moves_printer pmp(
            "x","y",x_index,y_index,
            legal_pieces_checkers_to_write,legal_pieces_checker_index,
            moves_to_write,move_predicate_index,
            conditions_to_write,condition_predicate_index);
        m.get_content()->accept(pmp);
        final_result += pmp.get_final_result();
        final_result += "    ("+repetitions_succ+" ?prevn ?n)\n";
        final_result += "    ("+counter_move_helper_name+"_"+std::to_string(current_index)+" ?x_"+std::to_string(x_index)+" ?y_"+std::to_string(y_index)+" ?x_last ?y_last ?prevn))\n\n";
    }
}

void standalone_moves_printer::dispatch(const rbg_parser::condition_check& m){
    // TODO: impl
}

void standalone_moves_printer::dispatch(const rbg_parser::alternative& m){
    // TODO: impl
}
