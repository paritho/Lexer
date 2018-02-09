#pragma once
#include <ctype>
#include <iostream>
#include <string>
#include "Token_enums.hpp"
#include "Location.hpp"

struct Token 
{
    // one constructor for each broad type of token
    Token() = default;
    Token(token_name name, Location loc);
    Token(const char* str, Location loc);
    Token(char c, Location loc);
    Token(relational_operators op, Location loc);
    Token(arithmatic_operators aop, Location loc);
    Token(bitwise_operators bop, Location loc);
    Token(logical_operators lop, Location loc);
    Token(keywords key, Location loc);
    Token(type_specifier ts, Location loc);
    Token(double num, Location loc);
    Token(integer_token it, radix rad, long long value, Location loc);
    
    ~Token() = default;

    // some comvienence. converts to true if not eof
    explicit operator bool() const { return name != tok_eof; }

    const char* display(token_name name);
    const char* display(symbol symbol);
    const char* display(keywords keyword);
    const char* display(relational_operators op);
    const char* display(arithmatic_operators op);
    const char* display(bitwise_operators op);
    const char* display(logical_operators op);
    const char* display(type_specifier type);
    const char* display(integer_token num);
    const char* display(char* str);
    const char* display(double num);
    const char* display(bool boolval);


    void set_file_path(std::string file) {return location.set_path(file);}
    void init_location() {return location.init();}
    bool has_attribute();
    
    token_name get_int_kind(radix rad);
    token_name get_name() { return name; }

    template<typename T>
    T get_attr(){
        assert(this.has_attribute());

        switch(name){
            case token_hex_int:
            case token_binary_int:
            case token_decimal_int:
                return attr.intval;
            case token_floating_point_literal:
                return attr.floatval;
            case token_boolean_literal:
                return attr.boolval;
            case token_character_literal:
                return attr.ch;
            case token_string_literal:
                return attr.str;
            case token_identifier:
                return attr.sym;
            case relational_operators:
                return attr.relop;
            case arithmatic_operators:
                return attr.arthop;
            case bitwise_operators:
                return attr.bitop;
            case logical_operators:
                return attr.logop;
            case type_specifier:
                return attr.typespec;
        }
     }


    private:
        token_name name;
        attribute attr;
        location location;
};

struct integer_token {
    radix radix;
    long long value;
}


union attribute
{
    attribute() = default;
    attribute(symbol s): sym(s){}
    attribute(keywords key) : keyword(key){}
    attribute(relational_operators op) : relop(op) {}
    attribute(arithmatic_operators op) : arthop(op) {}
    attribute(bitwise_operators op) : bitop(op) {}
    attribute(logical_operators op) :logop(op) {}
    attribute(type_specifier ts) : typespec(ts) {}
    attribute(integer_token inttok) : intval(inttok) {}
    attribute(double fp) : floatval(fp) {}
    attribute(char* str) : str(str){}
    attribute(char c) : ch(c) {}
    attribute(bool truefalse) : boolval(truefalse) {}
 
    // member vars for the token types
    symbol sym;
    keyword key;
    relational_operators relop;
    arithmatic_operators arthop;
    bitwise_operators bitop;
    logical_operators logop;
    type_specifier typespec;
    integer_token intval;
    bool boolval;
    char* str;
    char ch;
    double floatval;

};
