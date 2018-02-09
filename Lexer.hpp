#pragma once
#include <string>
#include <unordered_map>
#include "Token.hpp"
#include "Symbol.hpp"
#include "Location.hpp"

struct Lexer 
{
    Lexer(Symbol_table& symbol, std::string& input);

    // lexical analysis helper functions
    Token scan();
    Token operator ()() { return scan(); }

    bool eof(){return current == end;};
    // take a peek at the NEXT char
    char peek() const { return eof() ? 0 : *(current + 1); }

    Token lex_punctuator(token_name token);
    Token lex_relop(int len, relational_operators op);
    Token lex_arthop(arithmatic_operators op);
    Token lex_bitop(bitwise_operators op);
    Token lex_conditional();
    Token lex_assignment();
    Token lex_word(int len, char* ch);
    Token lex_number(char num);
    Token lex_binary_int();
    Token lex_hexidecimal_int();
    Token lex_character();
    Token lex_string();

    // other helpful functions
    bool is_comment_character(char ch);
    
    char accept();
    char ignore();
    
    void skip_space();
    void skip_newline();
    void skip_comment();

    
    private:
        const char* current;
        const char* end;
        const char* consumed;
        Location current_location;
        Location token_location;
        Symbol_table symbols;
        std::unordered_map<symbol, token> reserved_words;

};

