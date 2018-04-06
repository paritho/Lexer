#pragma once

#include "AST.hpp"
#include "Token.hpp"

struct Semantics {

    Semantics();
    ~Semantics() = default;

    Type* on_parse_basic_type();
    Type* on_parse_postfix_type();
    Type* on_parse_ref_type();
    Type* on_parse_type_list();

    Expr* on_id_expr(Token t);
    Expr* on_add_expr(Token t, Expr* e1, Expr* e2);
    Expr* on_mult_expr();
    Expr* on_bin_and_expr();
    Expr* on_bin_or_expr();
    Expr* on_bin_xor_expr();
    Expr* on_log_and_expr();
    Expr* on_log_or_expr();
    Expr* on_log_not_expr();
    Expr* on_rel_expr();
    Expr* on_eq_expr();
    Expr* on_shift_expr();
    Expr* on_case_expr();
    Expr* on_unary_expr();
    Expr* on_post_expr();
    Expr* on_parse_args();
    Expr* on_parse_arg_list();

    Stmt* on_parse_stmt();
    Stmt* on_enter_block_stmt();
    Stmt* on_leave_block_stmt();
    Stmt* on_if_stmt();
    Stmt* on_if_else_stmt();
    Stmt* on_while_stmt();
    Stmt* on_return_stmt();
    Stmt* on_breaking_stmt();

    Decl* on_obj_decl();
    Decl* on_obj_def();
    Decl* on_var_decl();
    Decl* on_var_def();
    Decl* on_function_decl();
    Decl* on_function_def();
    Decl* on_const_def();
    Decl* on_parse_param_list();
    Decl* on_parse_params();


}