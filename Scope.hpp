#pragma once;

#include <vector>
#include "Decl.hpp"
#include "Symbol.hpp"

struct Scope {
    Scope(Symbol_table& syms, Scope* enc = nullptr)
        : enclosing(enc),
          symbols(syms)
    {}

    virtual ~Scope() = default;

    symbol lookup_name(const char* str) const {
        return symbols.find(str);
    }

    void declare(const char* str){
        // .get will insert the symbol
        symbols.get(str);
    }

    Scope* get_enclosing_scope() { return enclosing; }

    private:
    Scope* enclosing;
    Symbol_table& symbols;

};

struct Global_Scope : Scope {
    using Scope::Scope;
};

struct Block_Scope : Scope {
    using Scope::Scope;
};

struct Param_Scope : Scope {
    using Scope::Scope;
};