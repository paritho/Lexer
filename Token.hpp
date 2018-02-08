#pragma once
#include <ctype>
#include <iostream>
#include <string>
#include "Token_enums.hpp"


struct Token 
{
    // one constructor for each broad type of token
    Token(token_name name) : name(name), attr(), location() {}
    Token(relational_operators op) : name(op), attr(op), location() {}
    Token(arithmatic_operators aop) : name(aop), attr(aop), location() {}
    Token(bitwise_operators bop) : name(bop), attr(bop), location() {}
    Token(logical_operators lop) : name(lop), attr(lop), location() {}
    Token(type_specifier ts) : name(ts), attr(ts), location() {}
    Token(double num) : name(ts_int), attr(num), location() {}
    Token(float fp) : name(ts_float), attr(fp), location() {}
    
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
   
    //const std:string to_string(attribute attr){ return to_string(attr); }

    void set_file_location(std::string file) {return location(file);}

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

struct location
{
    location() = default;
    location(std::string path)
        : file(file),
          line(0),
          column(0)
    {}

    std::string file;
    int line;
    int column;
}