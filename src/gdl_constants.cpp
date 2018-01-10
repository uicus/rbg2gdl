#include"gdl_constants.hpp"


std::string section_title(const std::string& name){
    return separator+";; "+name+'\n'+separator+'\n';
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
        for(uint i=1;i<=max_number;++i)
            result += "("+name+' '+std::to_string(i)+' '+std::to_string(i+1)+")\n";
    return result;
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
