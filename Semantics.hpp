#pragma once

#include "AST.hpp"
#include "Token.hpp"
#include "Scope.hpp"

struct Semantics {

    Semantics()
        : scope(nullptr)
    {}

    ~Semantics() = default;

    Type* on_void_type(Token t);
    Type* on_bool_type(Token t);
    Type* on_int_type(Token t);
    Type* on_fp_type(Token t);
    Type* on_char_type(Token t);
    Type* on_post_type_expr(Token t, Expr* e);
    Type* on_ast_type(Token t);
    Type* on_const_type(Token t);
    Type* on_volatile_type(Token t);
    Type* on_parse_postfix_type();
    Type* on_ref_type(Token t);
    Type* on_function_type(Type* t1, Type_List tl, Type* t2);
    Type_List on_type_list(Token t, Type_List tl, Type* t1, Type* t2);

    Expr* on_id_expr(Token t);
    Expr* on_bin_int(Token t);
    Expr* on_dec_int(Token t);
    Expr* on_fp_int(Token t);
    Expr* on_hex_int(Token t);
    Expr* on_bool_lit(Token t);
    Expr* on_char_lit(Token t);
    Expr* on_string_lit(Token t);
    Expr* on_add_expr(Token t, Expr* e1, Expr* e2);
    Expr* on_assign_expr(Token t, Expr* e);
    Expr* on_conditional_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_mult_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_bin_and_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_bin_or_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_bin_xor_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_log_and_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_log_or_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_log_not_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_rel_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_eq_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_shift_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_cast_expr();
    Expr* on_unary_expr(Token t, Expr* e1);
    Expr* on_post_expr(Expr* e1, Expr_List el, Expr* e2);
    Expr* on_parse_args();
    // NOTE: check for existance of e1
    Expr_List on_arg_list(Token t, Expr_List el, Expr* e1, Expr* e2);

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
    Decl* on_parse_params();
    Decl_List on_parse_param_list();

    void declare(Decl* d){
        Scope* s = get_current_scope();
        // if the symbol was already declared, throw error
        if(s->lookup_name(d->get_name()){
            std::stringstream ss;
            ss << "Redeclaration of " << d->get_name();
            throw std::runtime_error(ss.str());
        }    
        // otherwise, add symbol to the table
        s->declare(d->get_name());
    }

    // handling scope
    void enter_new_scope(){
        scope = new Global_Scope();
    }

    void leave_current_scope(){
        Scope* current = scope;
        scope = current->get_enclosing_scope();
    }

    Scope* get_current_scope() { return scope; }

    Scope* get_enclosing_scope(){ 
        return scope->get_enclosing_scope();
    }

    private:
    Scope* scope;

}