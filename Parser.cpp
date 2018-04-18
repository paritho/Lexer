
#include "Parser.hpp"
#include <cassert>


Parser::Parser(Symbol_table& syms, const std::string& source)
    : lexer(syms, source),
      token_que()
{
    fetch();
}

// post type
Type*
Parser::parse_type(){
  return parse_postfix_type();
}

// void | bool | int | float | char
// | ( type list? ) -> type | type
Type* 
Parser::parse_basic_type(){
    Type* t1 = parse_type();

    // should this be first? is it correct?
    // TODO: finish this section
    if(lookahead() == token_left_paren){
        Type_List tl = parse_type_list();
        match(token_right_paren);
        match(token_arrow);
        Type* t2 = parse_type();
        return actions.on_function_type(t1, tl, t2);
    }

    Token token = peek();
    assert(token.get_name() == token_type_specifier);
    switch(token.get_ts_attr()){
        case ts_void:
            return actions.on_void_type(accept());
        case ts_bool:
            return actions.on_bool_type(accept());
        case ts_int:
            return actions.on_int_type(accept());
        case ts_float:
            return actions.on_fp_type(accept());
        case ts_char:
            return actions.on_char_type(accept());
        default:
            break;
    }
    
    return t1;
}

// type-list , type | type
Type_List
Parser::parse_type_list(){
    Type* t1 = parse_basic_type();
    Type_List tl;

    while(Token token = matchif(token_comma)){
        Type* t2 = parse_type();
        tl = actions.on_type_list(token, tl, t1, t2);
    }

    if(tl.size() > 0) return tl;
    
    // if there's only 1 type in the typelist, push it on
    // to the vector so we can still return a Type_List
    tl.push_back(t1);

    return tl;
}

// post * | post const | post volatile | post [ expr ]
// | post [] | basic type
Type*
Parser::parse_postfix_type(){
    Type* t = parse_basic_type();

    if(lookahead(2) == token_left_brace){
        t = parse_postfix_type();
        match(token_left_brace);
        if(lookahead() != token_right_brace){
            Expr* e = parse_primary_expr();
        } 
        match(token_right_brace);
        return actions.on_post_type_expr(t, e);
    }

    // need an assert here??
    Token token = peek();
    switch(token.get_sym_attr()){
        case "*":
            return actions.on_ast_type(accept());
        case "const":
            return actions.on_const_type(accept());
        case "volatile":
            return actions.on_volatile_type(accept());
        default:
            throw std::runtime_error("Expected a postfix type");
    }

    return t;
}

// ref type & | post type
Type* 
Parser::parse_ref_type(){
    Type* t1 = parse_postfix_type();
    
    // not sure how to do this if & can be ref or
    // bitwise op, when whitespace doesn't matter
    if(lookahead() == token_reference) 
        return actions.on_ref_type(accept());

    return t1;
}

// assign expr
Expr* 
Parser::parse_expr(){
   return parse_assign_expr();

}

// cond expr = assign expr | cond expr
Expr*
Parser::parse_assign_expr(){
    Expr* e = parse_conditional_expr();
    Token token = peek();

    if(token.get_name() == token_assignment_op){
        e = parse_assign_expr();
        return actions.on_assign_expr(token, e);
    }

  return e;
}

Expr*
Parser::parse_constant_expr(){
    return parse_conditional_expr();
}

// log or expr ? expr : cond expr | log or expr
Expr*
Parser::parse_conditional_expr(){
    Expr* e = parse_log_or_expr();
    Token token = peek();

    if(token.get_name() == token_conditional_op){
        Token t = match(token_conditional_op);
        Expr* e1 = parse_expr();
        match(token_colon);
        Expr* e2 = parse_conditional_expr();
        return actions.on_conditional_expr(t, e1, e2);
    }

  return e;
}

// log or expr or log and expr | log and expr
Expr* 
Parser::parse_log_or_expr(){
    Expr* e1 = parse_log_and_expr();
    while(Token token = matchif_log_OR()) {
        Expr* e2 = parse_log_and_expr();
        e1 = actions.on_log_or_expr(token, e1, e2);
    }
    return e1;
}

// log and expr AND bit or expr | bit or expr
Expr*
Parser::parse_log_and_expr(){
    Expr* e1 = parse_bit_or_expr();
    while(Token token = matchif_log_AND()) {
        Expr* e2 = parse_bit_or_expr();
        e1 = actions.on_log_and_expr(token, e1, e2);
    } 
    return e1;
}

// bit or expr OR xor expr | xor expr
Expr*
Parser::parse_bit_or_expr(){
    Expr* e1 = parse_bit_xor_expr();
    while(Token token = matchif_bit_or()) {
        Expr* e2 = parse_bit_xor_expr(); 
        e1 = actions.on_bin_or_expr(token, e1, e2);
    } 
    return e1;
}

// bit xor expr ^ bit and expr | bit and expr
Expr*
Parser::parse_bit_xor_expr(){
    Expr* e1 = parse_bit_and_expr();
    while(Token token = matchif_bit_xor()){
        Expr* e2 = parse_bit_and_expr();
        e1 = actions.on_bin_xor_expr(token, e1, e2);
    }
    return e1;
}

// bit and expr & rel expr | rel expr
Expr*
Parser::parse_bit_and_expr(){
    Expr* e1 = parse_relational_expr();
    while(Token token = matchif_bit_and()) {
        Expr* e2 = parse_relational_expr();
        e1 = actions.on_bin_and_expr(token, e1, e2);
    }
    return e1;
}

// eq expr == rel expr | eq != rel | rel expr
Expr*
Parser::parse_equality_expr(){
    Expr* e1 = parse_relational_expr();
    while(Token token = matchif_eq()) {
        Expr* e2 = parse_relational_expr();
        e1 = actions.on_eq_expr(token, e1, e2);
    }
    return e1;
}

// rel expr < shift | rel > shift | rel <= shift
// | rel >= shift | shift epxr
Expr*
Parser::parse_relational_expr(){
    Expr* e1 = parse_shift_expr();
    while(Token token = matchif_rel()) {
        Expr* e2 = parse_shift_expr();
        e1 = actions.on_rel_expr(token, e1, e2);    
    }
    return e1;
}

// shift expr << add expr | shift >> add | add expr
Expr*
Parser::parse_shift_expr(){
    Expr* e1 = parse_add_expr();
    while(Token token = matchif_shift()) {
        Expr* e2 = parse_add_expr();
        e1 = actions.on_shift_expr(token, e1, e2);
    } 
    return e1;
}

// add expr + mult expr | add expr - mult expr
// | mult expr
Expr*
Parser::parse_add_expr(){
    Expr* e1 = parse_mult_expr();
    while(Token token = matchif_add()){
        Expr* e2 = parse_mult_expr();
        e1 = actions.on_add_expr(token, e1, e2);
    }

    return e1;
}

// mult expr * cast expr | mult / cast | mult % cast
// | cast
Expr*
Parser::parse_mult_expr(){
    Expr* e1 = parse_cast_expr();
    while(Token token = matchif_mult()) {
        Expr* e2 = parse_cast_expr();
        e1 = actions.on_mult_expr(token, e1, e2);
    }
    return e1;
}

// cast expr as type | unexpr
Expr*
Parser::parse_cast_expr(){
    Expr* e = parse_unary_expr();
    //TODO: Fix
    return e;

}

// + unexpr | - unexpr | ~ unexpr | !unexpr (<-- this ! not in lang?)
// & unexpr | * unexpr | postfix expr
Expr* 
Parser::parse_unary_expr(){
    Expr* e1 = parse_postfix_expr();
    while(Token token = matchif(token_unary_op)){
        Expr* e2 = parse_unary_expr();   
        e1 = actions.on_unary_expr(token, e2);
    } 
    return e1;
}

// postfix ( arg list ) | postfix [ arg list ] | prim expr
Expr*
Parser::parse_postfix_expr(){
    Expr* e1 = parse_primary_expr();
    switch(lookahead()){
        case token_left_bracket:{
            Expr* e2 = parse_postfix_expr();
            match(token_left_bracket);
            Expr_List el = parse_arg_list();
            match(token_right_bracket);
            return actions.on_post_expr(e1, el, e2);
        }
        case token_left_paren: {
            Expr* e2 = parse_postfix_expr();
            match(token_left_paren);
            Expr_List el = parse_arg_list();
            match(token_right_paren);
            return actions.on_post_expr(e1, el, e2);
        }
        default:
            break;
    }

    return e1;
}

// arg list, arg | arg
Expr_List
Parser::parse_arg_list(){
    Expr* e1 = parse_argument();
    Expr_List el;

    while(Token token = matchif(token_comma)){
        Expr* e2 = parse_argument();
        el = actions.on_arg_list(token, el, e1, e2);
    }

    if(el.size() > 0 ) return el;

    el.push_back(e1);

    return el;
}

Expr*
Parser::parse_argument(){
    return parse_primary_expr();
}

// literal | id | ( expr )
Expr*
Parser::parse_primary_expr(){
    // lookahead returns the token name
    switch(lookahead()){
      // literals
      case token_binary_int:
            return actions.on_bin_int(accept());
      case token_decimal_int:
            return actions.on_dec_int(accept());
      case token_floating_point_literal:
            return actions.on_fp_int(accept());
      case token_hex_int:
            return actions.on_hex_int(accept());
      case token_boolean_literal:
            return actions.on_bool_lit(accept());
      case token_character_literal:
            return actions.on_char_lit(accept());
      case token_string_literal:
            return actions.on_string_lit(accept());

      // identifiers
      case token_identifier:
          return actions.on_id_expr(accept());

      case token_left_paren:
          match(token_left_paren);
          Expr* e = parse_expr();
          match(token_right_paren);
          return e;
      default:
          break; 
    }

    // if no matches, throw exception
    throw std::runtime_error("Primary Expression Expected");

}

// block | if | while | break | continue | return
// | decl | expr
Stmt* 
Parser::parse_stmt(){
    Token token = peek();
    if(token.get_name() == token_left_brace) return parse_block_stmt();
    
    // may need to switch on get_sym_attr().get();
    if(token.get_name() == token_keywords){
        switch(token.get_key_attr()){
            case key_def:
                parse_decl();
                return;
            case key_return:
                parse_return_stmt();
                return;
            case key_continue:
            case key_break:
                parse_breaking_stmt();
                return;
            case key_while:
                parse_while_stmt();
                return;
            case key_if:
                parse_if_stmt();
                return;
            default:
                throw std::runtime_error("Expected a keyword");
        }
    }

    parse_primary_expr();
}

// { stmtseq }
Stmt*
Parser::parse_block_stmt(){
    match(token_left_brace);
    parse_stmtseq();
    match(token_right_brace);
}

// break ; | continue ;
Stmt*
Parser::parse_breaking_stmt(){
    Token token = peek();
    assert(token.get_name() == token_keywords);
    switch(token.get_key_attr()){
        case key_break:
        case key_continue:
            accept();
            break;
        default:
            throw std::runtime_error("Expected Breaking Statment");
    }

    match(token_semicolon);
}

// if ( expr ) stmt | if ( expr ) stmt else stmt
Stmt* 
Parser::parse_if_stmt(){
    assert(peek().get_name() == token_keywords);
    accept();
    match(token_left_paren);
    parse_primary_expr();
    match(token_right_paren);
    parse_stmt();
    Token token = peek();
    if(token.get_name() == token_keywords && token.get_key_attr() == key_else) parse_stmt();
}

// while ( expr ) stmt
Stmt* 
Parser::parse_while_stmt(){
    assert(peek().get_name() == token_keywords);
    accept();
    match(token_left_paren);
    parse_primary_expr();
    match(token_right_paren);
    parse_stmt();
}

// stmtseq stmt | stmt
Stmt*
Parser::parse_stmtseq(){
    while(lookahead() != token_right_brace) parse_stmt();

}

// return expr ; | return ;
Stmt_List
Parser::parse_return_stmt(){
    assert(peek().get_name() == token_keywords);
    accept();
    if(lookahead() == token_semicolon) {
        match(token_semicolon);
        return;
    }
    parse_primary_expr();
    match(token_semicolon);
}

// func decl | local decl
Decl* 
Parser::parse_decl(){
    switch(lookahead()){
        case key_def:{
            token_name name = lookahead(2);
            if(name == token_colon) return parse_local_decl();
            if(name == token_left_paren) return parse_func_def();
            }
            break;
        case key_let:
        case key_var:
            return parse_local_decl();
        default:
            throw std::runtime_error("Declaration Expected");
    }
}

// obj def
Decl* 
Parser::parse_local_decl(){
    return parse_obj_def();
}

// var def | const def | value def
Decl* 
Parser::parse_obj_def(){
    Token token = peek();
    // this may not work. this may need to be token.get_sym_attr().get();
    assert(token.get_name() == token_keywords);
    switch(token.get_key_attr()){
        case key_def:
            return parse_val_def();
        case key_let:
            return parse_const_def();
        case key_var:
            return parse_var_def();
        default:
            throw std::runtime_error("Object Definition Expected");
    }
}

// var ident : type ; | var ident : type = expr
Decl* 
Parser::parse_var_def(){
    assert(peek().get_name() == token_keywords);
    accept();
    Token token = match(token_identifier);
    match(token_colon);
    Type* t = parse_basic_type();

    Decl* d = actions.on_var_decl(token, t);
    Expr* e;

    switch(lookahead()){
        case token_semicolon:
            accept();
            return d;
        default:
            match(token_assignment_op);
            e = parse_primary_expr();
            match(token_semicolon);
            break;
    }
  return actions.on_var_def(token, t, e);
}

// let ident : type = expr ;
Decl* 
Parser::parse_const_def(){
    assert(peek().get_name() == token_keywords);
    accept();
    Token token = match(token_identifier);
    match(token_colon);
    Type* t = parse_basic_type();
    actions.on_const_def(token, t);
    match(token_assignment_op);
    Expr* e = parse_primary_expr();
    match(token_semicolon);

    return actions.on_const_decl(token, t, e);
}

// def ident : type = expr ;
Decl* 
Parser::parse_val_def(){
      assert(peek().get_name() == token_keywords);
      accept();
      Token token = match(token_identifier);
      match(token_colon);
      Type* t = parse_basic_type();
      actions.on_obj_decl(token, t);
      match(token_assignment_op);
      Expr* e = parse_primary_expr();
      match(token_semicolon);

      return actions.on_obj_def(token, t, e);
}

// def ident ( param list? ) -> type block stmt
Decl* 
Parser::parse_func_def(){
    assert(peek().get_name() == token_keywords);
    accept();
    Token token = match(token_identifier);
    match(token_left_paren);
    actions.enter_param_scope();
    Decl_List dl = parse_param_list();
    match(token_right_paren);
    match(token_arrow);
    Type* t = parse_basic_type();
    
    t = actions.construct_function_type(dl, t);
    actions.on_function_decl(token, dl, t);
    Stmt* stmt = parse_block_stmt();
    return actions.on_function_def(token, dl, stmt, t);
}

// param list, param | param
Decl_List
Parser::parse_param_list(){
    Decl* d1 = parse_parameter();
    Decl_List dl;

    while(Token token = match(token_comma)){
        Decl* d2 = parse_parameter();
        dl = actions.on_parse_param_list(token, dl, d2);
    }

    if(dl.size() > 0) return dl;

    dl.pop_back(d1);
    return dl;
}

// ident : type
Decl*
Parser::parse_parameter(){
    Token token = matchif(token_identifier);
    match(token_colon);
    Type* t = parse_basic_type();
    actions.on_parse_param(token, t)
}

// declseq decl | decl
Decl_List
Parser::parse_decl_seq(){
    Decl_List dl;
    while(!token_que.empty()) {
        dl.push_back(parse_decl());
    }
    return dl;
}

// declseq
void 
Parser::parse_program(){
    parse_decl_seq();
}

// member functions
token_name 
Parser::lookahead(){
    assert(!token_que.empty());
    return token_que.front().get_name();
}

token_name 
Parser::lookahead(int n){
    if(n < token_que.size()) return token_que[n].get_name();

    // is this correct?
    n = n - token_que.size() + 1;
    while(--n != 0) fetch();
    
    return token_que.back().get_name();
}

Token 
Parser::match(token_name token){
    if(lookahead() == token) return accept();

    std::stringstream ss;
    Location loc = peek().get_loc();
    ss << "Syntax error";
}

Token 
Parser::matchif(token_name token){
    if(lookahead() == token) return accept();
    return {};
}

Token 
Parser::matchif_eq(){
    if(lookahead() == token_relational_op){
      Token token = peek();
      switch(token.get_relop_attr()){
        case op_equals:
        case op_notequal:
          return accept();
        default:
          return {};
      }
    }
    return {};
}

Token
Parser::matchif_log_OR(){
    if(lookahead() == token_logical_op){
        Token token = peek();
        switch(token.get_log_attr()){
            case logop_OR:
                return accept();
            default:
                throw std::runtime_error("Expected Logical Or");
        }
    }
    return {};
}

Token
Parser::matchif_log_AND(){
    if(lookahead() == token_logical_op){
        Token token = peek();
        switch(token.get_log_attr()){
            case logop_AND:
                return accept();
            default:
                throw std::runtime_error("Expected Logical And");
        }
    }
    return {};
}
Token
Parser::matchif_bit_or(){
    if(lookahead() == token_bitwise_op){
        Token token = peek();
        switch(token.get_bit_attr()){
            case op_or:
                return accept();
            default:
                throw std::runtime_error("Expected Bitwise Or");
        }
    }
    return {};
}
Token
Parser::matchif_bit_xor(){
    if(lookahead() == token_bitwise_op){
        Token token = peek();
        switch(token.get_bit_attr()){
            case op_xor:
                return accept();
            default:
                throw std::runtime_error("Expected Bitwise  Xor");
        }
    }
    return {};
}
Token
Parser::matchif_bit_and(){
    if(lookahead() == token_bitwise_op){
        Token token = peek();
        switch(token.get_bit_attr()){
            case op_and:
                return accept();
            default:
                throw std::runtime_error("Expected Bitwise And");
        }
    }
    return {};
}

Token 
Parser::matchif_rel(){
    if(lookahead() == token_relational_op){
      Token token = peek();
      switch(token.get_relop_attr()){
        case op_lessthan:
        case op_greaterthan:
        case op_ltequals:
        case op_gtequals:
          return accept();
        default:
          return {};
      }
    }
    return {};
}

Token
Parser::matchif_shift(){
    if(lookahead() == token_bitwise_op){
      Token token = peek();
      switch(token.get_bit_attr()){
        case op_leftshift:
        case op_rightshift:
          return accept();
        default:
          return {};
      }
    }
    return {};
}

Token 
Parser::matchif_add(){
    if(lookahead() == token_arithmatic_op){
      Token token = peek();
      switch(token.get_arthop_attr()){
        case op_plus:
        case op_minus:
          return accept();
        default:
          return {};
      }
    }
    return {};
}

Token 
Parser::matchif_mult(){
    if(lookahead() == token_arithmatic_op){
        Token token = peek();
        switch(token.get_arthop_attr()){
          case op_mult:
          case op_divide:
          case op_mod:
            return accept();
          default:
            return {};
        }
    }
    return {};
   
}

Token
Parser::accept(){
    Token token = peek();
    token_que.pop_front();
    if(token_que.empty()) fetch();

    return token;
}

Token
Parser::peek(){
  assert(!token_que.empty());
  return token_que.front();

}

void
Parser::fetch(){
    token_que.push_back(lexer());
}



