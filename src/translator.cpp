#include"translator.hpp"
#include"gdl_constants.hpp"

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
}

void translator::init_to_gdl(void){
    result += section_title("Init");
    first_player_to_gdl();
    board_to_gdl();
}

std::string translator::to_gdl(void){
    result += section_title(pg.get_name());
    roles_to_gdl();
    init_to_gdl();
    return result;
}
