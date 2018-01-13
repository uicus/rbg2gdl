#ifndef AUTOMATON_BUILDER
#define AUTOMATON_BUILDER

#include"abstract_dispatcher.hpp"
#include"automaton.hpp"
#include"pure_game_move.hpp"
#include"game_move.hpp"

class automaton_builder : public rbg_parser::abstract_dispatcher{
        automaton final_result;
        void prepare_pure_move_automaton(const rbg_parser::pure_game_move* letter);
        void prepare_modifier_automaton(const rbg_parser::game_move* letter);
        std::vector<automaton> convert_elements_to_automatons(const std::vector<std::unique_ptr<rbg_parser::game_move>>& elements)const;
    public:
        void dispatch(const rbg_parser::sum& m)override;
        void dispatch(const rbg_parser::pure_sum& m)override;
        void dispatch(const rbg_parser::concatenation& m)override;
        void dispatch(const rbg_parser::pure_concatenation& m)override;
        void dispatch(const rbg_parser::bracketed_move& m)override;
        void dispatch(const rbg_parser::pure_bracketed_move& m)override;
        void dispatch(const rbg_parser::shift& m)override;
        void dispatch(const rbg_parser::ons& m)override;
        void dispatch(const rbg_parser::off& m)override;
        void dispatch(const rbg_parser::assignment& m)override;
        void dispatch(const rbg_parser::player_switch& m)override;
        void dispatch(const rbg_parser::condition_check& m)override;
        void dispatch(const rbg_parser::modifier_block& m)override;
        void dispatch(const rbg_parser::conjunction&)override{};
        void dispatch(const rbg_parser::alternative&)override{};
        void dispatch(const rbg_parser::negatable_condition&)override{};
        void dispatch(const rbg_parser::comparison&)override{};
        void dispatch(const rbg_parser::move_condition&)override{};
        automaton get_final_result(void);
};

#endif
