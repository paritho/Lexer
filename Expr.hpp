#pragma once

#include <vector>
#include "Type.hpp"

using Expr_List = std::vector<Expr*>;

enum kind {
    converstion,
    int_kind,
    bool_kind,
    float_kind,
    char_kind,
    id_kind,
    index_kind,
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

enum unop {
    pos,
    neg,
    not,
    address,
    dereference
};

enum binop{
    add,
    sub,
    mul,
    mod,
    rem,
    and,
    or,
    xor,
    lshft,
    rshft,
    logadd,
    logor,
    equal,
    noteq,
    gteq,
    lteq,
    gt,
    lt
};

enum conv_kind{
    // if no converstion ie, entity
    none,
    value,
    ctrunc,
    // convert to ...
    ctbool,
    ctchar,
    ctint,
    ctfloat,
};

struct Expr {
    Expr(kind k, Types* t)
        : kind(k),
          type(t)
    {}

    virtual ~Expr() = default;

    kind get_kind() const { return kind; }
    Types* get_type() { return type; }

    private:
    kind kind;
    Types* type;
};

struct Int_Expr : Expr {
    Int_Expr(long long n)
        : Expr(int_kind, new Int_Type()), 
          val(n)
    {}

    long long val;
};

struct Bool_Expr : Expr {
    Bool_Expr(bool b)
        : Expr(bool_kind, new Bool_Type()), 
          val(b)
    {}

    bool val;
};

struct Float_Expr : Expr {
    Float_Expr(double n)
        : Expr(float_kind, new Float_Type()), 
          val(n)
    {}

    double val;
};

struct Char_Expr : Expr {
    Char_Expr(char c)
        : Expr(char_kind, new Char_Type()), val(c)
    {}

    char val;
};

struct Id_Expr : Expr {
    Id_Expr(Decl* d)
        : Expr(id_kind, new Id_Type()), 
          val(d)
    {}

    Decl* val;
};

struct String_Expr : Expr {
    String_Expr(symbol s)
        : Expr(string_kind, new String_Type()), val(s)
    {}

    symbol val;
};

struct Ptr_Expr : Expr {
    Ptr_Expr(int& n)
        : Expr(ptr_kind, new Int_Type()), val(n)
    {}

    int& val;
};

struct Ref_Expr : Expr {
    Ref_Expr(int& n)
        : Expr(ref_kind, new Ref_Type()), val(n)
    {}

    int& val;
};

struct Cast_Expr : Expr {
    Cast_Expr(Expr* e, Types* t)
        : Expr(cast_kind, t),
          src(e),
          dest(t)
    {}

    Expr* src;
    Types* dest;
};

struct Assign_Expr : Expr {
    Assign_Expr(Types* t, Expr* lhs, Expr* rhs)
        : Expr(ass_kind, t),
          lhs(lhs),
          rhs(rhs)
    {}

    Expr* lhs;
    Expr* rhs;
};

struct Conditional_Expr : Expr {
    Conditional_Expr(Expr* test, Expr* tbranch, Expr* fbranch)
        : Expr(cond_kind, nullptr),
          test(test),
          tbranch(tbranch),
          fbranch(fbranch)
    {}

    Expr* test;
    Expr* tbranch;
    Expr* fbranch;
};

struct Converstion_Expr : Expr {
    Converstion_Expr(Expr* src, conv_kind ck)
        : Expr(converstion, nullptr),
          src(src),
          dest(ck)
    {}

    Expr* src;
    conv_kind dest;
};

// template for any expr that takes two operands
// add, mult, bin, log
template <typename T>
struct Bin_Expr : Expr {
    Bin_Expr(kind k, T op, Expr* e1, Expr* e2)
        : Expr(k, new Arith_Type()),
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

struct Unary_Expr : Expr {
    Unary_Expr(unop op, Expr* expr)
        : Expr(unary_kind, new Unary_Type()),
          e(expr)
    {}

    unop unop;
    Expr* e;
};

struct Postfix_Expr : Expr {
    Postfix_Expr(kind k, Expr* expr, Expr_List& args)
        : Expr(k, new Post_Type()),
          base(expr),
          args(args)
    { }

    Expr* base;
    Expr_List args;
};

struct Call_Expr : Postfix_Expr {
    Call_Expr(Expr* expr, Expr_List& args)
        : Postfix_Expr(call_kind, expr, args)
    {}
};

struct Index_Expr : Postfix_Expr {
    Index_Expr(Expr* expr, Expr_List& args)
        : Postfix_Expr(index_kind, expr, args)
    {}
};