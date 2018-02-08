#pragma once
#include <cassert>
#include <cctype>
#include "Lexer.hpp"

Token
Lexer::lex(symbol sym, std::string input)
{
    return scan(input);
}

Token 
Lexer::scan(std::string input)
{
    assert(!eof());
    while(!eof())
    {
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
                return lex_token(token_left_bracket);
            case '}':
                return lex_token(token_right_bracket);
            case '[':
                return lex_token(token_left_brace);
            case ']':
                return lex_token(token_right_brace);
            case '(':
                return lex_token(token_left_paren);
            case ')':
                return lex_token(token_right_paren);
            case ',':
                return lex_token(token_comma);
            case ';':
                return lex_token(token_semicolon);
            case ':':
                return lex_token(token_colon);

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
                return lex_token(token_conditional_op);
            case '=':
                if(peek()) != '=') return lex_token(token_assignment_op);
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
Lexer::lex_token(token_name token)
{
    accept();
    return { token }; // TODO: add location data

}

Token
Lexer::lex_relop(int len, relational_operators op)
{

}

Token 
Lexer::lex_arthop(arithmatic_operators op)
{

}

Token 
Lexer::lex_bitop(bitwise_operators op)
{

}

Token 
Lexer::lex_logop(int len, logical_operators op)
{

}

Token 
Lexer::lex_word(int len, char* c)
{
    assert(std::isalpha(c));

}

Token
Lexer::lex_number(char* c)
{
    assert(std::isdigit(c));

}

bool 
Lexer::is_comment_character(char ch)
{

}

void
Lexer::accept()
{
    char c = *current;
    comsumed += c;
    ++current;
}

void 
Lexer::ignore()
{
    char c = *current;
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
    ignore();
}

void 
Lexer::skip_comment()
{

}
