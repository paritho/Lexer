#pragma once
#include <ctype>
#include <iostream>
#include <string>
#include "Token_enums.hpp"


struct Token 
{
    // one constructor for each broad type of token
    Token(token_name name) : name(name), attr() {}
    Token(relational_operators op) : name(op), attr(op) {}
    Token(arithmatic_operators aop) : name(aop), attr(aop) {}
    Token(bitwise_operators bop) : name(bop), attr(bop) {}
    Token(logical_operators lop) : name(lop), attr(lop) {}
    Token(type_specifier ts) : name(ts), attr(ts) {}
    Token(double num) : name(ts_int), attr(num) {}
    Token(float fp) : name(ts_float), attr(fp) {}
    
    ~Token() = default;

    // some comvienence. converts to true if not eof
    explicit operator bool() const { return name != tok_eof; }

    // TODO: provide to_string method to print tokens according to spec:
    // <token_name:token_attr>\n
    const std:string to_string(token_name name);
    const std:string to_string(relational_operators op);
    const std:string to_string(arithmatic_operators op);
    const std:string to_string(bitwise_operators op);
    const std:string to_string(logical_operators op);
    const std:string to_string(type_specifier ts);
    const std:string to_string(bool val);
    const std:string to_string(double num);
    const std:string to_string(float fp);

    const std:string to_string(attribute attr){ return to_string(attr); }

    private:
        token_name name;
        attribute attr;
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