
#include "Semantics.hpp"
#include "AST.hpp"

// for basic Type, we only need to accept the token
// we don't need to do anything with it (right now)
Type*
Semantics::on_basic_type(Token t){
    switch(t.get_ts_attr()){
        case ts_void:
            return new Void_Type();
        case ts_bool:
            return new Bool_Type();
        case ts_int:
            return new Int_Type();
        case ts_float:
            return new Float_Type();
        case ts_char:
            return new Char_Type();
    }
}

Type*
Semantics::on_post_type_expr(Token t, Expr* e){

}

Type*
Semantics::on_ast_type(Token t){

}

Type*
Semantics::on_parse_postfix_type(){

}

Type*
Semantics::on_ref_type(Token t){

}

Type*
Semantics::on_function_type(Type* t1, Type_List tl, Type* t2){

}

Type*
Semantics::construct_function_type(Decl_List dl, Type* t){

}

Type_List
Semantics::on_type_list(Token t, Type_List tl, Type* t1, Type* t2){

}

Type*
Semantics::get_common_type(Type* t1, Type* t2){
    if(is_same_type(t1, t2)) return t1;
    if(t1->is_ref_to(t2)) return t2;
    if(t2->is_ref_to(t1)) return t1;

    throw std::runtime_error("These have no common type, sorry");
}

//-------------------------------------------------------------------
//---------------------------- EXPR----------------------------------
//-------------------------------------------------------------------
Expr*
Semantics::on_id_expr(Token t){
    assert(t.get_name() == token_identifier);
    symbol sym = t.get_sym_attr();
    Decl* d = lookup(sym);

    // never deletable. this is bad
    return new Id_Expr(d);
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
    e1 = mustbe_arith(e1);
    e2 = mustbe_arith(e2);

    Type* t = mustbe_same(e1->get_type(), e2->get_type());

    arithmatic_operators arop = t.get_arthop_attr();
    // TODO: conv arop to binop
    return new Bin_Expr(arith_kind, arop, e1, e2);
}

Expr*
Semantics::on_assign_expr(Expr* e1, Expr* e2){
    e1 = mustbe_reference(e1);
    e2 = mustbe_value(e2);

    Type* t1 = e1->get_type()->get_obj_type();
    Type* t2 = e2->get_type();

    if(!mustbe_same(t1, t2)) 
        throw std::runtime_error("Types in Assign Expr not same");

    return new Assign_Expr(e1->get_type(), e1, e2);
}

Expr*
Semantics::on_conditional_expr(Expr* e1, Expr * e2, Expr* e3){
    e1 = mustbe_bool(e1);

    Type* ct = get_common_type(e1->get_type(), e2->get_type());
    e2 = conv_ttype(e2, ct);
    e3 = conv_ttype(e3, ct);

    return new Conditional_Expr(e1, e2, e3);
}

Expr*
Semantics::on_mult_expr(Token t, Expr* e1, Expr * e2){
    e1 = mustbe_arith(e1);
    e2 = mustbe_arith(e2);

    if(!mustbe_same(e1->get_type(), e2->get_type()))
        throw std::runtime_error("Can only multiply same types");
    
    arithmatic_operators arop = t.get_arthop_attr();
    // TODO: conv arop to binop
    return new Bin_Expr(arith_kind, arop, e1, e2);
}

Expr*
Semantics::on_bin_and_expr(Expr* e1, Expr * e2){
    e1 = mustbe_bool(e1);
    e2 = mustbe_bool(e2);
    return new Bin_Expr(bit_kind, bin_and_op, e1, e2);
}

Expr*
Semantics::on_bin_or_expr(Expr* e1, Expr * e2){
    e1 = mustbe_bool(e1);
    e2 = mustbe_bool(e2);
    return new Bin_Expr(bit_kind, bin_or_op, e1, e2);
}

Expr*
Semantics::on_bin_xor_expr(Expr* e1, Expr * e2){
    e1 = mustbe_bool(e1);
    e2 = mustbe_bool(e2);
    return new Bin_Expr(bit_kind, bit_xor_op, e1, e2);
}

Expr*
Semantics::on_log_and_expr( Expr* e1, Expr * e2){
    e1 = mustbe_bool(e1);
    e2 = mustbe_bool(e2);
    return new Bin_Expr(log_kind, log_not_op, e1, e2);
}

Expr*
Semantics::on_log_or_expr(Expr* e1, Expr * e2){
    e1 = mustbe_bool(e1);
    e2 = mustbe_bool(e2);
    return new Bin_Expr(log_kind, log_or_op, e1, e2);
}

Expr*
Semantics::on_log_not_expr(Expr* e1, Expr * e2){
    e1 = mustbe_bool(e1);
    e2 = mustbe_bool(e2);
    return new Bin_Expr(log_kind, log_not_op, e1, e2);
}

Expr*
Semantics::on_rel_expr(Token t, Expr* e1, Expr * e2){
    // mustbe arith checks for int or float, which are the
    // two types that can be checked relationally
    e1 = mustbe_arith(e1);
    e2 = mustbe_arith(e2);    
    // TODO: conv from relational_operators to binop
    relational_operators relop = t.get_relop_attr();
    return new Bin_Expr(rel_kind, relop, e1, e2);
}

Expr*
Semantics::on_eq_expr(Token t, Expr* e1, Expr * e2){
    relational_operators relop = t.get_relop_attr();
    return new Bin_Expr(bool_kind, equal_op, e1, e2);
}

Expr*
Semantics::on_shift_expr(Token t, Expr* e1, Expr * e2){
    e1 = mustbe_int(e1);
    e2 = mustbe_int(e2); 
    bitwise_operators bitop = t.get_bit_attr();
    // TODO: conv bitop to binop
    return new Bin_Expr(int_kind, bitop, e1, e2);
}

Expr*
Semantics::on_cast_expr(Expr* e, Type* t){
    return new Cast_Expr(conv_ttype(e,t), t);
}

Expr*
Semantics::on_unary_expr(Expr* e1, Expr* e2){

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
Semantics::on_if_stmt(Expr* e, Stmt* s){
    return new If_Stmt(e, s);
}

Stmt*
Semantics::on_if_else_stmt(Expr* e, Stmt* s1, Stmt* s2){
    return new If_Else_Stmt(e, s1, s2);
}

Stmt*
Semantics::on_while_stmt(Expr* e, Stmt* s){
    return new While_Stmt(e, s);
}

Stmt*
Semantics::on_return_stmt(){
    return new Breaking_Stmt(return_stmt);
}

Stmt*
Semantics::on_break_stmt(){
    return new Breaking_Stmt(break_stmt);
}

Stmt* 
Semantics::on_continue_stmt(){
    return new Breaking_Stmt(continue_stmt);
}

//------------------------------------------------------------------------
//---------------------------- DECL --------------------------------------
//------------------------------------------------------------------------

//Name lookup happens inside the _def function
Decl*
Semantics::on_obj_decl(Token token, Type* t){

}

Decl*
Semantics::on_obj_def(Token token, Type* t, Expr* e){

}

Decl*
Semantics::on_var_decl(Token token, Type* t){

}

Decl*
Semantics::on_var_def(Token token, Type* t, Expr* e){

}

Decl*
Semantics::on_function_decl(Token token, Decl_List dl, Type* t){

}

Decl*
Semantics::on_function_def(Token token, Decl_List dl, Stmt* stmt, Type* t){

}

Decl*
Semantics::on_param_decl(Token token, Type* t){

}

Decl*
Semantics::on_program(Decl_List& dl){

}

Expr*
conv_tval(Expr* e){
    if(e->get_kind() == ref_kind)
        return new Conversion_Expr(e, value, new Ref_Type());
    return e;
}

Expr*
conv_tbool(Expr* e){
    e = conv_tval(e);
    switch(e->get_kind()){
        case char_kind:
        case int_kind:
        case float_kind:
        case ptr_kind:
        case func_kind:
            return new Conversion_Expr(e, ctbool, new Bool_Type());
        case bool_kind:
            return e;
        default:
            throw std::runtime_error("Can't Make it A Bool");
    }
}

Expr*
conv_tint(Expr* e){
    e = conv_tval(e);
    switch(e->get_kind()){
        case int_kind:
            return e;
        case bool_kind:
        case char_kind:
            return new Conversion_Expr(e, ctint, new Int_Type());
        case float_kind:
            return new Conversion_Expr(e, ctrunc, new Int_Type());
        default:
            throw std::runtime_error("Can't Make it an Int");
    }
}

Expr*
conv_tfp(Expr* e){
    e = conv_tval(e);
    switch(e->get_kind()){
        case int_kind:
            return new Conversion_Expr(e, ctfloat, new Float_Type());
        case float_kind:
            return e;
        default:
            throw std::runtime_error("Can't Make it a Float");
    }
    
}

Expr*
conv_tchar(Expr* e){
    e = conv_tval(e);
    switch(e->get_kind()){
        case char_kind:
            return e;
        case int_kind:
            return new Conversion_Expr(e, ctchar, new Char_Type());
        default:
            throw std::runtime_error("Can't Make it a Char");
    }
}

Expr*
conv_ttype(Expr* e, Type* t){
    if(t->get_name() != type_ref) e = conv_tval(e);

    // are they already the same type?
    if(e->get_type()->get_name() == t->get_name()) return e;

    switch(t->get_name()){
        case type_bool:
            return conv_tbool(e);
        case type_int:
            return conv_tint(e);
        case type_char:
            return conv_tchar(e);
        case type_float:
            return conv_tfp(e);
        default:
            throw std::runtime_error("Can't Make it that type");
    }
}

Expr* 
mustbe_reference(Expr* e){
    if(e->get_kind() != ref_kind) 
        throw std::runtime_error("Expected a reference");
    return e;
}

Expr* 
mustbe_value(Expr* e){
    return conv_tval(e);    
}

bool 
mustbe_same(Type* t1, Type* t2){
    return is_same_type(t1, t2);
}

Expr* 
mustbe_int(Expr* e){
    if(e->get_kind() != int_kind) 
        throw std::runtime_error("Epected int");
    return e;
}

Expr* 
mustbe_bool(Expr* e){
    if(e->get_kind() != bool_kind)
        throw std::runtime_error("Epected a bool");
    return e;
}

Expr* 
mustbe_funct(Expr* e){
    if(e->get_kind() != func_kind)
        throw std::runtime_error("Expected a function");
    return e;    
}

Expr* 
mustbe_arith(Expr* e){
    e = mustbe_value(e);
    if(e->get_kind() == int_kind || e->get_kind() == float_kind)
        throw std::runtime_error("Expect and arithmatic expr");
    return e;
}




