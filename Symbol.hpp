#pragma once
#include <unordered_set>
#include <string>

// a symbol is just a pointer to a string
using symbol = const std::string*;

// collection of symbols. Theses make string compares fast
class Symbol_table
{
    public:
    symbol get(const char* str);
    symbol get(const std::string& str);

    private:
    std::unordered_set<std::string> symbols;
}; 

// returns a unique symbol for the spelling of str
inline symbol
Symbol_table::get(const char* str){
    return &*symbols.insert(str).first;
}

inline symbol
Symbol_table::get(const std::string& str){
    return &*symbols.insert(str).first;
}

