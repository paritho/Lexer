#pragma once

#include <vector>

using Type_List = std::vector<Type*>;


enum type_name{
    type_void,
    type_bool,
    type_int,
    type_float,
    type_char,
    type_ref,
    type_post,

    // note: there will be no operations on these types:
    id_type, 
    arith_type
};

struct Type {
    Type(type_name tn)
        : name(tn)
    {}

    virtual ~Type() = default;

    private:
    type_name name;
};

struct Int_Type : Type {
    Int_Type()
        : Type(type_int)
    {}
};
struct Bool_Type : Type {
    Bool_Type()
        : Type(type_bool)
    {}
};
struct Float_Type : Type {
    Float_Type()
        : Type(type_float)
    {}
};
struct Char_Type : Type {
    Char_Type()
        : Type(type_char)
    {}
};
struct Id_Type : Type {
    Id_Type()
        : Type(id_type)
    {}
};
struct Ptr_Type : Type {
    Ptr_Type()
        : Type(type_int)
    {}
};
struct Ref_Type : Type {
    Ref_Type()
        : Type(type_ref)
    {}
};
struct Arith_Type : Type {
    Arith_Type()
        : Type(arith_type)
    {}
};

struct Post_Type : Type {
    Post_Type()
        : Type(type_post)
    {}
};

struct Func_Type : Type {
    Func_Type()
        : Type()
    {}
};