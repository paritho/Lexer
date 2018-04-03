#pragma once
#include "Token.hpp"
#include "Symbol.hpp"
#include "Location.hpp"

#include <string>
#include <unordered_map>
#include <cassert>
#include <cctype>
#include <stdexcept>


struct Lexer 
{
    Lexer(Symbol_table& symbol, const std::string& input);

    // lexical analysis helper functions
    Token scan();
    Token operator ()() { return scan(); }

    bool eof() { return current == end; }
    // take a peek at the NEXT char
    const char peek() { return eof() ? 0 : *(current + 1); }

    Token lex_punctuator(token_name token);
    Token lex_arrow_op();
    Token lex_relop(int len, relational_operators op);
    Token lex_arthop(arithmatic_operators op);
    Token lex_bitop(int len, bitwise_operators op);
    Token lex_logop(int len, logical_operators op);
    Token lex_conditional();
    Token lex_assignment();
    Token lex_word();
    Token lex_number();
    Token lex_binary_int();
    Token lex_hexidecimal_int();
    Token lex_character();
    Token lex_string();

    // other helpful functions
    char get_escape_sequence();
    bool is_comment_character(char ch);
    bool is_basic_character(char c);
    
    void accept();
    void accept(int n);
    void ignore();
    
    void skip_space();
    void skip_newline();
    void skip_comment();

    
    private:
        const char* current;
        const char* end;
        const char* consumed;
        Location current_location;
        Location token_location;
        Symbol_table& symbols;
        std::unordered_map<symbol, Token> reserved_words;

};

