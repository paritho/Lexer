
#include "Semantics.hpp"
#include "AST.hpp"

Types*
Semantics::on_void_type(Token t){

}

Types*
Semantics::on_bool_type(Token t){

}

Types*
Semantics::on_int_type(Token t){

}

Types*
Semantics::on_fp_type(Token t){

}

Types*
Semantics::on_char_type(Token t){

}

Types*
Semantics::on_post_type_expr(Token t, Expr* e){

}

Types*
Semantics::on_ast_type(Token t){

}

Types*
Semantics::on_const_type(Token t){

}

Types*
Semantics::on_volatile_type(Token t){

}

Types*
Semantics::on_parse_postfix_type(){

}

Types*
Semantics::on_ref_type(Token t){

}

Types*
Semantics::on_function_type(Types* t1, Type_List tl, Types* t2){

}

Types*
Semantics::construct_function_type(Decl_List dl, Types* t){

}

Type_List
Semantics::on_type_list(Token t, Type_List tl, Types* t1, Types* t2){

}

//-------------------------------------------------------------------
//---------------------------- EXPR----------------------------------
//-------------------------------------------------------------------
Expr*
Semantics::on_id_expr(Token t){
    assert(t.get_name() == token_identifier);
    symbol sym = t.get_sym_attr();
    
    // never deletable. this is bad
    return new Id_Expr(sym);
}

Expr*
Semantics::on_bin_int(Token t){
    assert(t.get_name() == token_binary_int);
    integer_token it = t.get_intval_attr();
    long long num = it.get_value();

    return new Int_Expr(num);
}

Expr*
Semantics::on_dec_int(Token t){
    assert(t.get_name() == token_decimal_int);
    integer_token it = t.get_intval_attr();
    long long num = it.get_value();

    return new Int_Expr(num);
}

Expr*
Semantics::on_hex_int(Token t){
    assert(t.get_name() == token_hex_int);
    integer_token it = t.get_intval_attr();
    long long num = it.get_value();

    return new Int_Expr(num);
}

Expr*
Semantics::on_fp_int(Token t){
    assert(t.get_name() == token_floating_point_literal);
    double num = t.get_float_attr();

    return new Float_Expr(num);
}


Expr*
Semantics::on_bool_lit(Token t){
    assert(t.get_name() == token_boolean_literal);
    bool b = t.get_bool_attr();

    return new Bool_Expr(b);
}

Expr*
Semantics::on_char_lit(Token t){
    assert(t.get_name() == token_character_literal);
    char c = t.get_ch_attr();

    return new Char_Expr(c);
}

Expr*
Semantics::on_string_lit(Token t){
    assert(t.get_name() == token_string_literal);
    symbol s = t.get_str_attr();

    return new String_Expr(s);
}

Expr*
Semantics::on_add_expr(Token t, Expr* e1, Expr* e2){

}

Expr*
Semantics::on_assign_expr(Token t, Expr* e){

}

Expr*
Semantics::on_conditional_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_mult_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_bin_and_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_bin_or_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_bin_xor_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_log_and_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_log_or_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_log_not_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_rel_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_eq_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_shift_expr(Token t, Expr* e1, Expr * e2){

}

Expr*
Semantics::on_cast_expr(){

}

Expr*
Semantics::on_unary_expr(Token t, Expr* e1){

}

Expr*
Semantics::on_post_expr(Expr* e1, Expr_List el, Expr* e2){

}

// NOTE: check for existance of e1
Expr_List
Semantics::on_arg_list(Token t, Expr_List el, Expr* e1, Expr* e2){

}

//---------------------------------------------------------------------
//----------------------------- STMT ----------------------------------
//---------------------------------------------------------------------
Stmt*
Semantics::on_parse_stmt(){

}

Stmt*
Semantics::on_enter_block_stmt(){

}

Stmt*
Semantics::on_leave_block_stmt(){

}

Stmt*
Semantics::on_if_stmt(){

}

Stmt*
Semantics::on_if_else_stmt(){

}

Stmt*
Semantics::on_while_stmt(){

}

Stmt*
Semantics::on_return_stmt(){

}

Stmt*
Semantics::on_breaking_stmt(){

}

//------------------------------------------------------------------------
//---------------------------- DECL --------------------------------------
//------------------------------------------------------------------------

//Name lookup happens inside the _def function
Decl*
Semantics::on_obj_decl(Token token, Types* t){

}

Decl*
Semantics::on_obj_def(Token token, Types* t, Expr* e){

}

Decl*
Semantics::on_var_decl(Token token, Types* t){

}

Decl*
Semantics::on_var_def(Token token, Types* t, Expr* e){

}

Decl*
Semantics::on_function_decl(Token token, Decl_List dl, Types* t){

}

Decl*
Semantics::on_function_def(Token token, Decl_List dl, Stmt* stmt, Types* t){

}

Decl*
Semantics::on_const_def(Token token, Types* t){

}

Decl*
Semantics::on_const_decl(Token token, Types* t, Expr* e){

}

Decl*
Semantics::on_parse_param(Token token, Types* t){

}

Decl_
Semantics::ist on_parse_param_list(Token token, Decl_List dl, Decl* d){

}
