#ifndef TRANSLATOR
#define TRANSLATOR

#include"parsed_game.hpp"
#include"token.hpp"
#include"types.hpp"
#include"automaton.hpp"

class translator{
        rbg_parser::parsed_game pg;
        std::string result;
        automaton moves_automaton;
        void roles_to_gdl(void);
        void first_player_to_gdl(void);
        void board_to_gdl(void);
        void init_to_gdl(void);
        void build_automaton(void);
    public:
        translator(rbg_parser::parsed_game&& pg);
        std::string to_gdl(void);
};

#endif
