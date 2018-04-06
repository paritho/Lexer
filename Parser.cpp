
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
    Type* t = parse_type();

    if(lookahead() == token_left_paren){
        parse_type_list();
        match(token_right_paren);
        match(token_arrow);
        parse_type();
        return;
    }

    Token token = peek();
    assert(token.get_name() == token_type_specifier);
    switch(token.get_ts_attr()){
        case ts_void:
        case ts_bool:
        case ts_int:
        case ts_float:
        case ts_char:
            parse_type();
            return;
        default:
            return;
    }
    return;
}

// type-list , type | type
Type_List
Parser::parse_type_list(){
    parse_basic_type();
    while(lookahead() == token_semicolon) parse_basic_type(); 
}

// post * | post const | post volatile | post [ expr ]
// | post [] | basic type
Type*
Parser::parse_postfix_type(){
    parse_basic_type();

    if(lookahead(2) == token_left_brace){
        parse_postfix_type();
        match(token_left_brace);
        if(lookahead() != token_right_brace) parse_primary_expr();
        match(token_right_brace);
        return;
    }
    while(lookahead() == token_unary_op) parse_postfix_type();

}

// ref type & | post type
Type* 
Parser::parse_ref_type(){

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
    assert(token.get_name() == token_relational_op);
    if(token.get_relop_attr() == op_equals){
        accept();
        // semantics.on_assignment_expr
        parse_assign_expr();
        return;
    }
  return;
}

Expr*
Parser::parse_constant_expr(){
    return parse_conditional_expr();
}

// log or expr ? expr : cond expr | log or expr
Expr*
Parser::parse_conditional_expr(){
    Expr* e = parse_log_or_expr();

    switch(lookahead()){
        case token_conditional_op:
            match(token_conditional_op);
            Expr* ex1 = parse_expr();
            match(token_colon);
            Expr* ex2 = parse_conditional_expr();
            // return on_conditional_expr;
            return;
        default:
            break;
    }
  return e;
}

// log or expr or log and expr | log and expr
Expr* 
Parser::parse_log_or_expr(){
    parse_log_and_expr();
    while(matchif_log_OR()) parse_log_and_expr();
}

// log and expr AND bit or expr | bit or expr
Expr*
Parser::parse_log_and_expr(){
    parse_bit_or_expr();
    while(matchif_log_AND()) parse_bit_or_expr();
     
}

// bit or expr OR xor expr | xor expr
Expr*
Parser::parse_bit_or_expr(){
    parse_bit_xor_expr();
    while(matchif_bit_or()) parse_bit_xor_expr(); 
}

// bit xor expr ^ bit and expr | bit and expr
Expr*
Parser::parse_bit_xor_expr(){
    parse_bit_and_expr();
    while(matchif_bit_xor()) parse_bit_and_expr();
}

// bit and expr & rel expr | rel expr
Expr*
Parser::parse_bit_and_expr(){
    parse_relational_expr();
    while(matchif_bit_and()) parse_relational_expr();
}

// eq expr == rel expr | eq != rel | rel expr
Expr*
Parser::parse_equality_expr(){
    parse_relational_expr();
    while(matchif_eq()) parse_relational_expr();
}

// rel expr < shift | rel > shift | rel <= shift
// | rel >= shift | shift epxr
Expr*
Parser::parse_relational_expr(){
    parse_shift_expr();
    while(matchif_rel()) parse_shift_expr();
}

// shift expr << add expr | shift >> add | add expr
Expr*
Parser::parse_shift_expr(){
    parse_add_expr();
    while(matchif_shift()) parse_add_expr();
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
    parse_cast_expr();
    while(matchif_mult()) parse_cast_expr();
}

// cast expr as type | unexpr
Expr*
Parser::parse_cast_expr(){
    parse_unary_expr();

}

// + unexpr | - unexpr | ~ unexpr | !unexpr (<-- this ! not in lang?)
// & unexpr | * unexpr | postfix expr
Expr* 
Parser::parse_unary_expr(){
    parse_postfix_expr();
    while(matchif(token_unary_op)) parse_unary_expr();   
}

// postfix ( arg list ) | postfix [ arg list ] | prim expr
Expr*
Parser::parse_postfix_expr(){
    parse_primary_expr();
    switch(lookahead()){
        case token_left_bracket:
            parse_postfix_expr();
            match(token_left_bracket);
            parse_arg_list();
            match(token_right_bracket);
            return;
        case token_left_paren:
            parse_postfix_expr();
            match(token_left_paren);
            parse_arg_list();
            match(token_right_paren);
            return;
        default:
            break;
    }
    return;
}

// arg list, arg | arg
Expr_List
Parser::parse_arg_list(){
    parse_argument();
    while(matchif(token_comma)) parse_argument();
}

Expr*
Parser::parse_argument(){
    parse_primary_expr();
}

// literal | id | ( expr )
Expr*
Parser::parse_primary_expr(){
    // lookahead returns the token name
    switch(lookahead()){
      // literals
      case token_binary_int:
      case token_decimal_int:
      case token_floating_point_literal:
      case token_boolean_literal:
      case token_hex_int:
      case token_character_literal:
      case token_string_literal:
          accept();
          return;

      // identifiers
      case token_identifier:
          return actions.on_id_expr(accept());

      case token_left_paren:
          match(token_left_paren);
          parse_expr();
          match(token_right_paren);
          return;
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
  return;
}

// obj def
Decl* 
Parser::parse_local_decl(){
    parse_obj_def();
}

// var def | const def | value def
Decl* 
Parser::parse_obj_def(){
    Token token = peek();
    // this may not work. this may need to be token.get_sym_attr().get();
    assert(token.get_name() == token_keywords);
    switch(token.get_key_attr()){
        case key_def:
            parse_val_def();
            return;
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
    match(token_identifier);
    match(token_colon);
    parse_basic_type();
    switch(lookahead()){
        case token_semicolon:
            accept();
            return;
        default:
            match(token_assignment_op);
            parse_primary_expr();
            match(token_semicolon);
            break;
    }
  return;
}

// let ident : type = expr ;
Decl* 
Parser::parse_const_def(){
    assert(peek().get_name() == token_keywords);
    accept();
    match(token_identifier);
    match(token_colon);
    parse_basic_type();
    match(token_assignment_op);
    parse_primary_expr();
    match(token_semicolon);
}

// def ident : type = expr ;
Decl* 
Parser::parse_val_def(){
      assert(peek().get_name() == token_keywords);
      accept();
      match(token_identifier);
      match(token_colon);
      parse_basic_type();
      match(token_assignment_op);
      parse_primary_expr();
      match(token_semicolon);

}

// def ident ( param list? ) -> type block stmt
Decl* 
Parser::parse_func_def(){
    assert(peek().get_name() == token_keywords);
    accept();
    match(token_identifier);
    match(token_left_paren);
    parse_param_list();
    match(token_right_paren);
    parse_block_stmt();
}

// param list, param | param
Decl*
Parser::parse_param_list(){
    parse_parameter();
    while(match(token_comma)) parse_parameter();
}

// ident : type
Decl*
Parser::parse_parameter(){
    matchif(token_identifier);
    match(token_colon);
    parse_basic_type();
}

// declseq decl | decl
Decl_List
Parser::parse_decl_seq(){
    while(!token_que.empty()) parse_decl();
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



