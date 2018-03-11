
#include "Parser.hpp"

Parser::Parser(Symbol_table& syms, const std::string& source)
    : lexer(syms, source),
      token()
{

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

}

token_name 
Parser::lookahead(int n){

}

Token 
Parser::match(token_name token){

}

Token 
Parser::matchif(token_name token){

}

Token 
Parser::matchif_eq(){

}

Token 
Parser::matchif_rel(){

}

Token
Parser::matchif_shift(){

}

Token 
Parser::matchif_add(){

}

Token 
Parser::matchif_mult(){

}

Token
Parser::accept(){

}

Token
Parser::peek(){

}

void
Parser::fetch(){
    
}



