#ifndef GDL_CONSTANTS
#define GDL_CONSTANTS

#include<string>

#include"types.hpp"

const std::string separator = ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n";
const std::string no_off = "no_off";
const std::string semi_turn = "semi_turn";
const std::string digit = "(digit 0)\n(digit 1)\n";

std::string section_title(const std::string& name);
uint length_of(uint v);
std::string succ(const std::string& name, uint max_number, bool logarithmic=false);
std::string any_number(const std::string& digits_name,uint max_number,bool logarithmic=false);
std::string unary_binary_equivalency(const std::string& name, int max_binary_number, int max_equivalent_number);
std::string control(const std::string& player_name);
std::string cell(uint x,uint y,const std::string& piece_name);
std::string cell(const std::string& x_name,const std::string& y_name,const std::string& piece_name);

#endif
