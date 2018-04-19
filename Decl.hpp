#pragma once

#include <vector>
#include <queue>

using Decl_List = std::vector<Decl*>;

struct Decl{
    Decl(const char* str)
        : name(str)
    {}

    const char* get_name(){ return name; }

    private:
    const char* name;

};

struct Local_Decl : Decl {
    Local_Decl(const char* str)
        : Decl(str)
    {}
};

struct Obj_Decl : Decl {
    Obj_Decl(const char* str) 
        : Decl(str)
    {}
};

struct Var_Decl : Decl {
    Var_Decl(const char* str) 
        : Decl(str)
    {}
};

struct Const_Decl : Decl {
    Const_Decl(const char* str) 
        : Decl(str)
    {}
};

struct Value_Decl : Decl {
    Value_Decl(const char* str) 
        : Decl(str)
    {}
};

struct Function_Decl : Decl {
    Function_Decl(const char* str) 
        : Decl(str)
    {}
};

struct Decl_Seq {
    Decl_Seq(Decl* decl){ };

    void add_seq(Decl* d){ return sequence.emplace(d); }
    Decl* get_next_decl(){ return sequence.pop(); }

    private:
    std::queue<Decl*> sequence;
};