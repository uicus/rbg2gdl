#ifndef GDL_CONSTANTS
#define GDL_CONSTANTS

#include<string>

#include"types.hpp"

std::string section_title(const std::string& name);
std::string piece_type(const std::string& var);
std::string variable_type(const std::string& var);
uint length_of(uint v);
std::string succ(const std::string& name, uint max_number, bool logarithmic=false);
std::string pairwise_succ(const std::string& name, uint max_x,uint max_y);
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
std::string move_predicate(uint k_straightness);
std::string legal(uint k_straightness);
std::string affected(uint k_straightness);
std::string next_control(uint k_straightness);
std::string next_turn(uint k_straightness,const std::string& turn_succ_name);
std::string capture_count(const std::string& pair_succ_name, const std::string& succ_name, uint max_x, uint max_y);
std::string next_state_number(uint k_straightness);
std::string next_cell(uint k_straightness);
std::string next_variable(uint k_straightness);

const std::string separator = ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n";
const std::string digit = "(digit 0)\n(digit 1)\n";
const std::string board_succ = "boardSucc";
const std::string pairwise_board_succ = "cellSucc";
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
const std::string modifier_name = "modifier";
const std::string modifier_header = "(<= ("+modifier_name+" ?q)\n";
const std::string modifier_because_turn_change = modifier_header+"    ("+next_player+" ?q ?player))\n";
const std::string modifier_because_off = modifier_header+"    ("+cell_change+" ?q ?piece))\n";
const std::string modifier_because_assignment = modifier_header+"    ("+var_assignment+" ?q ?var ?val))\n";
const std::string noop = "noop";
const std::string accept = "acceptState";
const std::string finisher = "finisher";
const std::string move_name = "move";
const std::string affected_cell_name = "affectedCell";
const std::string affected_var_name = "affectedVar";
const std::string turn_name = "turnCount";
const std::string variable_count = "variableCount";
const std::string capture_count_helper_name = "captureCountHelper";
const std::string capture_count_name = "captureCount";
const std::string next_non_affected_cells = "(<= (next "+cell("?x","?y","?piece")+")\n    (not ("+affected_cell_name+" ?x ?y))\n    (true "+cell("?x","?y","?piece")+"))\n";
const std::string next_non_affected_vars = "(<= (next ("+variable_count+" ?variable ?val))\n    (not ("+affected_var_name+" ?variable))\n    (true ("+variable_count+" ?variable ?val)))\n";
const std::string terminal_because_no_legal = "(<= terminal\n    (true "+control("?player")+")\n    (not (legal ?player ?m)))\n";
const std::string terminal_because_accept_state = "(<= terminal\n    ("+accept+" ?q)\n    (true ("+current_state+" ?q)))\n";
const std::string goal_if_ongoing = "(<= (goal ?player 0)\n    (role ?player)\n    (not terminal))\n";
const std::string goal_if_end_game = "(<= (goal ?player ?score)\n    "+variable_value("?player","?score")+"\n    terminal)\n";

#endif
