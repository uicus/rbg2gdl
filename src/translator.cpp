#include"translator.hpp"

std::string to_gdl(const rbg_parser::parsed_game& pg){
    return pg.get_name();
}
