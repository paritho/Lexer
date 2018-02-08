#pragma once
#include <ctype>
#include <iostream>
#include <string>
#include "Token_enums.hpp"
#include "Location.hpp"


struct Token 
{
    // one constructor for each broad type of token
    Token(token_name name, Location loc);
    Token(const char* str, Location loc);
    Token(relational_operators op, Location loc);
    Token(arithmatic_operators aop, Location loc);
    Token(bitwise_operators bop, Location loc);
    Token(logical_operators lop, Location loc);
    Token(type_specifier ts, Location loc);
    Token(double num, Location loc);
    Token(float fp, Location loc);
    
    ~Token() = default;

    // some comvienence. converts to true if not eof
    explicit operator bool() const { return name != tok_eof; }

    // TODO: provide to_string method to print tokens according to spec:
    // <token_name:token_attr>\n
    template<typename T>
    const std::string to_string(const T& val)
    {
        std::ostringstream ss;
        ss << '<' << val << ':' << attr << '>';
        return ss.str();
    }

    void set_file_path(std::string file) {return location.set_path(file);}
    void init_location() {return location.init();}
    bool has_attribute(){}

    private:
        token_name name;
        attribute attr;
        location location;
};

union attribute
{
    attribute() = default;
    attribute(symbol s): sym(s){}
    attribute(char* str) : str(str){}
    attribute(bool truefalse) : boolval(truefalse) {}
    attribute(relational_operators op) : relop(op) {}
    attribute(arithmatic_operators op) : arthop(op) {}
    attribute(bitwise_operators op) : bitop(op) {}
    attribute(logical_operators op) :logop(op) {}
    attribute(type_specifier ts) : typespec(ts) {}
    attribute(double num) : intval(n) {}
    attribute(float fp) : floatval(fp) {}
 
    // member vars for the token types
    symbol sym;
    relational_operators relop;
    arithmatic_operators arthop;
    bitwise_operators bitop;
    logical_operators logop;
    type_specifier typespec;
    bool boolval;
    char* str;
    double intval;
    float floatval;

};
