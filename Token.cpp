
#include "Token.hpp"


//constructors
Token::Token()
    :name(token_eof)
{}

Token::Token(token_name name, Location loc)
    : name(name), 
      location(loc)
{
    assert(!has_attribute());
}

Token::Token(token_name name, attribute at, Location loc)
    : name(name),
      attr(at)
{ }

Token::Token(symbol symbol, Location loc)
    : name(token_identifier),
      attr(symbol),
      location(loc)
{ }

// to construct string literal and exponential tokens
Token::Token(symbol str, token_name name, Location loc)
    : name(name),
      attr(str)
{ }

Token::Token(keywords key, Location loc)
    : name(token_keywords),
      attr(key),
      location(loc)
{}

Token::Token(relational_operators rop, Location loc)
    : name(token_relational_op), 
      attr(rop),
      location(loc)
{ }

Token::Token(arithmatic_operators aop, Location loc)
    : name(token_arithmatic_op),
      attr(aop),
      location(loc)      
{ }

Token::Token(bitwise_operators bop, Location loc)
    : name(token_bitwise_op),
      attr(bop),
      location(loc)
{ }

Token::Token(logical_operators lop, Location loc)
    : name(token_logical_op),
      attr(lop),
      location(loc)
{ }

Token::Token(unary_operators uop, Location loc)
    : name(token_unary_op),
      attr(uop),
      location(loc)
{ }

Token::Token(type_specifier ts, Location loc)
    : name(token_type_specifier),
      attr(ts),
      location(loc)
{ }

Token::Token(radix rad, long long value, Location loc)
    : name(get_int_base(rad)),
      attr(integer_token{rad, value}),
      location(loc)
{ }

Token::Token(char c, Location loc)
    : name(token_character_literal),
      attr(c),
      location(loc)
{ }

Token::Token(double num, Location loc)
    : name(token_floating_point_literal),
      attr(num),
      location(loc)
{ }

Token::Token(bool boolval, Location loc)
    : name(token_boolean_literal),
      attr(boolval),
      location(loc)
{ }


token_name
Token::get_int_base(radix base){
    switch(base){
        case binary:
            return token_binary_int;
        case decimal:
            return token_decimal_int;
        case hexidecimal:
            return token_hex_int;
    }
}

// functions to make the token display in the console
std::string
display(token_name name){
    switch(name){
        case token_left_brace:
            return "left brace";
        case token_right_brace:
            return "right brace";
        case token_left_paren:
            return "left paren";
        case token_right_paren:
            return "right paren";
        case token_left_bracket:
            return "left bracket";
        case token_right_bracket:
            return "right bracket";
        case token_comma:
            return "comma";
        case token_semicolon:
            return "semicolon";
        case token_colon:
            return "colon";
        case token_conditional_op:
            return "conditional operator";
        case token_assignment_op:
            return "assignment operator";
        case token_relational_op:
            return "relational operator";
        case token_arithmatic_op:
            return "arithmatic operator";
        case token_bitwise_op:
            return "bitwise operator";
        case token_logical_op:
            return "logical operator";
        case token_type_specifier:
            return "type specifier";
        case token_keywords:
            return "keyword";
        case token_identifier:
            return "identifier";
        case token_decimal_int:
            return "decimal integer";
        case token_binary_int:
            return "binary integer";
        case token_hex_int:
            return "hex integer";
        case token_floating_point_literal:
            return "floating point literal";
        case token_boolean_literal:
            return "boolean";
        case token_character_literal:
            return "character literal";
        case token_string_literal:
            return "string";
    }
}

std::string
display(symbol sym){
    return *sym;
}                  

std::string
display(keywords keyword){
    switch(keyword){
        case key_def:
            return "def";
        case key_else:
            return "else";
        case key_if:
            return "if";
        case key_let:
            return "let";
        case key_var:
            return "var";
        default:
            throw std::runtime_error("not a keyword");
    }
}

std::string
display(relational_operators op){
    switch(op){
        case op_equals:
            return "== ";
        case op_notequal:
            return "!= ";
        case op_lessthan:
            return "< ";
        case op_greaterthan:
            return "> ";
        case op_ltequals:
            return "<= ";
        case op_gtequals:
            return ">= ";
    }
}

std::string
display(arithmatic_operators op){
    switch(op){
        case op_plus:
            return "+ ";
        case op_minus:
            return "- ";
        case op_mult:
            return "* ";
        case op_divide:
            return "/ ";
        case op_ltequals:
            return "\% ";
    }
}

std::string
display(bitwise_operators op){
    switch(op){
        case op_and:
            return "& ";
        case op_or:
            return "| ";
        case op_xor:
            return "^ ";
        case op_leftshift:
            return "<< ";
        case op_rightshift:
            return ">> ";
        case op_not:
            return "~ ";
    }
}

std::string
display(logical_operators op){
    switch(op){
        case logop_AND:
            return "and";
        case logop_OR:
            return "or";
        case logop_NOT:
            return "not";  
    }
}

std::string
display(unary_operators op){
    switch(op){
        case uop_asterix:
            return "*";
        case uop_const:
            return "const";
        case uop_volatile:
            return "volatile";
        case uop_increment:
            return "+";
        case uop_decrement:
            return "-";
        case uop_bit_not:
            return "~";
        case uop_log_not:
            return "!";
        case uop_ref:
            return "&";
    }
}

std::string
display(type_specifier type){
    switch(type){
        case ts_bool:
            return "bool";
        case ts_char:
            return "char";
        case ts_int:
            return "int";
        case ts_float:
            return "float";
        case ts_void:
            return "void";
    }
}

std::string 
display(integer_token num){
    return std::to_string(num.value);
}

std::string
display(char c){ 
    std::ostringstream ss;
    // if the char is an escape char, we need to escape
    // it to show it
    std::string s;
    switch(c){
        case '\a':
            s = "\\a";
            break;
        case '\b':
            s = "\\b";
            break;
        case '\f':
            s = "\\f";
            break;
        case '\n':
            s = "\\n";
            break;
        case '\r':
            s = "\\r";
            break;
        case '\t':
            s = "\\t";
            break;
        case '\v':
            s = "\\v";
            break;
        default:
            s += c;
    }

    ss << s;

    return ss.str();
}

std::string
display(double num){
    std::ostringstream ss;
    ss << num;
    return ss.str();
}

std::string
display(long long num){
    std::ostringstream ss;
    ss << num;
    return ss.str();
}

std::string
display(bool boolval){
    return boolval ? "true" : "false";
}

std::string 
display(Location loc){
    std::ostringstream ss;
    ss << "line: " << loc.get_line();
    ss << " column: " << loc.get_column();
    ss << " in file: " << loc.get_file();
    return ss.str();
}

bool
Token::has_attribute(){
    switch(name){
        // list of tokens that need/have an attribute
        case token_hex_int:
        case token_binary_int:
        case token_decimal_int:
        case token_floating_point_literal:
        case token_boolean_literal:
        case token_character_literal:
        case token_string_literal:
        case token_identifier:
        case token_relational_op:
        case token_arithmatic_op:
        case token_bitwise_op:
        case token_logical_op:
        case token_type_specifier:
        case token_keywords:
                return true;
        default:
                return false;
    }
}

std::ostream& 
operator<<(std::ostream& os, Token t){

    os << '<';
    
    if(!t.has_attribute()) return os << display(t.get_name()) << '>';
    
    os << display(t.get_name()) << ": ";

    switch(t.get_name()){
        case token_hex_int:
        case token_binary_int:
        case token_decimal_int:
            os << display(t.get_intval_attr().value);
            break;
        case token_floating_point_literal:
            os << display(t.get_float_attr());
            break;
        case token_boolean_literal:
            os << display(t.get_bool_attr());
            break;
        case token_character_literal:
            os << display(t.get_ch_attr());
            break;
        case token_string_literal:
            os << display(t.get_str_attr());
            break;
        case token_identifier:
            os << display(t.get_sym_attr());
            break;
        case token_relational_op:
            os << display(t.get_relop_attr());
            break;
        case token_arithmatic_op:
            os << display(t.get_arthop_attr());
            break;
        case token_bitwise_op:
            os << display(t.get_bit_attr());
            break;
        case token_logical_op:
            os << display(t.get_log_attr());
            break;
        case token_type_specifier:
            os << display(t.get_ts_attr());
            break;
        case token_keywords:
            os << display(t.get_key_attr());
            break;
        default:
            throw std::runtime_error("Invalid token-type for get_attr() call");
    }

    os << '>';

    return os;
}