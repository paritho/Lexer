#pragma once
#include <string>
#include <unordered_map>
#include "Token.hpp"
#include "Symbol.hpp"

struct Lexer 
{
    Lexer(Symbol_table& symbol, const std::string& str)
        : symbols(symbol),
          current(str.data()), 
          end(str.size()),
          consumed('')
    { 
        reserved_words.insert({
            // keywords
            { symbols.get('def'), key_def },
            { symbols.get('else'), key_else },
            { symbols.get('if'), key_if },
            { symbols.get('let'), key_let },
            { symbols.get('var'), key_var },
            // logic ops
            { symbols.get('and'), op_AND },
            { symbols.get('or'), op_OR },
            { symbols.get('not'), op_NOT },
            // boolean literal
            { symbols.get('true'), true },
            { symbols.get('false'), false },
            // type specifiers
            { symbols.get('bool'), ts_bool },
            { symbols.get('char'), ts_char },
            { symbols.get('int'), ts_int},
            { symbols.get('float'), ts_float },
            { symbols.get('void'), ts_void},
        });
    }

    // lexical analysis helper functions
    Token lex(symbol sym, std::string input);
    Token scan(std::string input);
    Token operator ()() {return scan();}

    bool eof(){return current == end;};
    char peek() const { return eof() ? 0 : *current + 1; }

    // this function works for all token types not in a group
    Token lex_token(token_name token);
    // these are for grouped types
    Token lex_relop(int len, relational_operators op);
    Token lex_arthop(arithmatic_operators op);
    Token lex_bitop(bitwise_operators op);
    Token lex_logop(int len, logical_operators op);
    Token lex_word(int len, char* ch);
    Token lex_number(char num);

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
        std::string consumed;
        symbol symbols;
        std::unordered_map<symbol, token> reserved_words;

};

