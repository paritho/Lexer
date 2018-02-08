#pragma once
#include <sstream>
#include <string>
#include <cassert>
#include "Token.hpp"

//TODO:
/* define all helper functions: attr.get(), to_string()
   switch(str)
        case relational_operators:
        return "relational-operator";
*/

// define attribute.get();

bool
Token::has_attribute()
{
        switch(name)
        {
                // list of tokens that need/have an attribute
                case token_decimal_int_literal:
                case token_binary_int_literal:
                case token_hex_digit:
                case token_hex_int_literal:
                case token_binary_digit:
                case token_floating_point_literal:
                case token_boolean_literal:
                case token_character_literal:
                case token_string_literal:
                case relational_operators:
                case arithmatic_operators:
                case bitwise_operators:
                case logical_operators:
                case type_specifier:
                        return true;
                default:
                        return false;
        }
}

//constructors
Token::Token(token_name name, Location loc)
        : name(name), location(loc);
{
        assert(!has_attribute);

}

Token::Token(const char* str)
        :
{
        assert(!has_attribute);
}

Token::Token(relational_operators op)
        :
{

}

Token::Token(arithmatic_operators aop)
        :
{

}

Token::Token(bitwise_operators bop)
        :
{

}

Token::Token(logical_operators lop)
        :
{

}

Token::Token(type_specifier ts)
        :
{

}

Token::Token(double num)
        :
{

}

Token::Token(float fp)
        :
{

}