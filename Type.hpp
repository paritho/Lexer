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
    type_ptr,
    type_ref,

    // note: there will be no operations on these types:
    id_type, 
    arith_type
};

struct Types {
    Types(type_name tn)
        : name(tn)
    {}

    virtual ~Types() = default;

    type_name get_name() { return name; }

    private:
    type_name name;
};

struct Void_Type : Types {
    Void_Type()
        : Types(type_void)
    {}
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
    Ptr_Type(Types* t)
        : Types(type_ptr),
          el(t)
    {}

    Types* get_el_type() { return el; }

    Types* el;
};

struct Ref_Type : Types {
    Ref_Type(Types* t)
        : Types(type_ref),
          el(t)
    {}

    Types* get_obj_type(){ return el; }

    Types* el;
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
    Func_Type(Type_List& tl, Types* ret_type)
        : Types(type_func),
          params(tl),
          ret_type(ret_type)
    {}

    Type_List& get_params() { return params; }
    Types* get_ret_type(){ return ret_type; }

    Type_List params;
    Types* ret_type;
};