
#include "Parser.hpp"
#include <cassert>


Parser::Parser(Symbol_table& syms, const std::string& source)
    : lexer(syms, source),
      token_que()
{
    fetch();
}

// post type
void
Parser::parse_type(){
  parse_postfix_type();

}

// void | bool | int | float | char
// | ( type list? ) -> type | type
void 
Parser::parse_basic_type(){

    

}

// type-list , type | type
void 
Parser::parse_type_list(){
    // while (type) parse_basic_type()

}

// post * | post const | post volatile | post [ expr ]
// | post [] | type
void
Parser::parse_postfix_type(){


}

// ref type & | post type
void 
Parser::parse_ref_type(){

}

// assign expr
void 
Parser::parse_expr(){
    parse_assign_expr();

}

// cond expr = assign expr | cond expr
void 
Parser::parse_assign_expr(){
    parse_conditional_expr();

    switch(lookahead()){
        case op_equals:
            accept();
            parse_assign_expr();
            return;
        default:
          break;
    }
  return;
}

void 
Parser::parse_constant_expr(){
    parse_conditional_expr();
}

// log or expr ? expr : cond expr | log or expr
void 
Parser::parse_conditional_expr(){
    parse_log_or_expr();

    switch(lookahead()){
        case token_conditional_op:
            match(token_conditional_op);
            parse_expr();
            match(token_colon);
            parse_conditional_expr();
            return;
        default:
            break;
    }
  return;
}

// log or expr or log and expr | log and expr
void 
Parser::parse_log_or_expr(){
    parse_log_and_expr();

    while(matchif(token_conditional_op)) parse_log_and_expr();
}

// log and expr AND bit or expr | bit or expr
void 
Parser::parse_log_and_expr(){
    parse_bit_or_expr();
    if(lookahead() == op_and) {
        parse_log_and_expr();
        match(op_and);
        parse_bit_or_expr();
    }
  return;
}

// bit or expr OR xor expr | xor expr
void 
Parser::parse_bit_or_expr(){
    parse_bit_xor_expr();
    if(lookahead() == op_or){
        parse_bit_or_expr();
        match(op_or);
        parse_bit_xor_expr();
    }
  return;
}

// bit xor expr ^ bit and expr | bit and expr
void 
Parser::parse_bit_xor_expr(){
    parse_bit_and_expr();
    if(lookahead() == op_xor){
        parse_bit_xor_expr();
        match(op_xor);
        parse_bit_and_expr();
    }
  return;
}

// bit and expr & rel expr | rel expr
void 
Parser::parse_bit_and_expr(){
    parse_relational_expr();
    if(lookahead() == op_and){
        parse_bit_and_expr();
        match(op_and);
        parse_relational_expr();
    }
  return;
}

// eq expr == rel expr | eq != rel | rel expr
void 
Parser::parse_equality_expr(){
    parse_relational_expr();
    while(matchif_eq()) parse_relational_expr();
}

// rel expr < shift | rel > shift | rel <= shift
// | rel >= shift | shift epxr
void 
Parser::parse_relational_expr(){
    parse_shift_expr();
    while(matchif_rel()) parse_shift_expr();
}

// shift expr << add expr | shift >> add | add expr
void 
Parser::parse_shift_expr(){
    parse_add_expr();
    while(matchif_shift()) parse_add_expr();
}

// add expr + mult expr | add expr - mult expr
// | mult expr
void 
Parser::parse_add_expr(){
    parse_mult_expr();
    while(matchif_add()) parse_mult_expr();

}

// mult expr * cast expr | mult / cast | mult % cast
// | cast
void 
Parser::parse_mult_expr(){
    parse_cast_expr();
    while(matchif_mult()) parse_cast_expr();
}

// cast expr as type | unexpr
void 
Parser::parse_cast_expr(){
    parse_unary_expr();

}

// + unexpr | - unexpr | ~ unexpr | !unexpr (<-- this ! not in lang?)
// & unexpr | * unexpr | postfix expr
void 
Parser::parse_unary_expr(){
    parse_postfix_expr();
    switch(lookahead()){
        case op_plus:
        case op_minus:
        case op_mult:
        case op_not:
        case op_and:
            parse_unary_expr();
        default:
          break;
    }
   
}

// postfix ( arg list ) | postfix [ arg list ] | prim expr
void 
Parser::parse_postfix_expr(){
    parse_primary_expr();
    switch(lookahead()){
        case token_left_bracket:
            match(token_left_bracket);
            parse_arg_list();
            match(token_right_bracket);
            return;
        case token_left_paren:
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
void 
Parser::parse_arg_list(){
    parse_argument();
    while(matchif(token_comma)) parse_argument();
}

void
Parser::parse_argument(){
    parse_primary_expr();
}

// literal | id | ( expr )
void 
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
          accept();
          return;

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
void 
Parser::parse_stmt(){
    switch(lookahead()){
        case token_left_brace:
            parse_block_stmt();
            return;
        case key_if:
            parse_if_stmt();
            return;
        case key_while:
            parse_while_stmt();
            return;
        case key_break:
        case key_continue:
            parse_breaking_stmt();
            return;
        case key_return:
            parse_return_stmt;
            return;
        case key_def:
            parse_decl();
            return;
        default:
            parse_primary_expr();
    }
}

// { stmtseq }
void
Parser::parse_block_stmt(){
    match(token_left_brace);
    parse_stmtseq();
    match(token_right_brace);
}

// break ; | continue ;
void
Parser::parse_breaking_stmt(){
    matchif(key_break);
    matchif(key_continue);
    match(token_semicolon);
}

// if ( expr ) stmt | if ( expr ) stmt else stmt
void 
Parser::parse_if_stmt(){
    assert(peek().get_name() == key_if);
    accept();
    match(token_left_paren);
    parse_primary_expr();
    match(token_right_paren);
    parse_stmt();
    if(lookahead() == key_else) parse_stmt();
}

// while ( expr ) stmt
void 
Parser::parse_while_stmt(){
    assert(peek().get_name() == key_while);
    accept();
    match(token_left_paren);
    parse_primary_expr();
    match(token_right_paren);
    parse_stmt();
}

// stmtseq stmt | stmt
void
Parser::parse_stmtseq(){
    while(lookahead() != token_right_brace) parse_stmt();

}

// return expr ; | return ;
void 
Parser::parse_return_stmt(){
    assert(peek().get_name() == key_return);
    accept();
    if(lookahead() == token_semicolon) {
        match(token_semicolon);
        return;
    }
    parse_primary_expr();
    match(token_semicolon);
}

// func decl | local decl
void 
Parser::parse_decl(){
    switch(lookahead()){
        case key_def:
            token_name name = lookahead(2);
            if(name == token_colon) return parse_local_decl();
            if(name == token_left_paren) return parse_func_def();
        case key_let:
        case key_var:
            return parse_local_decl();
        default:
            throw std::runtime_error("Declaration Expected");
    }
  return;
}

// obj def
void 
Parser::parse_local_decl(){
    parse_obj_def();
}

// var def | const def | value def
void 
Parser::parse_obj_def(){
    switch(lookahead()){
        case key_def:
            parse_val_def;
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
void 
Parser::parse_var_def(){
    assert(peek().get_name() == key_var);
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
void 
Parser::parse_const_def(){
    assert(peek().get_name() == key_let);
    accept();
    match(token_identifier);
    match(token_colon);
    parse_basic_type();
    match(token_assignment_op);
    parse_primary_expr();
    match(token_semicolon);
}

// def ident : type = expr ;
void 
Parser::parse_val_def(){
      assert(peek().get_name() == key_def);
      accept;
      match(token_identifier);
      match(token_colon);
      parse_basic_type();
      match(token_assignment_op);
      parse_primary_expr();
      match(token_semicolon);

}

// def ident ( param list? ) -> type block stmt
void 
Parser::parse_func_def(){
    assert(peek().get_name() == key_def);
    accept();
    match(token_identifier);
    match(token_left_paren);
    parse_param_list();
    match(token_right_paren);
    parse_block_stmt();
}

// param list, param | param
void
Parser::parse_param_list(){
    parse_parameter();
    while(match(token_comma)) parse_parameter();
}

// ident : type
void
Parser::parse_parameter(){
    matchif(token_identifier);
    match(token_colon);
    parse_basic_type();
}

// declseq decl | decl
void 
Parser::parse_decl_seq(){
    while(!token_que.empty()) parse_decl;
}

// declseq
void 
Parser::parse_program(){
    parse_decl_seq();
}

// member functions
token_name 
Parser::lookahead(){
    assert(!token_que.empty);
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
    ss << "Syntax error at: " << peek().display(loc);
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



