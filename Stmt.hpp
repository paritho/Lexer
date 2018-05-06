#pragma once

#include <vector>
#include "Scope.hpp"

using Stmt_List = std::vector<Stmt*>;

struct Stmt {
    enum stmt_kind {
        block_kind,
        if_kind,
        if_else_kind,
        while_kind,
        decl_kind,
        expr_kind,
        breaking_kind
    };

    enum breaking_stmt {
        break_stmt,
        continue_stmt,
        return_stmt
    };


    Stmt(stmt_kind k)
        : kind(k)
    {}

    virtual ~Stmt() = default;  

    stmt_kind get_kind() { return kind; }

    private:
    stmt_kind kind;
};

struct Block_Stmt : Stmt {
    Block_Stmt(Stmt_List& sl)
        : Stmt(block_kind),
          stmts(sl)
    {}

    Stmt_List get_stmts() { return stmts;}

    Stmt_List stmts;
};

// if(expr) stmt
struct If_Stmt : Stmt {
    If_Stmt(Expr* test, Stmt* stmt)
        : Stmt(if_kind),
          test(test),
          tbranch(stmt)
    {}

    Expr* get_test() { return test;}
    Stmt* get_tbranch() {return tbranch; }

    Expr* test;
    Stmt* tbranch;
};

// if(expr) stmt else stmt
struct If_Else_Stmt : Stmt {
    If_Else_Stmt(Expr* test, Stmt* tbranch, Stmt* fbranch)
        : Stmt(if_else_kind),
          test(test),
          tbranch(tbranch),
          fbranch(fbranch)
    {}

    Expr* get_test() { return test; }
    Stmt* get_tbranch() { return tbranch; }
    Stmt* get_fbranch() { return fbranch; }

    Expr* test;
    Stmt* tbranch;
    Stmt* fbranch;
};

struct While_Stmt : Stmt {
    While_Stmt(Expr* test, Stmt* stmt)
        : Stmt(while_kind),
          test(test),
          body(stmt)
    {}

    Expr* get_test() { return test; }
    Stmt* get_body() { return body; }

    Expr* test;
    Stmt* body;
};

// for break; continue; and return;
struct Breaking_Stmt : Stmt {
    Breaking_Stmt(breaking_stmt bk)
        : Stmt(breaking_kind),
          kind(bk)
    {}

    breaking_stmt get_kind_of_breaking() { return kind; }

    breaking_stmt kind;
};

struct Decl_Stmt : Stmt {
    Decl_Stmt(Decl* d)
        : Stmt(decl_kind),
          decl(d)
    {}

    Decl* get_decl_stmt() { return decl; }

    Decl* decl;
};

struct Expr_Stmt : Stmt {
    Expr_Stmt(Expr* e)
        : Stmt(expr_kind),
          expr(e)
    {}

    Expr* get_expr_stmt(){ return expr; }

    Expr* expr;
};