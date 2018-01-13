#include"pure_moves_printer.hpp"
#include"gdl_constants.hpp"
#include"shift.hpp"
#include"ons.hpp"

pure_moves_printer::pure_moves_printer(
    uint& current_predicate_id,
    const std::string& x_name, const std::string& y_name,
    uint& x_name_index, uint& y_name_index,
    std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index):
final_result(),
current_predicate_id(current_predicate_id),
x_name(x_name),
y_name(y_name),
x_name_index(x_name_index),
y_name_index(y_name_index),
legal_pieces_checkers_to_write(legal_pieces_checkers_to_write),
legal_pieces_checker_index(legal_pieces_checker_index){
}

std::string pure_moves_printer::get_final_result(void){
    return std::move(final_result);
}

std::string pure_moves_printer::displacement(const std::string& coord_name, uint& coord_index, int displacement){
    if(displacement>0){
        ++coord_index;
        return
             "    ("+sum_name(board_arithmetics)
            +" ?"+coord_name+"_"+std::to_string(coord_index-1)
            +" "+std::to_string(displacement)
            +" ?"+coord_name+"_"+std::to_string(coord_index)
            +")\n";
    }
    else if(displacement<0){
        ++coord_index;
        return
             "    ("+sub_name(board_arithmetics)
            +" ?"+coord_name+"_"+std::to_string(coord_index-1)
            +" "+std::to_string(-displacement)
            +" ?"+coord_name+"_"+std::to_string(coord_index)
            +")\n";
    }
    else
        return "";
}

uint pure_moves_printer::get_checker_number(const std::set<rbg_parser::token>& current_set){
    auto it = legal_pieces_checkers_to_write.find(current_set);
    if(it == legal_pieces_checkers_to_write.end())
        return legal_pieces_checkers_to_write[current_set] = legal_pieces_checker_index++;
    else
        return it->second;
}

void pure_moves_printer::dispatch(const rbg_parser::shift& m){
    final_result += displacement(x_name,x_name_index,m.get_x());
    final_result += displacement(y_name,y_name_index,m.get_y());
}

void pure_moves_printer::dispatch(const rbg_parser::ons& m){
    if(m.get_legal_ons().size()==0)
        final_result += "    ("+sum_name(board_arithmetics)+" 0 0 1)\n"; // TODO: Do it in a more elegant way
    else if(m.get_legal_ons().size()==1)
        final_result += "    (true "+cell(x_name+std::to_string(x_name_index),y_name+std::to_string(x_name_index),m.get_legal_ons().begin()->to_string())+")\n";
    else{
        final_result += "    (true "+cell(x_name+std::to_string(x_name_index),y_name+std::to_string(x_name_index),"?piece")+")\n";
        final_result += "    ("+legal_pieces_checker+"_"+std::to_string(get_checker_number(m.get_legal_ons()))+" ?piece)\n";
    }
}
