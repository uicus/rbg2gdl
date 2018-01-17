#ifndef PURE_MOVES_PRINTER
#define PURE_MOVES_PRINTER

#include"abstract_dispatcher.hpp"
#include"token.hpp"
#include"pure_game_move.hpp"
#include"condition.hpp"

#include<string>
#include<set>
#include<map>
#include<cassert>

class pure_moves_printer : public rbg_parser::abstract_dispatcher{
        std::string final_result;
        const std::string& x_name;
        const std::string& y_name;
        uint& x_name_index;
        uint& y_name_index;
        std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write;
        uint& legal_pieces_checker_index;
        std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write;
        uint& move_predicate_index;
        std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write;
        uint& condition_predicate_index;
        bool standalone_hint;
        void displacement(const std::string& coord_name, uint& coord_index, int displacement);
        uint get_checker_number(const std::set<rbg_parser::token>& current_set);
        pure_moves_printer clone_printer(void);
        uint get_pure_move_helper_index(const rbg_parser::pure_game_move* pgm);
        uint get_condition_helper_index(const rbg_parser::condition* c);
        void postpone_pure_move(const rbg_parser::pure_game_move* pgm);
        void postpone_condition(const rbg_parser::condition* c);
        std::string side_of_comparison(const rbg_parser::token& var)const;
    public:
        pure_moves_printer(
            const std::string& x_name, const std::string& y_name,
            uint& x_name_index, uint& y_name_index,
            std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index,
            std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write, uint& move_predicate_index,
            std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write, uint& condition_predicate_index,
            bool standalone_hint = false);
        void dispatch(const rbg_parser::sum&)override{assert(false);};
        void dispatch(const rbg_parser::pure_sum& m)override;
        void dispatch(const rbg_parser::concatenation&)override{assert(false);};
        void dispatch(const rbg_parser::pure_concatenation& m)override;
        void dispatch(const rbg_parser::bracketed_move&)override{assert(false);};
        void dispatch(const rbg_parser::pure_bracketed_move& m)override;
        void dispatch(const rbg_parser::shift& m)override;
        void dispatch(const rbg_parser::ons& m)override;
        void dispatch(const rbg_parser::off&)override{assert(false);};
        void dispatch(const rbg_parser::assignment&)override{assert(false);};
        void dispatch(const rbg_parser::player_switch&)override{assert(false);};
        void dispatch(const rbg_parser::condition_check& m)override;
        void dispatch(const rbg_parser::modifier_block&)override{assert(false);};
        void dispatch(const rbg_parser::conjunction&)override;
        void dispatch(const rbg_parser::alternative&)override;
        void dispatch(const rbg_parser::negatable_condition&)override;
        void dispatch(const rbg_parser::comparison&)override;
        void dispatch(const rbg_parser::move_condition&)override;
        std::string get_final_result(void);
};

#endif
