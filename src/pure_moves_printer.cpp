#include"pure_moves_printer.hpp"
#include"gdl_constants.hpp"
#include"shift.hpp"
#include"ons.hpp"
#include"pure_concatenation.hpp"
#include"pure_sum.hpp"
#include"pure_bracketed_move.hpp"
#include"condition_check.hpp"
#include"conjunction.hpp"
#include"alternative.hpp"
#include"negatable_condition.hpp"
#include"move_condition.hpp"
#include"comparison.hpp"
#include<cassert>

pure_moves_printer::pure_moves_printer(
    const std::string& x_name, const std::string& y_name,
    uint& x_name_index, uint& y_name_index,
    std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index,
    std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write, uint& move_predicate_index,
    std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write, uint& condition_predicate_index,
    bool standalone_hint):
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
condition_predicate_index(condition_predicate_index),
standalone_hint(standalone_hint){
}

std::string pure_moves_printer::get_final_result(void){
    return std::move(final_result);
}

void pure_moves_printer::displacement(const std::string& coord_name, uint& coord_index, int displacement){
    if(displacement>0){
        final_result +=
             "("+sum_name(board_arithmetics)
            +" "+position_varaible(coord_name,coord_index)
            +" "+std::to_string(displacement)
            +" "+position_varaible(coord_name,coord_index+1)
            +")";
        ++coord_index;
    }
    else if(displacement<0){
        final_result +=
             "("+sub_name(board_arithmetics)
            +" "+position_varaible(coord_name,coord_index)
            +" "+std::to_string(-displacement)
            +" "+position_varaible(coord_name,coord_index+1)
            +")";
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

uint pure_moves_printer::get_condition_helper_index(const rbg_parser::condition* c){
    conditions_to_write.push_back(std::make_pair(c,condition_predicate_index++));
    return conditions_to_write.back().second;
}

void pure_moves_printer::postpone_pure_move(const rbg_parser::pure_game_move* pgm){
    final_result += "("+moves_helper_name+"_"+std::to_string(get_pure_move_helper_index(pgm));
    final_result += " "+position_varaible(x_name,x_name_index)+" "+position_varaible(y_name,y_name_index);
    final_result += " "+position_varaible(x_name,x_name_index+1)+" "+position_varaible(y_name,y_name_index+1);
    final_result += ")";
    ++x_name_index;
    ++y_name_index;
}

void pure_moves_printer::postpone_condition(const rbg_parser::condition* c){
    final_result += "("+moves_helper_name+"_"+std::to_string(get_condition_helper_index(c));
    final_result += " "+position_varaible(x_name,x_name_index)+" "+position_varaible(y_name,y_name_index);
    final_result += ")";
}

std::string pure_moves_printer::side_of_comparison(const rbg_parser::token& var, const std::string& alias){
    if(var.get_type() == rbg_parser::number)
        return var.to_string();
    else{
        final_result += variable_value(var.to_string(),"?"+alias)+"\n    ";
        return "?"+alias;
    }
}

void pure_moves_printer::dispatch(const rbg_parser::shift& m){
    displacement(x_name,x_name_index,m.get_x());
    if(m.get_x()!=0 && m.get_y()!=0)
        final_result += "\n    ";
    displacement(y_name,y_name_index,m.get_y());
}

void pure_moves_printer::dispatch(const rbg_parser::ons& m){
    if(m.get_legal_ons().size()==0)
        final_result += "("+sum_name(board_arithmetics)+" 0 0 1)"; // TODO: Do it in a more elegant way
    else if(m.get_legal_ons().size()==1)
        final_result += "(true "+cell(x_name+std::to_string(x_name_index),y_name+std::to_string(x_name_index),m.get_legal_ons().begin()->to_string())+")";
    else{
        final_result += "(true "+cell(position_varaible(x_name,x_name_index),position_varaible(y_name,y_name_index),"?piece")+")\n";
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
    if(standalone_hint)
        postpone_pure_move(&m);
    else{
        if(m.get_content().size()>0){
            auto pmp = clone_printer();
            m.get_content()[0]->accept(pmp);
            final_result += pmp.get_final_result();
        }
        for(uint i=1;i<m.get_content().size();++i){
            auto pmp = clone_printer();
            m.get_content()[i]->accept(pmp);
            final_result += "\n    "+pmp.get_final_result();
        }
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
    if(standalone_hint)
        postpone_condition(&m);
    else{
        if(m.get_content().size()>0){
            auto pmp = clone_printer();
            m.get_content()[0]->accept(pmp);
            final_result += pmp.get_final_result();
        }
        for(uint i=1;i<m.get_content().size();++i){
            auto pmp = clone_printer();
            m.get_content()[i]->accept(pmp);
            final_result += "\n    "+pmp.get_final_result();
        }
    }
}

void pure_moves_printer::dispatch(const rbg_parser::alternative& m){
    if(m.get_content().size()==0)
        final_result += "("+sum_name(board_arithmetics)+" 0 0 1)"; // TODO: Do it in a more elegant way
    else if(m.get_content().size()==1){
        auto pmp = clone_printer();
        m.get_content()[0]->accept(pmp);
        final_result += pmp.get_final_result();
    }
    else
        postpone_condition(&m);
}

void pure_moves_printer::dispatch(const rbg_parser::negatable_condition& m){
    auto pmp = clone_printer();
    m.get_content()->accept(pmp);
    if(m.is_negated()){
        pmp.standalone_hint = true;
        final_result += "(not "+pmp.get_final_result()+")";
    }
    else
        final_result += pmp.get_final_result();
}

void pure_moves_printer::dispatch(const rbg_parser::comparison& m){
    if(m.get_left_side().get_type() == rbg_parser::player){
        switch(m.get_kind_of_comparison().get_type()){
            case rbg_parser::double_equal:
                final_result += control(m.get_right_side().to_string());
                break;
            case rbg_parser::not_equal:
                final_result += "(not "+control(m.get_right_side().to_string())+")";
                break;
            default:
                assert(false);
        }
    }
    else{
        if(standalone_hint)
            postpone_condition(&m);
        else{
            std::string left_side = side_of_comparison(m.get_left_side(),"value1");
            std::string right_side = side_of_comparison(m.get_right_side(),"value2");
            final_result += "(";
            switch(m.get_kind_of_comparison().get_type()){
                case rbg_parser::double_equal:
                    final_result += eq_name(variables_arithmetics);
                    break;
                case rbg_parser::not_equal:
                    final_result += neq_name(variables_arithmetics);
                    break;
                case rbg_parser::greater_equal:
                    final_result += ge_name(variables_arithmetics);
                    break;
                case rbg_parser::greater:
                    final_result += greater_name(variables_arithmetics);
                    break;
                case rbg_parser::less_equal:
                    final_result += le_name(variables_arithmetics);
                    break;
                case rbg_parser::less:
                    final_result +=less_name(variables_arithmetics);
                    break;
                default:
                    assert(false);
            }
            final_result += " "+left_side+" "+right_side+")";
        }
    }
}

void pure_moves_printer::dispatch(const rbg_parser::move_condition& m){
    uint x_temp_index = 0, y_temp_index = 0;
    if(x_name_index==0)
        final_result += "("+eq_name(board_arithmetics)+" "+position_varaible(x_name,x_name_index)+" ?"+x_name+"_0)\n    ";
    if(y_name_index==0)
        final_result += "("+eq_name(board_arithmetics)+" "+position_varaible(y_name,y_name_index)+" ?"+y_name+"_0)\n    ";
    pure_moves_printer pmp(
        x_name+"_"+std::to_string(x_name_index),y_name+"_"+std::to_string(y_name_index),x_temp_index,y_temp_index,
        legal_pieces_checkers_to_write,legal_pieces_checker_index,
        moves_to_write,move_predicate_index,
        conditions_to_write,condition_predicate_index);
    m.get_content()->accept(pmp);
    final_result += pmp.get_final_result();
}
