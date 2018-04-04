#pragma once

#include <vector>

using Expr_List = std::vector<Expr*>;

enum kind {
    int_kind,
    bool_kind,
    float_kind,
    arith_kind,
    // other kinds
};

struct Expr {
    Expr(kind k)
        :kind(k)
    {}

    virtual ~Expr() = default;

    kind get_kind() const { return kind; }



    private:
    kind kind;
};

struct Int_Expr : Expr {
    Int_Expr(int n)
        : Expr(int_kind), val(n)
    {}

    int val;
};

struct Bool_Expr : Expr {
    Bool_Expr(bool b)
        : Expr(bool_kind), val(b)
    {}

    bool val;
};