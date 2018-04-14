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

    Decl* lookup_name(const char* str) const {
        auto iter = symbols.get(str)
        // TODO: fix. iter->second returns a token
        return iter == end() ? nullptr : iter->second();
    }

    void declare(const char* str){
        symbols.get(str);
    }

    Scope* get_enclosing_scope() { return enclosing; }

    private:
    Scope* enclosing;
    Symbol_table symbols;

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