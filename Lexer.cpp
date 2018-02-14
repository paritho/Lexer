#include <utility>
#include "Lexer.hpp"

// construct the lexer
Lexer::Lexer(Symbol_table& symbol, const std::string& input)
    : symbols(symbol),
      current(input.data()), 
      end(current + input.size()),
      consumed("")
{ 
    // reserve language words
    reserved_words.insert({
        //keywords
        {symbols.get("def"), Token(key_def)},
        {symbols.get("else"),  Token(key_else)},
        {symbols.get("if"),  Token(key_if)},
        {symbols.get("let"),  Token(key_let)},
        {symbols.get("var"),  Token(key_var)},
        // logic ops
        {symbols.get("and"),  Token(op_AND)},
        {symbols.get("or"),  Token(op_OR)},
        {symbols.get("not"),  Token(op_NOT)},
        // boolean literal
        {symbols.get("true"),  Token(true)},
        {symbols.get("false"),  Token(false)},
        // type specifiers
        {symbols.get("bool"),  Token(ts_bool)},
        {symbols.get("char"),  Token(ts_char)},
        {symbols.get("int"),  Token(ts_int)},
        {symbols.get("float"),  Token(ts_float)},
        {symbols.get("void"),  Token(ts_void)},
    });
    
}

Token 
Lexer::scan(){

    assert(!eof());
    while(!eof()){

        token_location = current_location;

        switch(*current){
            // spaces and comments
            case ' ':
            case '\t':
                skip_space();
                continue;
            case '\n':
                skip_newline();
                continue;
            case '#':
                skip_comment();
                continue;

            // punctuators                        
            case '{':
                return lex_punctuator(token_left_bracket);
            case '}':
                return lex_punctuator(token_right_bracket);
            case '[':
                return lex_punctuator(token_left_brace);
            case ']':
                return lex_punctuator(token_right_brace);
            case '(':
                return lex_punctuator(token_left_paren);
            case ')':
                return lex_punctuator(token_right_paren);
            case ',':
                return lex_punctuator(token_comma);
            case ';':
                return lex_punctuator(token_semicolon);
            case ':':
                return lex_punctuator(token_colon);

            // relational ops & some bitops
            case '<':
                if(peek() == '<') return lex_bitop(2, op_leftshift);
                if(peek() == '=') return lex_relop(2, op_ltequals);
                return lex_relop(1, op_lessthan);
            case '>':
                if(peek() == '>') return lex_bitop(2, op_rightshift);
                if(peek() == '=') return lex_relop(2, op_gtequals);
                return  lex_relop(1, op_greaterthan);
            case '!':
                if(peek() == '=') return lex_relop(2, op_notequal);

            // conditional and assignment
            // NOTE: == is a relational op that will be determined here
            case '?':
                return lex_conditional();
            case '=':
                if(peek() != '=') return lex_assignment();
                return lex_relop(2, op_equals);

            // arithmatic ops
            case '+':
                return lex_arthop(op_plus);
            case '-':
                return lex_arthop(op_minus);
            case '*':
                return lex_arthop(op_mult);
            case '/':
                return lex_arthop(op_divide);
            case '%':
                return lex_arthop(op_mod);

            // bitwise ops
            case '&':
                return lex_bitop(1, op_and);
            case '|':
                return lex_bitop(1, op_or);
            case '~':
                return lex_bitop(1, op_not);
            case '^':
                return lex_bitop(1, op_xor);
        
            // logic ops handled in lex_word

            // characters and strings
            case '\'':
                return lex_character();
            case '"':
                return lex_string();

            default:
                if(!std::isdigit(*current)) return lex_word();
                if(std::isdigit(*current)) return lex_number();
                
                // if we get here is an invalid char
                std::stringstream ss;
                ss << "invalid char" << *current;
                throw std::runtime_error(ss.str());

        }
    }
    // returns the constructor that gives eof token
    return {};
}

Token
Lexer::lex_punctuator(token_name token){
    accept();
    return {token, token_location};
}

Token
Lexer::lex_relop(int len, relational_operators op){

    accept(len);
    return {op, token_location};
}

Token 
Lexer::lex_arthop(arithmatic_operators op){

    accept();
    return {op, token_location};
}

Token
Lexer::lex_bitop(int len, bitwise_operators op){

    accept(len);
    return {op, token_location};
}

Token 
Lexer::lex_logop(int len, logical_operators op){

    accept(len);
    return {op, token_location};
}

Token
Lexer::lex_conditional(){
    accept();
    return {token_conditional_op, current_location};
}

Token
Lexer::lex_assignment(){
    accept();
    return {token_assignment_op, current_location};
}

Token 
Lexer::lex_word(){

    assert(std::isalpha(*current));
    const char* start = current;
    // accept first char
    accept();
    // accept subsequent char if alphanumeric
    while(!eof() && std::isalnum(*current)) accept();

    // build word
    std::string str(start, current);
    // add it to the symbol table
    symbol sym = symbols.get(str);
    // look for reserved word
    auto iter = reserved_words.find(sym);
    if(iter != reserved_words.end()) return {iter->second, token_location};

    // return the token if not a reserved word
    return {str, token_location};

}

Token
Lexer::lex_number(){
    assert(std::isdigit(*current));
    const char* start = current;

    if(*current == '0'){
        switch(peek()){
            case 'b':
            case 'B':
                return lex_binary_int();
            case 'x':
            case 'X':
                return lex_hexidecimal_int();
            default:
                break;
        }
    }

    // if we get here, it is a decimal number
    accept();
    while(!eof() && isdigit(*current)) accept();

    if(peek() != '.') {
        std::string number(start, current);
        return {decimal, std::stoll(number), token_location};
    }

    // if next char is e, its an exponent
    if(peek() == 'e' || peek() == 'E'){
        accept();
        while(!eof() && isdigit(*current)) accept();
        // FIX: What should this token be?
        return {std::string(start, current), token_location};
    }

    // otherwise, this is now a floating point number
    accept(); // move past the '.'
    while(!eof() && isdigit(*current)) accept();

    std::string fpnum(start, current);
    return {std::stod(fpnum), token_location};
}

Token
Lexer::lex_binary_int(){
    // we don't need the prefix 0b
    accept(2);
    const char* start = current;
    while(!eof() && (*current == '0' || *current == '1')) accept();
    std::string binnum(start,current);
    return {binary, std::stoll(binnum), token_location};
}

Token
Lexer::lex_hexidecimal_int(){
    // we don't need the prefix 0x
    accept(2);
    const char* start = current;
    while(!eof() && std::isxdigit(*current)) accept();
    std::string hexnum(start,current);
    return {hexidecimal, std::stoll(hexnum), token_location};
}

Token
Lexer::lex_character(){
    assert(*current == '\'');

    // advance beyond opening single quote 
    accept();
    if(eof()) throw std::runtime_error("Unterminated character literal");
    
    // is the character well formed?
    if(*current == '\n') throw std::runtime_error("Invalid mult-line character");
    if(*current == '\'') throw std::runtime_error("character literal cannot be empty");

    char c;
    // if the current char is not a \ it's a basic character
    // NOTE: this may be any character, digit except newline
    // and single quote
    if(*current != '\\'){
        c = *current;
        accept();
    }
    // is the character an escape sequence?
    else{
        assert(*current == '\\');
        switch(peek()){
            case '\'':
            case '\"':
            case '\\':
                c = peek();
            case 'a':
                c = '\a';
            case 'b':
                c = '\b';
            case 'f':
                c = '\f';
            case 'n':
                c = '\n';
            case 'r':
                c = '\r';
            case 't':
                c = '\t';
            case 'v':
                c = '\v';
            default:
                throw std::runtime_error("Invalid escape sequence");
        }
        // advance beyond escape sequence
        accept(2);
    }

    if(*current != '\'') throw std::runtime_error("Unterminated character literal");
    // move beyond closing single quote
    accept();

    // c should hold either a character or escape sequence
    return {c, token_location};
}

Token
Lexer::lex_string(){
    return {};
}

// this should be called from skip_comment ONLY
// a comment line terminates on \n character
bool 
Lexer::is_comment_character(char c){
    return c != '\n';
}

void
Lexer::accept(){
    assert(*current != '\n');
    // NOTE: this does not advance token_location
    current_location.next_column();
    char c = *current;
    consumed += c;
    ++current;
}

// to accept more than one char at a time
void 
Lexer::accept(int n){
    while(n < 0){
        accept();
        --n;
    }
}

void 
Lexer::ignore(){
    current_location.next_column();
    ++current;
}

void 
Lexer::skip_space(){ 
    assert(std::isspace(*current));
    while(!eof() && std::isspace(*current)) ignore(); 
}

void 
Lexer::skip_newline(){
    assert(*current == '\n');
    ignore();
    current_location.next_line();
    current_location.set_column(0);
}

void 
Lexer::skip_comment(){
    assert(*current == '#');
    while(is_comment_character(*current)) ignore();
}
