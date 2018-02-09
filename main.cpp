#include <iostream>
#include <fstream>
#include <iterator>
#include "Lexer.hpp"


int
main(int argc, char** argv)
{
    if(argc <= 1) throw std::runtime_error("No file provided");

    // throw and error if the file can't be opened or doesn't exist
    std::ifstream input(argv[1]);
    if(!input.good()) throw std::runtime_error("Unable to open file");

    // from a SO question
    std::string file_contents( (std::istreambuf_iterator<char>(input) ),
                               (std::istreambuf_iterator<char>()) );
    

    Symbol_table stable;
    Lexer lex(stable, argv[1]);

    while(Token token = lex())
        std::cout << token << '\n';

}