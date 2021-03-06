#pragma once

#include <vector>
#include "AST.hpp"

class Type;
class Decl;
class Stmt;

struct Expr {
    enum expr_kind {
        conversion,
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
        func_kind,
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

        binary_kind,
        conv_kind,
        call_kind,

    };

    enum unop {
        uop_increment,
        uop_decrement,
        uop_log_not,
        uop_ref,
        uop_ptr,
        uop_compliment
    };

    enum binop{
        add_op,
        sub_op,
        mul_op,
        mod_op,
        rem_op,
        bin_and_op,
        bin_or_op,
        bin_xor_op,
        lshft_op,
        rshft_op,
        log_add_op,
        log_or_op,
        equal_op,
        noteq_op,
        gteq_op,
        lteq_op,
        gt_op,
        lt_op
    };

    enum conv{
        // if no conversion ie, entity
        none,
        value,
        ctrunc,
        // convert to ...
        ctbool,
        ctchar,
        ctint,
        ctfloat,
    };
    
    Expr() = default;
    Expr(expr_kind k, Type* t)
        : kind(k),
          type(t)
    {}

    virtual ~Expr() = default;

    expr_kind get_kind() const { return kind; }
    Type* get_type() { return type; }

    private:
    expr_kind kind;
    Type* type;
};

using Expr_List = std::vector<Expr*>;

struct Int_Expr : Expr {
    Int_Expr() = default;
    Int_Expr(long long n)
        : Expr(int_kind, new Int_Type()), 
          val(n)
    {}

    long long get_value() { return val; }

    long long val;
};

struct Bool_Expr : Expr {
    Bool_Expr() = default;
    Bool_Expr(bool b)
        : Expr(bool_kind, new Bool_Type()), 
          val(b)
    {}

    bool get_value() { return val; }

    bool val;
};

struct Float_Expr : Expr {
    Float_Expr() = default;
    Float_Expr(double n)
        : Expr(float_kind, new Float_Type()), 
          val(n)
    {}

    double get_value() { return val; }

    double val;
};

struct Char_Expr : Expr {
    Char_Expr() = default;
    Char_Expr(char c)
        : Expr(char_kind, new Char_Type()), val(c)
    {}

    char get_value() { return val; }

    char val;
};

struct Id_Expr : Expr {
    Id_Expr() = default;
    Id_Expr(Decl* d)
        : Expr(id_kind, new Id_Type()), 
          val(d)
    {}

    Decl* get_decl() { return val; }

    Decl* val;
};

struct String_Expr : Expr {
    String_Expr() = default;
    String_Expr(symbol s)
        : Expr(string_kind, new String_Type()), val(s)
    {}

    symbol get_value() { return val; }

    symbol val;
};

struct Ptr_Expr : Expr {
    Ptr_Expr() = default;
    Ptr_Expr(int& n)
        : Expr(ptr_kind, new Int_Type()), val(n)
    {}

    int get_value() { return val; }

    int& val;
};

struct Ref_Expr : Expr {
    Ref_Expr() = default;
    Ref_Expr(int& n)
        : Expr(ref_kind, new Ref_Type()), val(n)
    {}

    int get_value() { return val; }

    int& val;
};

struct Cast_Expr : Expr {
    Cast_Expr() = default;
    Cast_Expr(Expr* e, Type* t)
        : Expr(cast_kind, t),
          src(e),
          dest(t)
    {}

    Expr* src;
    Type* dest;
};

struct Assign_Expr : Expr {
    Assign_Expr() = default;
    Assign_Expr(Type* t, Expr* lhs, Expr* rhs)
        : Expr(ass_kind, t),
          lhs(lhs),
          rhs(rhs)
    {}

    Expr* get_lhs() { return lhs; }
    Expr* get_rhs() { return rhs; }

    Expr* lhs;
    Expr* rhs;
};

struct Conditional_Expr : Expr {
    Conditional_Expr() = default;
    Conditional_Expr(Expr* test, Expr* tbranch, Expr* fbranch)
        : Expr(cond_kind, nullptr),
          test(test),
          tbranch(tbranch),
          fbranch(fbranch)
    {}

    Expr* get_test() {return test;}
    Expr* get_true_branch() {return tbranch;}
    Expr* get_false_branch() {return fbranch;}

    Expr* test;
    Expr* tbranch;
    Expr* fbranch;
};

struct Conversion_Expr : Expr {
    Conversion_Expr() = default;
    Conversion_Expr(Expr* src, conv ck, Type* t)
        : Expr(conversion, nullptr),
          src(src),
          expr_kind(ck),
          dest(t)
    {}

    Type* get_dest() { return dest; }
    Expr* get_src() { return src; }

    Expr* src;
    conv expr_kind;
    Type* dest;
};

// add, mult, bin, log, eq, rel, shift
struct Bin_Expr : Expr {
    Bin_Expr() = default;
    Bin_Expr(expr_kind k, binop op, Expr* e1, Expr* e2)
        : Expr(k, new Arith_Type()),
          op(op),
          e1(e1),
          e2(e2)
    {}

    Expr* get_lhs() { return e1; }
    Expr* get_rhs() { return e2; }

    binop op;
    Expr* e1;
    Expr* e2;
};

struct Add_Expr : Bin_Expr {
    Add_Expr() = default;
    Add_Expr(binop op, Expr* e1, Expr* e2)
        : Bin_Expr(arith_kind, op, e1, e2)
    {}
};

struct Log_Expr : Bin_Expr {
    Log_Expr() = default;
    Log_Expr(binop op, Expr* e1, Expr* e2)
        : Bin_Expr(log_kind, op, e1, e2)
    {}
};

struct Unary_Expr : Expr {
    Unary_Expr() = default;
    Unary_Expr(unop op, Type* t, Expr* expr)
        : Expr(unary_kind, t),
          e(expr),
          uop(op),
          t(t)
    {}

    Expr* get_expr() { return e; }

    unop uop;
    Expr* e;
    Type* t;
};

struct Postfix_Expr : Expr {
    Postfix_Expr() = default;
    Postfix_Expr(expr_kind k, Expr* expr, Expr_List& args)
        : Expr(k, new Post_Type()),
          base(expr),
          args(args)
    { }

    Expr* base;
    Expr_List args;
};

struct Call_Expr : Postfix_Expr {
    Call_Expr() = default;
    Call_Expr(Expr* expr, Type* ret, Expr_List& args)
        : Postfix_Expr(call_kind, expr, args),
          e(expr),
          ret(ret),
          args(args)
    {}

    Expr* e;
    Type* ret;
    Expr_List& args;
};

struct Index_Expr : Postfix_Expr {
    Index_Expr() = default;
    Index_Expr(Expr* expr, Expr_List& args)
        : Postfix_Expr(index_kind, expr, args),
          e(expr),
          args(args)
    {}

    Expr* e;
    Expr_List& args;
};
