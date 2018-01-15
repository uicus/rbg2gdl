#ifndef STANDALONE_MOVES_PRINTER
#define STNADALONE_MOVES_PRINTER

#include"abstract_dispatcher.hpp"
#include"token.hpp"
#include"types.hpp"
#include"pure_game_move.hpp"
#include"condition.hpp"

#include<string>
#include<cassert>
#include<vector>
#include<map>
#include<set>

class standalone_moves_printer : public rbg_parser::abstract_dispatcher{
        std::string final_result;
        uint current_index;
        std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write;
        uint& legal_pieces_checker_index;
        std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write;
        uint& move_predicate_index;
        std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write;
        uint& condition_predicate_index;
        std::string move_header(const std::string& x_name, const std::string& y_name,uint x_index,uint y_index)const;
        std::string move_footer(const std::string& x_name, const std::string& y_name,uint x_index,uint y_index)const;
    public:
        standalone_moves_printer(
            uint current_index,
            std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index,
            std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write, uint& move_predicate_index,
            std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write, uint& condition_predicate_index);
        void dispatch(const rbg_parser::sum&)override{assert(false);};
        void dispatch(const rbg_parser::pure_sum& m)override;
        void dispatch(const rbg_parser::concatenation&)override{assert(false);};
        void dispatch(const rbg_parser::pure_concatenation&)override{assert(false);};
        void dispatch(const rbg_parser::bracketed_move&)override{assert(false);};
        void dispatch(const rbg_parser::pure_bracketed_move& m)override;
        void dispatch(const rbg_parser::shift&)override{assert(false);};
        void dispatch(const rbg_parser::ons&)override{assert(false);};
        void dispatch(const rbg_parser::off&)override{assert(false);};
        void dispatch(const rbg_parser::assignment&)override{assert(false);};
        void dispatch(const rbg_parser::player_switch&)override{assert(false);};
        void dispatch(const rbg_parser::condition_check& m)override;
        void dispatch(const rbg_parser::modifier_block&)override{assert(false);};
        void dispatch(const rbg_parser::conjunction& m)override{assert(false);};
        void dispatch(const rbg_parser::alternative& m)override;
        void dispatch(const rbg_parser::negatable_condition& m)override{assert(false);};
        void dispatch(const rbg_parser::comparison&)override{assert(false);};
        void dispatch(const rbg_parser::move_condition&)override{assert(false);};
        std::string get_final_result(void);
};

#endif
