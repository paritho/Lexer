#pragma once

#include "Lexer.hpp"
#include <deque>
#include <string>

struct Parser{
    
    Parser(Symbol_table& syms, const std::string& source);

    void parse_type();

    // epxressions
    void parse_expr();
    void parse_assign_expr();
    void parse_conditional_expr();
    void parse_log_or_expr();
    void parse_log_and_expr();
    void parse_bit_or_expr();
    void parse_bit_and_expr();
    void parse_bit_xor_expr();
    void parse_equality_expr();
    void parse_relational_expr();
    void parse_shift_expr_expr();
    void parse_add_expr();
    void parse_mult_expr();
    void parse_cast_expr();
    void parse_unary_expr();
    void parse_postfix_expr();
    void parse_primary_expr();

    void parse_stmt();

    // declarations and definitions
    void parse_decl();
    void parse_local_decl();
    void parse_obj_def();
    void parse_var_def();
    void parse_const_def();
    void parse_val_def();
    void parse_func_def();
    void parse_decl_seq();

    void parse_program();

    private:
    // member functions
    token_name lookahead();
    token_name lookahead(int n);

    Token match(token_name token);
    Token matchif(token_name token);
    Token matchif_eq();
    Token matchif_rel();
    Token matchif_shift();
    Token matchif_add();
    Token matchif_mult();

    Token accept();
    Token peek();
    void fetch();

    // member vars
    Lexer lexer;
    std::deque<Token> token;

};


