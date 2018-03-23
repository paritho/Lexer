#pragma once

#include "Lexer.hpp"
#include <deque>
#include <string>

struct Parser{
    
    Parser(Symbol_table& syms, const std::string& source);

    void parse_type();
    void parse_basic_type();
    void parse_type_list();
    void parse_postfix_type();
    void parse_ref_type();

    // epxressions
    void parse_expr();
    void parse_assign_expr();
    void parse_constant_expr();
    void parse_conditional_expr();
    void parse_log_or_expr();
    void parse_log_and_expr();
    void parse_bit_or_expr();
    void parse_bit_and_expr();
    void parse_bit_xor_expr();
    void parse_equality_expr();
    void parse_relational_expr();
    void parse_shift_expr();
    void parse_add_expr();
    void parse_mult_expr();
    void parse_cast_expr();
    void parse_unary_expr();
    void parse_postfix_expr();
    void parse_arg_list();
    void parse_argument();
    void parse_primary_expr();

    // stmts
    void parse_stmt();
    void parse_block_stmt();
    void parse_breaking_stmt();
    void parse_if_stmt();
    void parse_while_stmt();
    void parse_stmtseq();
    void parse_return_stmt();

    // declarations and definitions
    void parse_decl();
    void parse_local_decl();
    void parse_obj_def();
    void parse_var_def();
    void parse_const_def();
    void parse_val_def();
    void parse_func_def();
    void parse_param_list();
    void parse_parameter();
    void parse_decl_seq();

    void parse_program();

    private:
    // member functions
    token_name lookahead();
    token_name lookahead(int n);

    Token match(token_name token);
    Token matchif(token_name token);
    Token matchif_eq();
    Token matchif_log_OR();
    Token matchif_log_AND();
    Token matchif_bit_or();
    Token matchif_bit_xor();
    Token matchif_bit_and();
    Token matchif_rel();
    Token matchif_shift();
    Token matchif_add();
    Token matchif_mult();

    Token accept();
    Token peek();
    void fetch();

    // member vars
    Lexer lexer;
    std::deque<Token> token_que;

};


