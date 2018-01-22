#include"gdl_constants.hpp"

std::string section_title(const std::string& name){
    return separator+";; "+name+'\n'+separator+'\n';
}

std::string piece_type(const std::string& var){
    return "(pieceType "+var+")";
}

std::string variable_type(const std::string& var){
    return "(variableType "+var+")";
}

uint length_of(uint v){
    uint i;
    for(i=0;v>>=1;++i);
    return i+1;
}

std::string succ(const std::string& name, uint max_number, bool logarithmic){
    std::string result;
    if(logarithmic){
        uint length = length_of(max_number);
        for(uint i=0;i<length;++i){
            if(i<length-1)
                result+="(<= ";
            result+="("+name;
            for(uint j=length-1;j>i;--j)
                result+=" ?x"+std::to_string(j);
            result+=" 0";
            for(uint j=i;j>0;--j)
                result+=" 1";
            for(uint j=length-1;j>i;--j)
                result+=" ?x"+std::to_string(j);
            result+=" 1";
            for(uint j=i;j>0;--j)
                result+=" 0";
            result+=")";
            for(uint j=length-1;j>i;--j)
                result+="\n\t(digit ?x"+std::to_string(j)+')';
            if(i<length-1)
                result+=')';
            result+='\n';
        }
    }
    else
        for(uint i=0;i<max_number-1;++i)
            result += "("+name+' '+std::to_string(i)+' '+std::to_string(i+1)+")\n";
    return result+'\n';
}

std::string pairwise_succ(const std::string& name, uint max_x,uint max_y){
    std::string result;
    for(uint i=0;i<max_x;++i){
        if(i>0)
            result += "("+name+" "+std::to_string(i-1)+" "+std::to_string(max_y-1)+" "+std::to_string(i)+" 0)\n";
        for(uint j=0;j<max_y-1;++j)
            result += "("+name+" "+std::to_string(i)+" "+std::to_string(j)+" "+std::to_string(i)+" "+std::to_string(j+1)+")\n";
    }
    result += '\n';
    return result;
}

std::string sum(const std::string& succ_name, const std::string& sum_name){
    std::string result;
    result += "(<= ("+sum_name+" ?x 0 ?x)\n    ("+succ_name+" ?x ?y))\n";
    result += "(<= ("+sum_name+" ?x 0 ?x)\n    ("+succ_name+" ?y ?x))\n";
    result += "(<= ("+sum_name+" ?x ?y ?z)\n";
    result += "    ("+succ_name+" ?x ?succx)\n";
    result += "    ("+succ_name+" ?prevy ?y)\n";
    result += "    ("+sum_name+" ?succx ?prevy ?z))\n";
    return result;
}

std::string sub(const std::string& sum_name, const std::string& sub_name){
    std::string result;
    result += "(<= ("+sub_name+" ?x ?y ?z)\n";
    result += "    ("+sum_name+" ?z ?y ?x))\n";
    return result;
}

std::string eq(const std::string& sum_name, const std::string& eq_name){
    std::string result;
    result += "(<= ("+eq_name+" ?x ?z)\n";
    result += "    ("+sum_name+" ?x 0 ?z))\n";
    return result;
}

std::string arithmetics(const std::string& succ_name, const std::string& arithmetics_name){
    std::string result;
    result += sum(succ_name,sum_name(arithmetics_name)) + '\n';
    result += sub(sum_name(arithmetics_name),sub_name(arithmetics_name)) + '\n';
    result += eq(sum_name(arithmetics_name),eq_name(arithmetics_name)) + '\n'; // TODO: check later if necessary
    return result;
}

std::string comaprisons(const std::string arithmetics_name, const std::string& succ_name){
    std::string result;
    result += "(<= ("+neq_name(arithmetics_name)+" ?x ?y)\n    (not ("+eq_name(arithmetics_name)+" ?x ?y))\n    ("+succ_name+" ?x ?nextx)\n    ("+succ_name+" ?y ?nexty))\n";
    result += "(<= ("+ge_name(arithmetics_name)+" ?x ?y)\n    ("+sum_name(arithmetics_name)+" ?z ?y ?x))\n";
    result += "(<= ("+greater_name(arithmetics_name)+" ?x ?y)\n    ("+ge_name(arithmetics_name)+" ?x ?y)\n    ("+neq_name(arithmetics_name)+" ?x ?y))\n";
    result += "(<= ("+le_name(arithmetics_name)+" ?x ?y)\n    ("+sum_name(arithmetics_name)+" ?z ?x ?y))\n";
    result += "(<= ("+less_name(arithmetics_name)+" ?x ?y)\n    ("+le_name(arithmetics_name)+" ?x ?y)\n    ("+neq_name(arithmetics_name)+" ?x ?y))\n";
    result += '\n';
    return result;
}

std::string sum_name(const std::string& arithmetics_name){
    return arithmetics_name+"Sum";
}

std::string sub_name(const std::string& arithmetics_name){
    return arithmetics_name+"Sub";
}

std::string eq_name(const std::string& arithmetics_name){
    return arithmetics_name+"Eq";
}

std::string neq_name(const std::string& arithmetics_name){
    return arithmetics_name+"NotEq";
}

std::string ge_name(const std::string& arithmetics_name){
    return arithmetics_name+"GrOrEq";
}

std::string greater_name(const std::string& arithmetics_name){
    return arithmetics_name+"Gr";
}

std::string le_name(const std::string& arithmetics_name){
    return arithmetics_name+"LeOrEq";
}

std::string less_name(const std::string& arithmetics_name){
    return arithmetics_name+"Le";
}

std::string any_number(const std::string& digits_name,uint max_number,bool logarithmic){
    std::string result = "";
    if(logarithmic){
        uint l = length_of(max_number);
        for(uint i=0;i<l;++i)
            result+=" ?"+digits_name+std::to_string(i);
    }
    else
        result+=" ?"+digits_name;
    return result;
}

std::string unary_binary_equivalency(const std::string& name, int max_binary_number, int max_equivalent_number){
    std::string result;
    uint l = length_of(max_binary_number);
    uint max_significant_length = length_of(max_equivalent_number);
    uint current_number = 0;
    while(!(current_number&(1<<(max_significant_length)))){
        result+="("+name+" "+std::to_string(current_number);
        for(uint i=l;i>0;--i)
            result+=(current_number&(1<<(i-1))?" 1":" 0");
        result+=")\n";
        ++current_number;
    }
    for(++max_significant_length;max_significant_length<=l;++max_significant_length){
        result+="("+name+" "+std::to_string(current_number);
        for(uint i=0;i<l-max_significant_length;++i)
            result+=" 0";
        result+=" 1";
        for(uint i=0;i+1<max_significant_length;++i)
            result+=" ?s"+std::to_string(i);
        result+=")\n";
    }
    return result;
}

std::string control(const std::string& player_name){
    return "(control "+player_name+")";
}

std::string cell(uint x,uint y,const std::string& piece_name){
    return cell(std::to_string(x),std::to_string(y),piece_name);
}

std::string cell(const std::string& x_name,const std::string& y_name,const std::string& piece_name){
    return "(cell "+x_name+" "+y_name+" "+piece_name+")";
}

std::string variable_value(const std::string& variable_name,const std::string& value){
    return "(var "+variable_name+" "+value+")"; // TODO: "("+variable_predicate+" "+variable_name+" "+value+")"; <-- check this strange segfault
}

std::string position_varaible(const std::string& position_name,uint index){
    return "?"+position_name+(index>0 ? "_"+std::to_string(index) : "");
}

std::string move_predicate(uint k_straightness){
    std::string result;
    result += "("+move_name;
    for(uint i=0;i<k_straightness;++i){
        std::string number = std::to_string(i+1);
        result += " ?q"+number+" ?x"+number+" ?y"+number;
    }
    result += ")";
    return result;
}

std::string legal(uint k_straightness){
    std::string result;
    result += "(<= (legal ?player "+noop+")\n    (role ?player)\n    (not (true "+control("?player")+")))\n";
    result += "(<= (legal ?player "+move_predicate(k_straightness)+")\n    ";
    result += "(true "+control("?player")+")\n    ";
    result += "(true ("+current_state+" ?q0))\n    ";
    result += "(true ("+current_cell+" ?x0 ?y0))\n    ";
    for(uint i=0;i<k_straightness;++i){
        std::string prev_number = std::to_string(i);
        std::string number = std::to_string(i+1);
        result += "("+transition+" ?q"+prev_number+" ?x"+prev_number+" ?y"+prev_number+" ?q"+number+" ?x"+number+" ?y"+number+")\n    ";
    }
    for(uint i=0;i<k_straightness-1;++i)
        result += "("+modifier_name+" ?q"+std::to_string(i+1)+")\n    ";
    result += "("+finisher+" ?q"+std::to_string(k_straightness)+"))\n";
    result += '\n';
    return result;
}

std::string affected(uint k_straightness){
    std::string result;
    for(uint i=0;i<k_straightness;++i){
        std::string number = std::to_string(i+1);
        result += "(<= ("+affected_cell_name+" ?x"+number+" ?y"+number+")\n    ";
        result += "(does ?player "+move_predicate(k_straightness)+")\n    ";
        result += "("+cell_change+" ?q"+number+" ?piece))\n";
    }
    result += '\n';
    for(uint i=0;i<k_straightness;++i){
        std::string number = std::to_string(i+1);
        result += "(<= ("+affected_var_name+" ?var)\n    ";
        result += "(does ?player "+move_predicate(k_straightness)+")\n    ";
        result += "("+var_assignment+" ?q"+number+" ?var ?val))\n";
    }
    result += '\n';
    return result;
}

std::string next_control(uint k_straightness){
    std::string result;
    result += "(<= (next "+control("?player")+")\n    ";
    result += "(does ?movingPlayer "+move_predicate(k_straightness)+")\n    ";
    result += "("+next_player+" ?q"+std::to_string(k_straightness)+" ?player))\n";
    result += "(<= (next "+control("?player")+")\n    ";
    result += "(does ?movingPlayer "+move_predicate(k_straightness)+")\n    ";
    result += "("+next_player+" ?q"+std::to_string(k_straightness)+" "+continue_move+")\n    ";
    result += "(true "+control("?player")+"))\n";
    result += "(<= (next "+control("?player")+")\n    ";
    result += "(does ?movingPlayer "+move_predicate(k_straightness)+")\n    ";
    result += "(role ?nextPlayer)\n    ";
    result += "(not ("+next_player+" ?q"+std::to_string(k_straightness)+" ?nextPlayer))\n    ";
    result += "(true "+control("?player")+"))\n";
    result += '\n';
    return result;
}

std::string next_turn(uint k_straightness, const std::string& turn_succ_name){
    std::string result;
    result += "(<= (next ("+turn_name+" ?next))\n    ";
    result += "(does ?movingPlayer "+move_predicate(k_straightness)+")\n    ";
    result += "("+next_player+" ?q"+std::to_string(k_straightness)+" ?player)\n    ";
    result += "(true ("+turn_name+" ?n))\n    ";
    result += "("+turn_succ_name+" ?n ?next))\n";
    result += "(<= (next ("+turn_name+" ?n))\n    ";
    result += "(does ?movingPlayer "+move_predicate(k_straightness)+")\n    ";
    result += "("+next_player+" ?q"+std::to_string(k_straightness)+" "+continue_move+")\n    ";
    result += "(true ("+turn_name+" ?n)))\n";
    result += "(<= (next ("+turn_name+" ?n))\n    ";
    result += "(does ?movingPlayer "+move_predicate(k_straightness)+")\n    ";
    result += "(not ("+next_player+" ?q"+std::to_string(k_straightness)+" ?nextPlayer))\n    ";
    result += "(role ?nextPlayer)\n    ";
    result += "(true ("+turn_name+" ?n)))\n";
    result += '\n';
    return result;
}

std::string capture_count(const std::string& pair_succ_name, const std::string& succ_name, uint max_x, uint max_y){
    std::string result;
    result += "(<= ("+capture_count_name+" ?piece ?val)\n    ";
    result += "("+capture_count_helper_name+" ?piece ?val "+std::to_string(max_x-1)+" "+std::to_string(max_y-1)+"))\n";
    result += "(<= ("+capture_count_helper_name+" ?piece 0 0 0)\n    ";
    result += "(true "+cell(0,0,"?anotherPiece")+")\n    ";
    result += piece_type("?piece")+"\n    ";
    result += "(distinct ?piece ?anotherPiece))\n";
    result += "(<= ("+capture_count_helper_name+" ?piece 1 0 0)\n    ";
    result += "(true "+cell(0,0,"?piece")+"))\n";
    result += "(<= ("+capture_count_helper_name+" ?piece ?n ?x ?y)\n    ";
    result += "(true "+cell("?x","?y","?anotherPiece")+")\n    ";
    result += "(distinct ?piece ?anotherPiece)\n    ";
    result += "("+pair_succ_name+" ?prevx ?prevy ?x ?y)\n    ";
    result += "("+capture_count_helper_name+" ?piece ?n ?prevx ?prevy))\n";
    result += "(<= ("+capture_count_helper_name+" ?piece ?n ?x ?y)\n    ";
    result += "(true "+cell("?x","?y","?piece")+")\n    ";
    result += "("+pair_succ_name+" ?prevx ?prevy ?x ?y)\n    ";
    result += "("+succ_name+" ?prevn ?n)\n    ";
    result += "("+capture_count_helper_name+" ?piece ?prevn ?prevx ?prevy))\n";
    result += '\n';
    return result;
}

std::string next_state_number(uint k_straightness){
    std::string result;
    result += "(<= (next ("+current_state+" ?q"+std::to_string(k_straightness)+"))\n    ";
    result += "(does ?movingPlayer "+move_predicate(k_straightness)+"))\n";
    result += '\n';
    return result;
}

std::string next_cell(uint k_straightness){
    std::string result;
    for(uint i=1;i<=k_straightness;++i){
        auto number = std::to_string(i);
        result += "(<= (next "+cell("?x"+number,"?y"+number,"?piece")+")\n    ";
        result += "(does ?player "+move_predicate(k_straightness)+")\n    ";
        result += "("+cell_change+" ?q"+number+" ?piece)";
        for(uint j=i+1;j<=k_straightness;++j){
            result += "\n    (not ("+cell_change+" ?q"+std::to_string(j)+" ?piece"+std::to_string(j)+"))";
            result += "\n    "+piece_type("?piece"+std::to_string(j));
        }
        result += ")\n";
    }
    result += '\n';
    return result;
}

std::string next_variable(uint k_straightness){
    std::string result;
    result += "(<= (newVal ?val ?val)\n    ("+variables_succ+" ?val ?next))\n";
    result += "(<= (newVal ?val ?numVal)\n    "+variable_value("?val","?numVal")+")\n\n";
    for(uint i=1;i<=k_straightness;++i){
        auto number = std::to_string(i);
        result += "(<= (next ("+variable_count+" ?variable ?numVal))\n    ";
        result += "(does ?player "+move_predicate(k_straightness)+")\n    ";
        result += "("+var_assignment+" ?q"+number+" ?variable ?val)\n    ";
        result += "(newVal ?val ?numVal)";
        for(uint j=i+1;j<=k_straightness;++j){
            result += "\n    (not ("+var_assignment+" ?q"+std::to_string(j)+" ?variable ?val"+std::to_string(j)+"))";
            result += "\n    (newVal ?val"+std::to_string(j)+" ?anotherVal)";
        }
        result += ")\n";
    }
    result += '\n';
    return result;
}

std::string terminal_because_no_legal(uint k_straightness){
    std::string result;
    result +="(<= terminal\n    (true "+control("?player")+")\n    (not (legal ?player "+move_predicate(k_straightness)+"))";
    for(uint i=0;i<k_straightness;++i){
        std::string number = std::to_string(i+1);
        result += "\n     (epsilon ?q"+number+" ?q"+number+")";
        result += "\n     (file ?x"+number+")";
        result += "\n     (rank ?y"+number+")";
    }
    result += ")\n";
    return result;
}
