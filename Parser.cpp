
#include "Parser.hpp"
#include <cassert>


Parser::Parser(Symbol_table& syms, const std::string& source)
    : lexer(syms, source),
      token()
{
    fetch();
}

void 
Parser::parse_type(){

}

void 
Parser::parse_expr(){

}

void 
Parser::parse_assign_expr(){

}

void 
Parser::parse_conditional_expr(){

}

void 
Parser::parse_log_or_expr(){

}

void 
Parser::parse_log_and_expr(){

}

void 
Parser::parse_bit_or_expr(){

}

void 
Parser::parse_bit_and_expr(){

}

void 
Parser::parse_bit_xor_expr(){

}

void 
Parser::parse_equality_expr(){

}

void 
Parser::parse_relational_expr(){

}

void 
Parser::parse_shift_expr_expr(){

}

void 
Parser::parse_add_expr(){

}

void 
Parser::parse_mult_expr(){

}

void 
Parser::parse_cast_expr(){

}

void 
Parser::parse_unary_expr(){

}

void 
Parser::parse_postfix_expr(){

}

void 
Parser::parse_primary_expr(){

}

void 
Parser::parse_stmt(){

}

void 
Parser::parse_decl(){

}

void 
Parser::parse_local_decl(){

}

void 
Parser::parse_obj_def(){

}

void 
Parser::parse_var_def(){

}

void 
Parser::parse_const_def(){

}

void 
Parser::parse_val_def(){

}

void 
Parser::parse_func_def(){

}

void 
Parser::parse_decl_seq(){

}

void 
Parser::parse_program(){

}

// member functions
token_name 
Parser::lookahead(){
    assert(!token_que.empty);
    return token_que.front().get_name();
}

token_name 
Parser::lookahead(int n){
    
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



