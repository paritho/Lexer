#pragma once

#include <vector>
#include "Scope.hpp"

using Stmt_List = std::vector<Stmt*>;

enum stmt_kind {
    block_kind,
    if_kind,
    if_else_kind,
    while_kind,
    break_kind,
    decl_kind,
    expr_kind,
};

struct Stmt {
    Stmt(){};
    virtual ~Stmt() = default;  

    private:
    Scope scope;
};

struct Block_Stmt : Stmt {
    Block_Stmt()
        : Stmt()
    {}

};

// if(expr) stmt
struct If_Stmt : Stmt {
    If_Stmt()
        : Stmt()
    {}

};

// if(expr) stmt else stmt
struct If_Else_Stmt : Stmt {
    If_Else_Stmt()
        : Stmt()
    {}

};

struct While_Stmt : Stmt {
    While_Stmt()
        : Stmt()
    {}

};

// for break; continue; and return;
struct Breaking_Stmt : Stmt {
    Breaking_Stmt(){}
        : Stmt()
    {}
};

struct Decl_Stmt : Stmt {
    Decl_Stmt()
        : Stmt()
    {}
};

struct Expr_Stmt : Stmt {
    Expr_Stmt()
        : Stmt()
    {}
}