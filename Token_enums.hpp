#pragma once

enum token_name
{
    token_eof,

    // punctuators
    token_left_brace,
    token_right_brace,
    token_left_paren,
    token_right_paren,
    token_left_bracket,
    token_right_bracket,
    token_comma,
    token_semicolon,
    token_colon,

    // single operators
    token_conditional_op,
    token_assignment_op,

    // keywords
    key_def,
    key_else,
    key_if,
    key_let,
    key_var,

    // identifier
    token_identifier,

    // literals
    token_decimal_int_literal,
    token_binary_int_literal,
    token_hex_digit,
    token_hex_int_literal,
    token_binary_digit,
    token_floating_point_literal,
    token_boolean_literal,
    token_character_literal,
    token_string_literal,
    token_type_specifier
};

// operator groups
enum relational_operators 
{
    op_equals,
    op_notequal,
    op_lessthan,
    op_greaterthan,
    op_ltequals,
    op_gtequals,
};

enum arithmatic_operators
{
    op_plus,
    op_minus,
    op_mult,
    op_divide,
    op_mod,
};

enum bitwise_operators 
{
    op_and,
    op_or,
    op_not
};

enum logical_operators 
{
    op_AND,
    op_OR,
    op_NOT,
};

// helper enums
enum type_specifier
{
    ts_bool,
    ts_char,
    ts_int,
    ts_float,
    ts_void,
};

enum radix
{
    binary,
    decimal,
    hexidecimal
};
