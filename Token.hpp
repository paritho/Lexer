#pragma once
#include "Token_enums.hpp"
#include "Location.hpp"
#include "Symbol.hpp"

#include <cctype>
#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <stdexcept>


struct integer_token {
    radix rad;
    long long value;
};

union attribute
{
    attribute() = default;
    attribute(symbol s): sym(s){}
    attribute(keywords key) : key(key){}
    attribute(relational_operators op) : relop(op) {}
    attribute(arithmatic_operators op) : arthop(op) {}
    attribute(bitwise_operators op) : bitop(op) {}
    attribute(logical_operators op) :logop(op) {}
    attribute(type_specifier ts) : typespec(ts) {}
    attribute(integer_token inttok) : intval(inttok) {}
    attribute(double fp) : floatval(fp) {}
    attribute(const char* str) : str(str){}
    attribute(char c) : ch(c) {}
    attribute(bool truefalse) : boolval(truefalse) {}
 
    // member vars for the token types
    symbol sym;
    keywords key;
    relational_operators relop;
    arithmatic_operators arthop;
    bitwise_operators bitop;
    logical_operators logop;
    type_specifier typespec;
    integer_token intval;
    bool boolval;
    const char* str;
    char ch;
    double floatval;

};

struct Token 
{
    // one constructor for each broad type of token
    Token();
    Token(token_name name, Location loc={});
    Token(symbol symbol, Location loc={});
    Token(keywords key, Location loc={});
    Token(relational_operators op, Location loc={});
    Token(arithmatic_operators aop, Location loc={});
    Token(bitwise_operators bop, Location loc={});
    Token(logical_operators lop, Location loc={});
    Token(type_specifier ts, Location loc={});
    Token(radix rad, long long value, Location loc={});
    Token(char c, Location loc={});
    Token(const char* str, Location loc={});
    Token(double num, Location loc={});
    Token(bool boolval, Location loc={});
    
    // some comvienence. converts to true if not eof
    explicit operator bool() const { return name != token_eof; }

    const char* display(token_name name);
    const char* display(symbol symbol);
    const char* display(keywords keyword);
    const char* display(relational_operators op);
    const char* display(arithmatic_operators op);
    const char* display(bitwise_operators op);
    const char* display(logical_operators op);
    const char* display(type_specifier type);
    const char* display(integer_token num);
    const char* display(char c);
    const char* display(char* str);
    const char* display(double num);
    const char* display(long long num);
    const char* display(bool boolval);
    const char* display(Location loc);


    void set_file_path(std::string file) { return location.set_path(file); }
    bool has_attribute();
    
    token_name get_int_base(radix base);
    token_name get_name() { return name; }

    symbol get_sym_attr(){return attr.sym;};
    keywords get_key_attr(){return attr.key;};
    relational_operators get_relop_attr(){return attr.relop;};
    arithmatic_operators get_arthop_attr(){return attr.arthop;};
    bitwise_operators get_bit_attr(){return attr.bitop;};
    logical_operators get_log_attr(){return attr.logop;};
    type_specifier get_ts_attr(){return attr.typespec;};
    integer_token get_intval_attr(){return attr.intval;};
    bool get_bool_attr(){ return attr.boolval;};
    const char* get_str_attr() {return attr.str;};
    char get_ch_attr() {return attr.ch;};
    double get_float_attr() {return attr.floatval;};

    private:
        token_name name;
        attribute attr;
        Location location;
};

std::ostream& 
operator<<(std::ostream& os, Token t);