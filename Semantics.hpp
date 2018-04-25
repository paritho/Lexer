#pragma once

#include "AST.hpp"
#include "Token.hpp"
#include "Scope.hpp"

struct Semantics {

    Semantics()
        : scope(nullptr)
    {}

    ~Semantics(){
        assert(!scope);
    };

    Type* on_basic_type(Token t);
    Type* on_post_type_expr(Token t, Expr* e);
    Type* on_ast_type(Token t);
    Type* on_parse_postfix_type();
    Type* on_ref_type(Token t);
    Type* on_function_type(Type* t1, Type_List tl, Type* t2);
    Type* construct_function_type(Decl_List dl, Type* t);
    Type_List on_type_list(Token t, Type_List tl, Type* t1, Type* t2);
    Type* get_common_type(Type* t1, Type* t2);

    Expr* on_id_expr(Token t);
    Expr* on_bin_int(Token t);
    Expr* on_dec_int(Token t);
    Expr* on_hex_int(Token t);
    Expr* on_fp_int(Token t);
    Expr* on_bool_lit(Token t);
    Expr* on_char_lit(Token t);
    Expr* on_string_lit(Token t);
    Expr* on_add_expr(Token t, Expr* e1, Expr* e2);
    Expr* on_assign_expr(Expr* e1, Expr* e2);
    Expr* on_conditional_expr(Expr* e1, Expr * e2, Expr* e3);
    Expr* on_mult_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_bin_and_expr( Expr* e1, Expr * e2);
    Expr* on_bin_or_expr(Expr* e1, Expr * e2);
    Expr* on_bin_xor_expr( Expr* e1, Expr * e2);
    Expr* on_log_and_expr( Expr* e1, Expr * e2);
    Expr* on_log_or_expr(Expr* e1, Expr * e2);
    Expr* on_log_not_expr( Expr* e1, Expr * e2);
    Expr* on_rel_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_eq_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_shift_expr(Token t, Expr* e1, Expr * e2);
    Expr* on_cast_expr(Expr* e, Type* t);
    Expr* on_call_expr(Expr* e, Expr_List& args);
    Expr* on_index_expr(Expr* e, Expr_List& args);
    Expr* on_unary_expr(Expr* e1, Expr* e2);
    Expr* on_post_expr(Expr* e1, Expr_List el, Expr* e2);

    Stmt* on_if_stmt(Expr* e, Stmt* s);
    Stmt* on_if_else_stmt(Expr* e, Stmt* s1, Stmt* s2);
    Stmt* on_while_stmt(Expr* e, Stmt* s);
    Stmt* on_return_stmt();
    Stmt* on_break_stmt();
    Stmt* on_continue_stmt();
    Stmt* on_decl_stmt();
    Stmt* on_expr_stmt();
    Stmt* on_block_stmt(Stmt_List& sl);
    void enter_block();
    void leave_block();

    // Declarations. Name lookup happens inside the _def function
    Decl* on_obj_decl(Token token, Type* t);
    Decl* on_obj_def(Token token, Type* t, Expr* e);
    Decl* on_var_decl(Token token, Type* t);
    Decl* on_var_def(Token token, Type* t, Expr* e);
    Decl* on_function_decl(Token token, Decl_List dl, Type* t);
    Decl* on_function_def(Token token, Decl_List dl, Stmt* stmt, Type* t);
    Decl* on_param_decl(Token token, Type* t);
    Decl* on_program(Decl_List& dl);

    void declare(Decl* d){
        Scope* s = get_current_scope();
        // if the symbol was already declared, throw error
        if(s->lookup_name(d->get_name())){
            std::stringstream ss;
            ss << "Redeclaration of " << d->get_name();
            throw std::runtime_error(ss.str());
        }    
        // otherwise, add symbol to the table
        s->declare(d->get_name());
    }

    Decl* lookup(symbol name);

    // handling type converstions
    Expr* conv_tval(Expr* e);
    Expr* conv_tbool(Expr* e);
    Expr* conv_tint(Expr* e);
    Expr* conv_tfp(Expr* e);
    Expr* conv_tchar(Expr* e);
    Expr* conv_ttype(Expr* e, Type* t);

    // asserting functions
    Expr* mustbe_reference(Expr* e);
    Expr* mustbe_value(Expr* e);
    bool mustbe_same(Type* t1, Type* t2);
    Expr* mustbe_int(Expr* e);
    Expr* mustbe_bool(Expr* e);
    Expr* mustbe_funct(Expr* e);
    Expr* mustbe_arith(Expr* e);

    // handling scope
    void enter_global_scope();
    void enter_param_scope();
    void leave_current_scope(){
        Scope* current = scope;
        scope = current->get_enclosing_scope();
    };

    Scope* get_current_scope() { return scope; };

    Scope* get_enclosing_scope(){ 
        return scope->get_enclosing_scope();
    };

    private:
    Scope* scope;

};