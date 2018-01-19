#ifndef MODIFIERS_PRINTER
#define MODIFIERS_PRINTER

#include"abstract_dispatcher.hpp"

#include<cassert>
#include<string>

class modifiers_printer : public rbg_parser::abstract_dispatcher{
        std::string final_result;
        uint state_id;
    public:
        modifiers_printer(uint state_id);
        void dispatch(const rbg_parser::sum&)override{assert(false);};
        void dispatch(const rbg_parser::pure_sum&)override{assert(false);};
        void dispatch(const rbg_parser::concatenation&)override{assert(false);};
        void dispatch(const rbg_parser::pure_concatenation&)override{assert(false);};
        void dispatch(const rbg_parser::bracketed_move&)override{assert(false);};
        void dispatch(const rbg_parser::pure_bracketed_move&)override{assert(false);};
        void dispatch(const rbg_parser::shift&)override{assert(false);};
        void dispatch(const rbg_parser::ons&)override{assert(false);};
        void dispatch(const rbg_parser::off& m)override;
        void dispatch(const rbg_parser::assignment& m)override;
        void dispatch(const rbg_parser::player_switch& m)override;
        void dispatch(const rbg_parser::condition_check&)override{assert(false);};
        void dispatch(const rbg_parser::modifier_block& m)override;
        void dispatch(const rbg_parser::conjunction&)override{assert(false);};
        void dispatch(const rbg_parser::alternative&)override{assert(false);};
        void dispatch(const rbg_parser::negatable_condition&)override{assert(false);};
        void dispatch(const rbg_parser::comparison&)override{assert(false);};
        void dispatch(const rbg_parser::move_condition&)override{assert(false);};
        std::string get_final_result(void);
};

#endif
