#pragma once

#include <vector>

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

    // note: there will be no operations on these types:
    id_type, 
    arith_type
};

struct Type {
    Type() = default;
    Type(type_name tn)
        : name(tn)
    {}

    virtual ~Type() = default;

    type_name get_name() { return name; }
    bool is_ref_to(Type* t);
    bool is_prt_to(Type* t);

    private:
    type_name name;
};

using Type_List = std::vector<Type*>;

struct Void_Type : Type {
    Void_Type()
        : Type(type_void)
    {}
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

struct String_Type : Type {
    String_Type()
        : Type(type_string)
    {}
};

struct Id_Type : Type {
    Id_Type()
        : Type(id_type)
    {}
};

struct Ptr_Type : Type {
    Ptr_Type() = default;
    Ptr_Type(Type* t)
        : Type(type_ptr),
          el(t)
    {}

    Type* get_el_type() { return el; }

    Type* el;
};

struct Ref_Type : Type {
    Ref_Type() = default;
    Ref_Type(Type* t)
        : Type(type_ref),
          el(t)
    {}

    Type* get_obj_type(){ return el; }

    Type* el;
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

struct Unary_Type : Type {
    Unary_Type()
        : Type(type_unary)
    {}
};

struct Func_Type : Type {
    Func_Type() = default;
    Func_Type(Type_List& tl, Type* ret_type)
        : Type(type_func),
          params(tl),
          ret_type(ret_type)
    {}

    Type_List& get_params() { return params; }
    Type* get_ret_type(){ return ret_type; }

    Type_List params;
    Type* ret_type;
};