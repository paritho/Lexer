#pragma once

#include "Lexer.hpp"
#include "AST.hpp"
#include "Semantics.hpp"
#include <deque>
#include <string>

struct Parser{
    
    Parser(Symbol_table& syms, const std::string& source);

    Type* parse_type();
    Type* parse_basic_type();
    Type* parse_postfix_type();
    Type* parse_ref_type();
    Type_List parse_type_list();

    // epxressions
    Expr* parse_expr();
    Expr* parse_assign_expr();
    Expr* parse_constant_expr();
    Expr* parse_conditional_expr();
    Expr* parse_log_or_expr();
    Expr* parse_log_and_expr();
    Expr* parse_bit_or_expr();
    Expr* parse_bit_and_expr();
    Expr* parse_bit_xor_expr();
    Expr* parse_equality_expr();
    Expr* parse_relational_expr();
    Expr* parse_shift_expr();
    Expr* parse_add_expr();
    Expr* parse_mult_expr();
    Expr* parse_cast_expr();
    Expr* parse_unary_expr();
    Expr* parse_postfix_expr();
    Expr* parse_argument();
    Expr* parse_primary_expr();
    Expr_List parse_arg_list();

    // stmts
    Stmt* parse_stmt();
    Stmt* parse_expr_stmt();
    Stmt* parse_block_stmt();
    Stmt* parse_breaking_stmt();
    Stmt* parse_if_stmt();
    Stmt* parse_while_stmt();
    Stmt* parse_return_stmt();
    Stmt_List parse_stmtseq();

    // declarations and definitions
    Decl* parse_decl();
    Decl* parse_local_decl();
    Decl* parse_obj_def();
    Decl* parse_var_def();
    Decl* parse_const_def();
    Decl* parse_val_def();
    Decl* parse_func_def();
    Decl* parse_parameter();
    Decl_List parse_param_list();
    Decl_List parse_decl_seq();

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
    Semantics actions;

};


