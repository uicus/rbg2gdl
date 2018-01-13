#ifndef PURE_MOVES_PRINTER
#define PURE_MOVES_PRINTER

#include"abstract_dispatcher.hpp"
#include"token.hpp"

#include<string>
#include<set>
#include<map>

class pure_moves_printer : public rbg_parser::abstract_dispatcher{
        std::string final_result;
        uint& current_predicate_id;
        const std::string& x_name;
        const std::string& y_name;
        uint& x_name_index;
        uint& y_name_index;
        std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write;
        uint& legal_pieces_checker_index;
        std::string displacement(const std::string& coord_name, uint& coord_index, int displacement);
        uint get_checker_number(const std::set<rbg_parser::token>& current_set);
    public:
        pure_moves_printer(
            uint& current_predicate_id,
            const std::string& x_name, const std::string& y_name,
            uint& x_name_index, uint& y_name_index,
            std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index);
        void dispatch(const rbg_parser::sum&)override{};
        void dispatch(const rbg_parser::pure_sum& m)override;
        void dispatch(const rbg_parser::concatenation&)override{};
        void dispatch(const rbg_parser::pure_concatenation& m)override;
        void dispatch(const rbg_parser::bracketed_move&)override{};
        void dispatch(const rbg_parser::pure_bracketed_move& m)override;
        void dispatch(const rbg_parser::shift& m)override;
        void dispatch(const rbg_parser::ons& m)override;
        void dispatch(const rbg_parser::off&)override{};
        void dispatch(const rbg_parser::assignment&)override{};
        void dispatch(const rbg_parser::player_switch&)override{};
        void dispatch(const rbg_parser::condition_check& m)override;
        void dispatch(const rbg_parser::modifier_block&)override{};
        void dispatch(const rbg_parser::conjunction&)override;
        void dispatch(const rbg_parser::alternative&)override;
        void dispatch(const rbg_parser::negatable_condition&)override;
        void dispatch(const rbg_parser::comparison&)override;
        void dispatch(const rbg_parser::move_condition&)override;
        std::string get_final_result(void);
};

#endif
