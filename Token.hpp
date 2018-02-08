#pragma once
#include <ctype>
#include <iostream>
#include <string>
#include "Token_enums.hpp"


struct Token 
{
    // one constructor for each broad type of token
    Token(token_name name);
    Token(const char* str);
    Token(relational_operators op);
    Token(arithmatic_operators aop);
    Token(bitwise_operators bop);
    Token(logical_operators lop);
    Token(type_specifier ts);
    Token(double num);
    Token(float fp);
    
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

struct location
{
    location()
        : line(0), column(0)
    {}

    void set_path(std::string path) { return file = f; }
    void init(){ return line=column=0; }
    void next_line() { return ++line; }
    void next_column(){ return ++column; }

    std::string file;
    int line;
    int column;
}