#pragma once

#include <vector>

using Type_List = std::vector<Types*>;


enum type_name{
    type_void,
    type_bool,
    type_int,
    type_float,
    type_char,
    type_string,
    type_ref,
    type_post,
    type_unary,
    type_func,

    // note: there will be no operations on these types:
    id_type, 
    arith_type
};

struct Types {
    Types(type_name tn)
        : name(tn)
    {}

    virtual ~Types() = default;

    private:
    type_name name;
};

struct Int_Type : Types {
    Int_Type()
        : Types(type_int)
    {}
};
struct Bool_Type : Types {
    Bool_Type()
        : Types(type_bool)
    {}
};
struct Float_Type : Types {
    Float_Type()
        : Types(type_float)
    {}
};
struct Char_Type : Types {
    Char_Type()
        : Types(type_char)
    {}
};
struct String_Type : Types {
    String_Type()
        : Types(type_string)
    {}
};
struct Id_Type : Types {
    Id_Type()
        : Types(id_type)
    {}
};
struct Ptr_Type : Types {
    Ptr_Type()
        : Types(type_int)
    {}
};
struct Ref_Type : Types {
    Ref_Type()
        : Types(type_ref)
    {}
};
struct Arith_Type : Types {
    Arith_Type()
        : Types(arith_type)
    {}
};

struct Post_Type : Types {
    Post_Type()
        : Types(type_post)
    {}
};

struct Unary_Type : Types {
    Unary_Type()
        : Types(type_unary)
    {}
};

struct Func_Type : Types {
    Func_Type()
        : Types(type_func)
    {}
};