#pragma once

#include <vector>

using Expr_List = std::vector<Expr*>;

enum kind {
    int_kind,
    bool_kind,
    float_kind,
    char_kind,
    id_kind,
    string_kind,
    ptr_kind,
    ref_kind,
    arith_kind,
    post_kind,
    arg_kind,
    unary_kind,
    cast_kind,
    shift_kind,
    rel_kind,
    bit_kind,
    log_kind,
    cond_kind,
    ass_kind,
    const_kind,
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

struct Float_Expr : Expr {
    Float_Expr(double n)
        : Expr(float_kind), val(n)
    {}

    double val;
};

struct Char_Expr : Expr {
    Char_Expr(char c)
        : Expr(char_kind), val(c)
    {}

    char val;
};

struct Id_Expr : Expr {
    Id_Expr(symbol s)
        : Expr(id_kind), val(s)
    {}

    symbol val;
};

struct String_Expr : Expr {
    String_Expr(std::string s)
        : Expr(string_kind), val(s)
    {}

    std::string val;
};

struct Ptr_Expr : Expr {
    Ptr_Expr(int& n)
        : Expr(ptr_kind), val(n)
    {}

    int& val;
};

struct Ref_Expr : Expr {
    Ref_Expr(int& n)
        : Expr(ref_kind), val(n)
    {}

    int& val;
};

// template for any expr that takes two operands
// add, mult, bin, log
template <typename T>
struct Bin_Expr : Expr {
    Bin_Expr(kind k, T op, Expr* e1, Expr* e2)
        : Expr(k),
          op(op),
          e1(e1),
          e2(e2)
    {}

    T op;
    Expr* e1;
    Expr* e2;
};

struct Arith_Expr : Bin_Expr<arithmatic_operators> {
    Arith_Expr(arithmatic_operators t, Expr* e1, Expr* e2)
        : Bin_Expr<arithmatic_operators>(arith_kind, t, e1, e2)
    {}

};

struct Log_Expr : Bin_Expr<logical_operators> {
    Log_Expr(logical_operators t, Expr* e1, Expr* e2)
        : Bin_Expr<logical_operators>(log_kind, t, e1, e2)
    {}

};

