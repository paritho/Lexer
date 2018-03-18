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

    // operator groups
    token_relational_op,
    token_arithmatic_op,
    token_bitwise_op,
    token_logical_op,
    token_type_specifier,
    token_keywords,

    // identifier
    token_identifier,

    // literals
    token_decimal_int,
    token_binary_int,
    token_hex_int,
    token_floating_point_literal,
    token_boolean_literal,
    token_character_literal,
    token_string_literal,
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
    op_xor,
    op_leftshift,
    op_rightshift,
    op_not,
};

enum logical_operators 
{
    logop_AND,
    logop_OR,
    logop_NOT,
};

enum keywords
{
    key_as,
    key_break,
    key_continue,
    key_def,
    key_else,
    key_if,
    key_let,
    key_return,
    key_var,
    key_while,

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
