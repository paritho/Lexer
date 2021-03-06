
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
        {symbols.get("as"), Token(key_as)},
        {symbols.get("break"), Token(key_break)},
        {symbols.get("continue"), Token(key_continue)},
        {symbols.get("def"), Token(key_def)},
        {symbols.get("else"),  Token(key_else)},
        {symbols.get("if"),  Token(key_if)},
        {symbols.get("let"),  Token(key_let)},
        {symbols.get("return"), Token(key_return)},
        {symbols.get("var"),  Token(key_var)},
        {symbols.get("while"), Token(key_while)},
        // logic ops
        {symbols.get("and"),  Token(logop_AND)},
        {symbols.get("or"),  Token(logop_OR)},
        {symbols.get("not"),  Token(logop_NOT)},
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
                if(peek() == '=') return lex_relop(2, op_equals);
                return lex_assignment();

            // arithmatic ops
            case '+':
                return lex_arthop(op_plus);
            case '-':
                if(peek() == '>') return lex_arrow_op();
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

            default: {
                if(std::isdigit(*current)) return lex_number();
                // is_basic_character() tests for digits, but 
                // digits will have been found in the preceeding if
                if(is_basic_character(*current)) return lex_word();
                                
                // if we get here is an invalid char
                std::stringstream ss;
                ss << "invalid char" << *current;
                throw std::runtime_error(ss.str());
            }
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
Lexer::lex_arrow_op(){
    accept(2);
    return {token_arrow, current_location};
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

    assert(is_basic_character(*current));
    const char* start = current;
    // accept first char
    accept();
    // accept subsequent char if alphanumeric
    while(!eof() && is_basic_character(*current)) accept();

    // build word
    std::string str(start, current);
    
    // add it to the symbol table
    symbol sym = symbols.get(str);
    // look for reserved word
    auto iter = reserved_words.find(sym);
       
    if(iter != reserved_words.end()) {
        Token& token = iter->second;
        return {token.get_name(), token.get_attr(), token_location};
    }

    // return the token if not a reserved word
    return {sym, token_location};

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

    // if next char is e, its an exponent
    if(*current == 'e' || *current == 'E'){
        accept();
        while(!eof() && isdigit(*current)) accept();

        // NOTE: should this be something other than a string?
        std::string st(start, current);
        return {symbols.get(st), token_floating_point_literal, token_location};
    }

    // determine if its a whole or fractional number
    if(*current != '.') {
        std::string number(start, current);
        return {decimal, std::stoll(number), token_location};
    }

    // fractional bit
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
        c = get_escape_sequence();
        // advance beyond escaped chars
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
    assert(*current == '"');
    // move past "
    accept();

    if(eof()) throw std::runtime_error("Unterminated string");

    std::string sret;

    while(*current != '"') {
        if(*current == '\n') throw std::runtime_error("Multi-line strings not allowed");

        // assume its a normal char
        char c = *current;

        if(*current == '\\') {
            c = get_escape_sequence();
        }
                
        sret += c;
        accept();
    }
    // move past final "
    accept();
    
    return {symbols.get(sret), token_string_literal, token_location};
}

char
Lexer::get_escape_sequence(){
    assert(*current == '\\');
        switch(peek()){
            case '\'':
            case '\"':
            case '\\':
                return peek();
            case 'a':
                return '\a';
            case 'b':
                return '\b';
            case 'f':
                return '\f';
            case 'n':
                return '\n';
            case 'r':
                return '\r';
            case 't':
                return '\t';
            case 'v':
                return '\v';
            default:
                throw std::runtime_error("Invalid escape sequence");
        }
}

// this should be called from skip_comment ONLY
// a comment line terminates on \n character
bool 
Lexer::is_comment_character(char c){
    return c != '\n';
}

bool
Lexer::is_basic_character(char c){
    return c == '_' || std::isalnum(c);
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
    while(n > 0){
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
    while(!eof() && is_comment_character(*current)) ignore();
}
