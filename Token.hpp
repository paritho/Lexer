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

    long long get_value() { return value; }
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
    attribute(unary_operators op) :uop(op) {}
    attribute(type_specifier ts) : typespec(ts) {}
    attribute(integer_token inttok) : intval(inttok) {}
    attribute(double fp) : floatval(fp) {}
    attribute(const char* st) : str(st){}
    attribute(char c) : ch(c) {}
    attribute(bool truefalse) : boolval(truefalse) {}
 
    // member vars for the token types
    symbol sym;
    keywords key;
    relational_operators relop;
    arithmatic_operators arthop;
    bitwise_operators bitop;
    logical_operators logop;
    unary_operators uop;
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
    Token(token_name name, attribute at, Location loc={});
    Token(symbol symbol, Location loc={});
    Token(symbol str, token_name name, Location loc={});
    Token(keywords key, Location loc={});
    Token(relational_operators op, Location loc={});
    Token(arithmatic_operators aop, Location loc={});
    Token(bitwise_operators bop, Location loc={});
    Token(logical_operators lop, Location loc={});
    Token(unary_operators uop, Location loc={});
    Token(type_specifier ts, Location loc={});
    Token(radix rad, long long value, Location loc={});
    Token(char c, Location loc={});
    Token(double num, Location loc={});
    Token(bool boolval, Location loc={});
    
    // some comvienence. converts to true if not eof
    explicit operator bool() const { return name != token_eof; }

    std::string display(token_name name);
    std::string display(symbol sym);
    std::string display(keywords keyword);
    std::string display(relational_operators op);
    std::string display(arithmatic_operators op);
    std::string display(bitwise_operators op);
    std::string display(logical_operators op);
    std::string display(unary_operators op);
    std::string display(type_specifier type);
    std::string display(integer_token num);
    std::string display(char c);
    std::string display(double num);
    std::string display(long long num);
    std::string display(bool boolval);
    std::string display(Location loc);


    void set_file_path(std::string file) { return location.set_path(file); }
    bool has_attribute();
        
    token_name get_int_base(radix base);
    token_name get_name() { return name; }
    attribute get_attr(){return attr;}
    Location get_loc(){return location;}

    symbol get_sym_attr() const;
    const symbol get_str_attr() const;
    keywords get_key_attr() const;
    relational_operators get_relop_attr() const;
    arithmatic_operators get_arthop_attr() const;
    bitwise_operators get_bit_attr() const;
    logical_operators get_log_attr() const;
    unary_operators get_unop_attr() const;
    type_specifier get_ts_attr() const;
    integer_token get_intval_attr() const;
    bool get_bool_attr() const;
    char get_ch_attr() const;
    double get_float_attr() const;

    private:
        token_name name;
        attribute attr;
        Location location;
};

inline symbol
Token::get_sym_attr() const {return attr.sym;};

inline const symbol
Token::get_str_attr() const { return attr.sym; };

inline keywords 
Token::get_key_attr() const {return attr.key;};

inline relational_operators 
Token::get_relop_attr() const {return attr.relop;};

inline arithmatic_operators 
Token::get_arthop_attr() const {return attr.arthop;};

inline bitwise_operators 
Token::get_bit_attr() const {return attr.bitop;};

inline logical_operators 
Token::get_log_attr() const {return attr.logop;};

inline unary_operators 
Token::get_unop_attr() const {return attr.uop;};

inline type_specifier 
Token::get_ts_attr() const {return attr.typespec;};

inline integer_token 
Token::get_intval_attr() const {return attr.intval;};

inline bool 
Token::get_bool_attr() const {return attr.boolval;};

inline char 
Token::get_ch_attr() const {return attr.ch;};

inline double 
Token::get_float_attr() const {return attr.floatval;};

std::ostream& 
operator<<(std::ostream& os, Token t);