#pragma once
#include <cassert>
#include <cctype>
#include "Lexer.hpp"

// construct the lexer
Lexer::Lexer(Symbol_table& symbol, const std::string& str)
    : symbols(symbol),
      current(str.data()), 
      end(str.size()),
      consumed("")
{ 
    // reserve language words
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

Token 
Lexer::scan()
{
    assert(!eof());
    while(!eof())
    {
        token_location = current_location;

        switch(*current)
        {
            // spaces and comments
            case ' ':
            case '\t':
                skip_space();
                continue;
            case '\n':
                skip_newline();
                continue;
            case '#':
                skip_comment();
                continue;

            // punctuators                        
            case '{':
                return lex_punctuator(token_left_bracket);
            case '}':
                return lex_punctuator(token_right_bracket);
            case '[':
                return lex_punctuator(token_left_brace);
            case ']':
                return lex_punctuator(token_right_brace);
            case '(':
                return lex_punctuator(token_left_paren);
            case ')':
                return lex_punctuator(token_right_paren);
            case ',':
                return lex_punctuator(token_comma);
            case ';':
                return lex_punctuator(token_semicolon);
            case ':':
                return lex_punctuator(token_colon);

            // relational ops
            case '<':
                if(peek() != '=') return lex_relop(1, op_lessthan);
                return lex_relop(2, op_ltequals);
            case '>':
                if(peek() != '=') return lex_relop(1, op_greaterthan);
                return lex_relop(2, op_gtequals);
            case '!':
                if(peek() == '=') return lex_relop(2, op_notequal);

            // conditional and assignment
            // NOTE: == is a relational op that will be determined here
            case '?':
                return lex_conditional();
            case '=':
                if(peek()) != '=') return lex_assignment();
                return lex_relop(2, op_equals);

            // arithmatic ops
            case '+':
                return lex_arthop(op_plus);
            case '-':
                return lex_arthop(op_minus);
            case '*':
                return lex_arthop(op_mult);
            case '/':
                return lex_arthop(op_divide);
            case '%':
                return lex_arthop(op_mod);

            // bitwise ops
            case '&':
                return lex_bitop(op_and);
            case '|':
                return lex_bitop(op_or);
            case '`':
                return lex_bitop(op_not);
        
            // logic ops handled in lex_word

            default:
                if(!std::isdigit(*current)) return lex_word();
                if(std::isdigit(*current)) return lex_number();
                
                // if we get here is an invalid char
                std::stringstream ss;
                ss << "invalid char" << *current;
                throw std::runtime_error(ss.str());

        }
    }

}

Token
Lexer::lex_punctuator(token_name token)
{
    accept();
    return Token(token, token_location);
}

Token
Lexer::lex_relop(int len, relational_operators op)
{
    while(len > 0)
    {
        accept();
        --len;
    }

    return Token(op, token_location);
}

Token 
Lexer::lex_arthop(arithmatic_operators op)
{
    accept();
    return Token(op, token_location);
}

Token 
Lexer::lex_bitop(bitwise_operators op)
{
    accept();
    return Token(op, token_location);
}

Token 
Lexer::lex_logop(int len, logical_operators op)
{
    accept();
    return Token(op, token_location);
}

Token 
Lexer::lex_word(int len, char* c)
{
    assert(std::isalpha(c));
    const char* start = current;
    // accept first char
    accept();
    // accept subsequent char if alphanumeric
    while(!eof() && is_alnum(*current)) accept();

    // build word
    std::string str(start, current);
    // add it to the symbol table
    Symbol_table sym = symbols.get(str);
    // look for reserved word
    auto iter = reserved_words.find(sym);
    if(iter != reserved_words.end()) return Token(iter->second, token_location);

    // return the token if not a reserved word
    return Token(str, token_location);


}

Token
Lexer::lex_number(char* c)
{
    assert(std::isdigit(c));

}

bool 
Lexer::is_comment_character()
{
    return *current != '\n';
}

void
Lexer::accept()
{
    current_location.next_column();
    char c = *current;
    comsumed += c;
    ++current;
}

void 
Lexer::ignore()
{
    current_location.next_column();
    ++current;
}

void 
Lexer::skip_space()
{
    while(!eof && std::isspace(*current)) ignore();
}

void 
Lexer::skip_newline()
{
    assert(*current == '\n');
    ignore();
    current_location.next_line();
    current_location.set_column(0);
}

void 
Lexer::skip_comment()
{
    assert(*current == '#');
    while(is_comment_character()) ignore();
}
