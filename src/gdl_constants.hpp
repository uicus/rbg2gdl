#ifndef GDL_CONSTANTS
#define GDL_CONSTANTS

#include<string>

#include"types.hpp"

const std::string separator = ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n";
const std::string digit = "(digit 0)\n(digit 1)\n";
const std::string board_succ = "boardSucc";
const std::string board_arithmetics = "board";
const std::string repetitions_succ = "repSucc";
const std::string variables_succ = "variablesSucc";
const std::string variables_arithmetics = "variables";
const std::string legal_pieces_checker = "legalPiece";
const std::string moves_helper_name = "moveHelper";
const std::string counter_move_helper_name = "moveRepeatingHelper";
const std::string conditions_helper_name = "conditionHelper";
const std::string variables_predicate = "var";
const std::string transition = "transition";
const std::string next_player = "nextPlayer";
const std::string continue_move = "continue_move";
const std::string epsilon = "epsilon";
const std::string current_state = "currentState";
const std::string current_cell = "currentCell";
const std::string cell_change = "off";
const std::string var_assignment = "assignment";

std::string section_title(const std::string& name);
uint length_of(uint v);
std::string succ(const std::string& name, uint max_number, bool logarithmic=false);
std::string sum(const std::string& succ_name, const std::string& sum_name);
std::string sub(const std::string& sum_name, const std::string& sub_name);
std::string eq(const std::string& sum_name, const std::string& eq_name);
std::string arithmetics(const std::string& succ_name, const std::string& arithmetics_name);
std::string comaprisons(const std::string arithmetics_name);
std::string sum_name(const std::string& arithmetics_name);
std::string sub_name(const std::string& arithmetics_name);
std::string eq_name(const std::string& arithmetics_name);
std::string neq_name(const std::string& arithmetics_name);
std::string ge_name(const std::string& arithmetics_name);
std::string greater_name(const std::string& arithmetics_name);
std::string le_name(const std::string& arithmetics_name);
std::string less_name(const std::string& arithmetics_name);
std::string any_number(const std::string& digits_name,uint max_number,bool logarithmic=false);
std::string unary_binary_equivalency(const std::string& name, int max_binary_number, int max_equivalent_number);
std::string control(const std::string& player_name);
std::string cell(uint x,uint y,const std::string& piece_name);
std::string cell(const std::string& x_name,const std::string& y_name,const std::string& piece_name);
std::string variable_value(const std::string& variable_name,const std::string& value);
std::string position_varaible(const std::string& position_name,uint index);

#endif
