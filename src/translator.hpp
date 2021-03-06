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
        void arithmetics_to_gdl(void);
        void build_automaton(void);
        void automaton_to_gdl(void);
        void modifiers_to_gdl(void);
        void legal_to_gdl(void);
        void next_state(void);
        void variables_logic(void);
        void identifiers_recognition(void);
        void end_game_logic(void);
    public:
        translator(rbg_parser::parsed_game&& pg);
        std::string to_gdl(void);
};

#endif
