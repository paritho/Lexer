
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
    return new Post_Type();
}

Type*
Semantics::on_ast_type(Token t){
    return new Ptr_Type();
}

Type*
Semantics::on_parse_postfix_type(){
    // TODO: implement
}

Type*
Semantics::on_ref_type(Token t){
    return new Ref_Type();
}

Type*
Semantics::on_function_type(Type* t1, Type_List tl, Type* t2){
    return new Func_Type();
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

    return new Assign_Expr(*e1->get_type(), e1, e2);
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
Semantics::on_unary_expr(Token token, Expr* e){
    unary_operators uop = token.get_unop_attr();
    Type* t;
    switch(uop){
        case uop_increment:
        case uop_decrement:
            e = mustbe_arith(e);
            t = e->get_type();
            break;
        case uop_log_not:
            e = mustbe_bool(e);
            t = e->get_type();
            break;
        case uop_compare:
            e = mustbe_int(e);
            t = e->get_type();
            break;
        // ref is a ptr
        case uop_ref:
        case uop_ptr:
            e = mustbe_reference(e);
            t = e->get_type();
            break;
    }

    return new Unary_Expr(uop, t, e);
}

Expr*
Semantics::on_call_expr(Expr* e, Expr_List& args){
    e = mustbe_funct(e);
    Func_Type* f = static_cast<Func_Type*>(e->get_type());
    
    // TODO: params need to be in a type_list not expr_list
    Type_List* params = f->get_params();
    if(params.size() < args.size() || args.size() < params.size()) 
        throw std::runtime_error('Arg size doesnt match params');
    //TODO: check if args and params match

    return new Call_Expr(e, f->get_ret_type(), args);
}

Expr*
Semantics::on_index_expr(Expr* e, Expr_List& args){
    Index_Expr* ie = static_cast<Index_Expr*>(e);
    // we don't have array specification?
    return ie;

}

//---------------------------------------------------------------------
//----------------------------- STMT ----------------------------------
//---------------------------------------------------------------------
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

void 
Semantics::enter_block(){
    Scope* enc = get_enclosing_scope();
    if(dynamic_cast<Global_Scope*>(enc)){
        Function_Decl* curFnc = get_cur_func();
        for(Decl* param : curFnc->get_params()) declare(param);
    }
}

void
Semantics::leave_block(){
    leave_current_scope();
}

//------------------------------------------------------------------------
//---------------------------- DECL --------------------------------------
//------------------------------------------------------------------------

//Name lookup happens inside the _def function
Decl*
Semantics::on_obj_decl(Token token, Type* t){
    Decl* d = new Value_Decl(token.get_sym_attr(), t);
    declare(d);
    return d;
}

Decl*
Semantics::on_obj_def(Decl* decl, Type* t, Expr* e){
    Value_Decl* d = static_cast<Value_Decl*>(decl);
    d->set_initialized(e);
    return d;
}

Decl*
Semantics::on_var_decl(Token token, Type* t){
    Decl* d = new Var_Decl(token.get_sym_attr(), t);
    declare(d);
    return d;
}

Decl*
Semantics::on_var_def(Decl* decl, Type* t, Expr* e){
    Var_Decl* d = static_cast<Var_Decl*>(decl);
    d->set_initialized(e);
    return d;
}


Decl*
Semantics::on_const_decl(Token token, Type* t){
    Decl* d = new Const_Decl(token.get_sym_attr(), t);
    declare(d);
    return d;
}

Decl*
Semantics::on_const_def(Decl* decl, Type* t, Expr* e){
    Const_Decl* d = static_cast<Const_Decl*>(decl);
    d->set_initialized(e);
    return d;
}

Decl*
Semantics::on_function_def(Decl* decl, Stmt* s){
    Function_Decl* f = static_cast<Function_Decl*>(decl);
    f->set_body(s);

    currFunc = nullptr;
    return f;
}

Decl*
Semantics::on_function_decl(Token token, Decl_List& dl, Type* t){
    // TODO: params needs to be a type_list not decl_list
    Func_Type* ft = new Func_Type(params, t);
    Function_Decl* f = new Function_Decl(token.get_sym_attr(),parms);
    f->set_type(t);

    declare(f);
    currFunc = f;
    return f;
}   

Decl*
Semantics::on_param_decl(Token token, Type* t){
    Decl* param = new Param_Decl(token.get_sym_attr(), t);
    declare(param);
    return param;
}

Decl*
Semantics::on_program(Decl_List& dl){
    return new Prog_Decl(dl);
}

Decl*
Semantics::lookup(symbol name){
    Scope* s = get_current_scope();
    while(s){
        if(Decl* d = s->lookup(name)) return d;
        // enclosing scope is the parent scope
        s = s->get_enclosing_scope();
    }
    // if here, there was no successfull lookup
    return nullptr;
}

void
Semantics::declare(Decl* d){
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

//------------------------------------------------------------------------
//---------------------------- CONV --------------------------------------
//------------------------------------------------------------------------
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




