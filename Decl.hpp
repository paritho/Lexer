#pragma once

#include <vector>
#include <queue>
#include "AST.hpp"

using Decl_List = std::vector<Decl*>;

enum kind {
    var,
    let,
    value,
    program,
    param,
    function
};

struct Decl{
    Decl(const char* str, kind k, Types* t = nullptr)
        : name(str),
          kind(k)
    {}

    virtual ~Decl() = default;

    const char* get_name(){ return name; }
    kind get_kind() { return kind; }

    private:
    const char* name;
    kind kind;
    Types* t;

};

struct Function_Decl : Decl {
    Function_Decl(const char* str, Types* t, Decl_List& dl, Stmt* s)
        : Decl(str, function, t),
          params(dl),
          body(s)
    {}

    Decl_List& get_params() { return params; }
    Func_Type* get_type();
    Types* get_return_type();
    Stmt* get_body() { return body; }

    private:
    Decl_List params;
    Stmt* body;
};

struct Prog_Decl : Decl {
    Prog_Decl(Decl_List& dl)
        : Decl(nullptr, program, nullptr),
          decls(dl)
    {}

    Decl_List& get_decls(){ return decls; }

    private:
    Decl_List decls;
}

struct Obj_Decl : Decl {
    Obj_Decl(const char* str, kind k, Type* t, Expr* e) 
        : Decl(str, k, t),
          t(t),
          e(e)
    {}

    Expr* get_initialized() { return e;}
    Type* get_type() { return t;}

    private:
    Type* t;
    Expr* e;
};

struct Var_Decl : Obj_Decl {
    Var_Decl(const char* str, Type* t, Expr* e = nullptr) 
        : Obj_Decl(str, var, t, e)
    {}
};

struct Const_Decl : Obj_Decl {
    Const_Decl(const char* str, Type* t, Expr* e = nullptr) 
        : Obj_Decl(str, let, t, e)
    {}
};

struct Value_Decl : Obj_Decl {
    Value_Decl(const char* str, kind k, Types* t, Expr* e) 
        : Decl(str, value, t,e)
    {}
};

struct Param_Decl : Obj_Decl{
    Param_Decl(const char* str, Types* t)
        : Obj_Decl(str, param, t, nullptr)
    { };

    void add_seq(Decl* d){ return sequence.emplace(d); }
    Decl* get_next_decl(){ return sequence.pop(); }

    private:
    std::queue<Decl*> sequence;
};
