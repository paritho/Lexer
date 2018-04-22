
#include "Type.hpp"

static bool is_same_type(Types* t1, Types* t2){
    if(t1 == t2) return true;

    if(t1->get_name() != t2->get_name()) return false;

    // if we get here, the types have the same name
    switch(t1->get_name()){
        case type_bool:
        case type_int:
        case type_float:
        case type_char:
        case type_string:
        case type_unary:
        case type_post:
        case id_type:
        case arith_type:
            return true;
        case type_func:
            return is_same_func(t1, t2);
        case type_ptr:
            return t1->get_el_type() == t2->get_el_type();
        case type_ref:
            return t1->get_obj_type() == t2->get_obj_type();
    };
}

// function type is the same if the params types and return types
// are all the same
static bool is_same_func(Types* t1, Types* t2){
    Type_List& t1params = t1->get_params();
    Type_List& t2params = t2->get_params();

    Types* t1ret = t1->get_ret_type();
    Types* t2ret = t2->get_ret_type();

    // comparitor function for use in std::equals
    auto compare = [](Types* a, Types* b){
        return is_same_type(a,b);
    };

    bool eq_params = std::equal(t1params.begin(), t1params.end(), 
                                t2params.begin(), t2params.end(),
                                compare);
    
    return eq_params && (t1ret == t2ret);
}