#include"pure_moves_printer.hpp"
#include"gdl_constants.hpp"
#include"shift.hpp"
#include"ons.hpp"
#include"pure_concatenation.hpp"
#include"pure_sum.hpp"
#include"pure_bracketed_move.hpp"
#include"condition_check.hpp"

pure_moves_printer::pure_moves_printer(
    const std::string& x_name, const std::string& y_name,
    uint& x_name_index, uint& y_name_index,
    std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index,
    std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write, uint& move_predicate_index,
    std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write, uint& condition_predicate_index):
final_result(),
x_name(x_name),
y_name(y_name),
x_name_index(x_name_index),
y_name_index(y_name_index),
legal_pieces_checkers_to_write(legal_pieces_checkers_to_write),
legal_pieces_checker_index(legal_pieces_checker_index),
moves_to_write(moves_to_write),
move_predicate_index(move_predicate_index),
conditions_to_write(conditions_to_write),
condition_predicate_index(condition_predicate_index){
}

std::string pure_moves_printer::get_final_result(void){
    return std::move(final_result);
}

void pure_moves_printer::displacement(const std::string& coord_name, uint& coord_index, int displacement){
    if(displacement>0){
        final_result +=
             "    ("+sum_name(board_arithmetics)
            +" ?"+coord_name+"_"+std::to_string(coord_index)
            +" "+std::to_string(displacement)
            +" ?"+coord_name+"_"+std::to_string(coord_index+1)
            +")\n";
        ++coord_index;
    }
    else if(displacement<0){
        final_result +=
             "    ("+sub_name(board_arithmetics)
            +" ?"+coord_name+"_"+std::to_string(coord_index)
            +" "+std::to_string(-displacement)
            +" ?"+coord_name+"_"+std::to_string(coord_index+1)
            +")\n";
        ++coord_index;
    }
}

uint pure_moves_printer::get_checker_number(const std::set<rbg_parser::token>& current_set){
    auto it = legal_pieces_checkers_to_write.find(current_set);
    if(it == legal_pieces_checkers_to_write.end())
        return legal_pieces_checkers_to_write[current_set] = legal_pieces_checker_index++;
    else
        return it->second;
}

pure_moves_printer pure_moves_printer::clone_printer(void){
    return pure_moves_printer(
        x_name,y_name,x_name_index,y_name_index,
        legal_pieces_checkers_to_write,legal_pieces_checker_index,
        moves_to_write,move_predicate_index,
        conditions_to_write,condition_predicate_index);
}

uint pure_moves_printer::get_pure_move_helper_index(const rbg_parser::pure_game_move* pgm){
    moves_to_write.push_back(std::make_pair(pgm,move_predicate_index++));
    return moves_to_write.back().second;
}

void pure_moves_printer::postpone_pure_move(const rbg_parser::pure_game_move* pgm){
    final_result += "("+moves_helper_name+"_"+std::to_string(get_pure_move_helper_index(pgm));
    final_result += " ?"+x_name+"_"+std::to_string(x_name_index)+" ?"+y_name+"_"+std::to_string(y_name_index);
    final_result += " ?"+x_name+"_"+std::to_string(x_name_index+1)+" ?"+y_name+"_"+std::to_string(y_name_index+1);
    final_result += ")";
    ++x_name_index;
    ++y_name_index;
}

void pure_moves_printer::dispatch(const rbg_parser::shift& m){
    displacement(x_name,x_name_index,m.get_x());
    displacement(y_name,y_name_index,m.get_y());
}

void pure_moves_printer::dispatch(const rbg_parser::ons& m){
    if(m.get_legal_ons().size()==0)
        final_result += "("+sum_name(board_arithmetics)+" 0 0 1)"; // TODO: Do it in a more elegant way
    else if(m.get_legal_ons().size()==1)
        final_result += "(true "+cell(x_name+std::to_string(x_name_index),y_name+std::to_string(x_name_index),m.get_legal_ons().begin()->to_string())+")";
    else{
        final_result += "(true "+cell(x_name+std::to_string(x_name_index),y_name+std::to_string(x_name_index),"?piece")+")\n";
        final_result += "    ("+legal_pieces_checker+"_"+std::to_string(get_checker_number(m.get_legal_ons()))+" ?piece)";
    }
}

void pure_moves_printer::dispatch(const rbg_parser::pure_sum& m){
    if(m.get_content().size()==0)
        final_result += "("+sum_name(board_arithmetics)+" 0 0 1)"; // TODO: Do it in a more elegant way
    else if(m.get_content().size()==1){
        auto pmp = clone_printer();
        m.get_content()[0]->accept(pmp);
        final_result += pmp.get_final_result();
    }
    else
        postpone_pure_move(&m);
}

void pure_moves_printer::dispatch(const rbg_parser::pure_concatenation& m){
    if(m.get_content().size()>0){
        auto pmp = clone_printer();
        m.get_content()[0]->accept(pmp);
        final_result += pmp.get_final_result();
    }
    for(uint i=1;i<m.get_content().size();++i){
        auto pmp = clone_printer();
        m.get_content()[0]->accept(pmp);
        final_result += "\n    "+pmp.get_final_result();
    }
}

void pure_moves_printer::dispatch(const rbg_parser::pure_bracketed_move& m){
    if(m.is_star() || m.get_number_of_repetitions() > 1)
        postpone_pure_move(&m);
    else{
        auto pmp = clone_printer();
        m.get_content()->accept(pmp);
        final_result += pmp.get_final_result();
    }
}

void pure_moves_printer::dispatch(const rbg_parser::condition_check& m){
    auto pmp = clone_printer();
    m.get_content()->accept(pmp);
    final_result += pmp.get_final_result();
}

void pure_moves_printer::dispatch(const rbg_parser::conjunction& m){
    // TODO: impl
}

void pure_moves_printer::dispatch(const rbg_parser::alternative& m){
    // TODO: impl
}

void pure_moves_printer::dispatch(const rbg_parser::negatable_condition& m){
    // TODO: impl
}

void pure_moves_printer::dispatch(const rbg_parser::comparison& m){
    // TODO: impl
}

void pure_moves_printer::dispatch(const rbg_parser::move_condition& m){
    // TODO: impl
}
