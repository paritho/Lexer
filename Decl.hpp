#pragma once

#include <vector>
#include <queue>

using Decl_List = std::vector<Decl*>;

struct Decl{

};

struct Local_Decl : Decl {

};

struct Obj_Decl : Decl {

};

struct Var_Decl : Decl {

};

struct Const_Decl : Decl {

};

struct Value_Decl : Decl {

};

struct Function_Decl : Decl {

};

struct Decl_Seq {
    Decl_Seq(Decl* decl){ };

    void add_seq(Decl* d){ return sequence.emplace(d); }
    Decl* get_next_decl(){ return sequence.pop(); }

    private:
    std::queue<Decl*> sequence;
};