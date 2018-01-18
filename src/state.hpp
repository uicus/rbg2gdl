#ifndef STATE
#define STATE

#include<vector>
#include<map>
#include<set>
#include<string>

#include"game_move.hpp"
#include"pure_game_move.hpp"
#include"condition.hpp"
#include"token.hpp"
#include"types.hpp"
#include"edge.hpp"

class state{
        const rbg_parser::game_move* action; // doesn't own
        uint id;
        std::vector<edge> next_states;
        static uint next_id;
        std::string epsilon_transition(uint destination_id)const;
    public:
        state(const rbg_parser::game_move* action=nullptr);
        state(const state& s);
        state& operator=(const state& s);
        state(state&& s)=default;
        state& operator=(state&& s)=default;
        ~state(void)=default;
        uint get_id(void)const;
        void inform_about_being_appended(uint shift_value);
        void inform_about_state_deletion(uint deleted_index);
        void connect_with_state(uint index_in_local_register, const rbg_parser::pure_game_move* label_condition=nullptr);
        bool modifier(void)const;
        void absorb(state&& rhs);
        std::string write_if_turn_changer(void)const;
        std::string write_transitions(
            const std::vector<state>& local_register,
            std::map<std::set<rbg_parser::token>,uint>& legal_pieces_checkers_to_write, uint& legal_pieces_checker_index,
            std::vector<std::pair<const rbg_parser::pure_game_move*,uint>>& moves_to_write, uint& move_predicate_index,
            std::vector<std::pair<const rbg_parser::condition*,uint>>& conditions_to_write, uint& condition_predicate_index)const;
};

#endif
