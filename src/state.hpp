#ifndef STATE
#define STATE

#include<vector>

#include"game_move.hpp"
#include"pure_game_move.hpp"
#include"types.hpp"
#include"edge.hpp"

class state{
        const rbg_parser::game_move* action; // doesn't own
        uint id;
        std::vector<edge> next_states;
        static uint next_id;
    public:
        state(const rbg_parser::game_move* action=nullptr);
        state(const state& s);
        state& operator=(const state& s);
        state(state&& s)=default;
        state& operator=(state&& s)=default;
        ~state(void)=default;
        uint get_id(void)const;
        void inform_about_being_appended(uint shift_value);
        void connect_with_state(uint index_in_local_register, const rbg_parser::pure_game_move* label_condition=nullptr);
        bool modifier(void)const;
};

#endif
