#include "codegen.hpp"
#include "Type.hpp"
#include "Expr.hpp"
#include "Decl.hpp"
#include "Stmt.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>

/// Associates declarations with values.
using variable_map = std::unordered_map<const Decl*, llvm::Value*>;

// -------------------------------------------------------------------------- //
// Root context

/// The root code generation context. This provides facilities for 
/// translating Beaker types and constants into LLVM types and constants.
struct cg_context
{
  cg_context()
    : ll(new llvm::LLVMContext())
  { }

  /// Destroys the codegen context.
  ~cg_context() { delete ll; }

  /// Returns the LLVM context.
  llvm::LLVMContext *get_context() const { return ll; }

  // Names

  /// Returns a name for the declaration.
  std::string get_name(const Decl* d);

  // Types

  /// Generate the corresponding Type for `t`.
  llvm::Type* get_type(const Type* t);
  llvm::Type* get_bool_type(const Bool_Type* t);
  llvm::Type* get_char_type(const Char_Type* t);
  llvm::Type* get_int_type(const Int_Type* t);
  llvm::Type* get_float_type(const Float_Type* t);
  llvm::Type* get_ref_type(const Ref_Type* t);
  llvm::Type* get_str_type(const String_Type* t);
  llvm::Type* get_ptr_type(const Ptr_Type* t);
  llvm::Type* get_postfix_type(const Post_Type* t);
  llvm::Type* get_unary_type(const Unary_Type* t);
  llvm::Type* get_fn_type(const Func_Type* t);

  /// Returns the corresponding Type for the declaration `d`.
  llvm::Type* get_type(const Obj_Decl* d);

  /// The underlying LLVM context.
  llvm::LLVMContext* ll;
};

// -------------------------------------------------------------------------- //
// Module context

/// The module generation context provides facilities for generating 
/// and referencing module-level declarations.
struct cg_module
{
  cg_module(cg_context& cxt, const Prog_Decl* prog);

  /// Returns the LLVM context.
  llvm::LLVMContext* get_context() const { return parent->get_context(); }

  /// Returns the underlying LLVM module.
  llvm::Module* get_module() const { return mod; }

  //  Names

  /// Generates a declaration name for `d`.
  std::string get_name(const Decl* d) { return parent->get_name(d); }

  // Types

  /// Generate a corresponding Type to `t`.
  llvm::Type* get_type(const Type* t) { return parent->get_type(t); }

  /// Generates a Type corresponding to the Type `d`.
  llvm::Type* get_type(const Obj_Decl* d) { return parent->get_type(d); }

  // Global values

  /// Associate the value `v` with the global declaration `d`.
  void declare(const Decl* d, llvm::GlobalValue* v);

  /// Returns the global value corresponding to `d` or nullptr.
  llvm::GlobalValue* lookup(const Decl* d) const;

  // Declaration generation

  /// Process expressions as top-level declarations.
  void generate();
  void generate(const Decl* d);
  void generate_var_decl(const Var_Decl* d);
  void generate_fn_decl(const Function_Decl* d);

  /// The parent context.
  cg_context* parent; 

  /// The corresponding translation unit.
  const Prog_Decl* prog;  
  
  /// The underlying LLVM module.
  llvm::Module* mod; 

  /// A lookup table for global modules.
  variable_map globals;
};

// -------------------------------------------------------------------------- //
// Function context

/// Provides the codegen context for expressions.
struct cg_function
{
  cg_function(cg_module& m, const Function_Decl* d);

  // Context

  /// Returns the LLVM context.
  llvm::LLVMContext* get_context() const { return parent->get_context(); }

  /// Returns the owning LLVM module.
  llvm::Module* get_module() const { return parent->get_module(); }

  /// Returns the underlying LLVM Function.
  llvm::Function* get_function() const { return fn; }

  // Names

  /// Returns the name for the declaration `d`.
  std::string get_name(const Decl* d) { return parent->get_name(d); }

  // Types

  /// Generates the Type corresponding to the expression `e`.
  llvm::Type* get_type(const Type* t) { return parent->get_type(t); }

  /// Generates the Type corresponding to the expression `e`.
  llvm::Type* get_type(const Expr* e) { return get_type(e->get_type()); }

  /// Generate the corresponding Type for `t`.
  llvm::Type* get_type(const Obj_Decl* t) { return parent->get_type(t); }

  // Local variables

  /// Declare a new local value.
  void declare(const Decl* x, llvm::Value* v);

  /// Lookup a value. This may return a global value.
  llvm::Value* lookup(const Decl* x) const;

  // Function definition

  void define();

  // Block management

  /// Returns the entry block. This is where local variables are allocated.
  llvm::BasicBlock* get_entry_block() const { return entry; }

  /// Returns the current block.
  llvm::BasicBlock* get_current_block() const { return curr; }

  /// Returns a new block with the given name. The block is unlinked until
  /// it is emitted.
  llvm::BasicBlock* make_block(const char* label);

  /// Emits a new block, making it active.
  void emit_block(llvm::BasicBlock* bb);

  // Instruction generation

  /// Generate a list of instructions to compute the value of e.
  llvm::Value* generate_expr(const Expr* e);
  llvm::Value* generate_bool_expr(const Bool_Expr* e);
  llvm::Value* generate_int_expr(const Int_Expr* e);
  llvm::Value* generate_float_expr(const Float_Expr* e);
  llvm::Value* generate_id_expr(const Id_Expr* e);
  llvm::Value* generate_unop_expr(const Unary_Expr* e);
  llvm::Value* generate_arithmetic_expr(const Unary_Expr* e);
  llvm::Value* generate_int_expr(const Unary_Expr* e);
  llvm::Value* generate_float_expr(const Unary_Expr* e);
  llvm::Value* generate_bitwise_expr(const Unary_Expr* e);
  llvm::Value* generate_logical_expr(const Unary_Expr* e);
  llvm::Value* generate_address_expr(const Unary_Expr* e);
  llvm::Value* generate_deref_expr(const Unary_Expr* e);
  llvm::Value* generate_binop_expr(const Bin_Expr* e);
  llvm::Value* generate_arithmetic_expr(const Bin_Expr* e);
  llvm::Value* generate_int_expr(const Bin_Expr* e);
  llvm::Value* generate_float_expr(const Bin_Expr* e);
  llvm::Value* generate_bitwise_expr(const Bin_Expr* e);
  llvm::Value* generate_logical_expr(const Bin_Expr* e);
  llvm::Value* generate_and_expr(const Bin_Expr* e);
  llvm::Value* generate_or_expr(const Bin_Expr* e);
  llvm::Value* generate_relational_expr(const Bin_Expr* e);
  llvm::Value* generate_call_expr(const Call_Expr* e);
  llvm::Value* generate_index_expr(const Index_Expr* e);
  llvm::Value* generate_cast_expr(const Cast_Expr* e);
  llvm::Value* generate_cond_expr(const Conditional_Expr* e);
  llvm::Value* generate_assign_expr(const Assign_Expr* e);
  llvm::Value* generate_conv_expr(const Conversion_Expr* e);

  // Statements
  void generate_stmt(const Stmt* s);
  void generate_block_stmt(const Blo* s);
  // no when in lang?
  //void generate_when_stmt(const when_stmt* s);
  void generate_if_stmt(const If_Stmt* s);
  void generate_if_else_stmt(const If_Else_Stmt* s);
  void generate_while_stmt(const While_Stmt* s);
  void generate_break_stmt(const Breaking_Stmt* s);
  void generate_cont_stmt(const Breaking_Stmt* s);
  void generate_ret_stmt(const Breaking_Stmt* s);
  void generate_decl_stmt(const Decl_Stmt* s);
  void generate_expr_stmt(const Expr_Stmt* s);

  // Local declarations
  void generate_decl(const Decl* d);
  void generate_var_decl(const Var_Decl* d);

  void make_variable(const Var_Decl* d);
  void make_reference(const Var_Decl* d);

  /// The parent module context.
  cg_module* parent;

  /// The function original function
  const Function_Decl* src;
  
  /// The underlying function being defined
  llvm::Function* fn;
  
  /// The entry block.
  llvm::BasicBlock* entry;

  /// The current block.
  llvm::BasicBlock* curr; 
  
  /// Local variables.
  variable_map locals;
};

// -------------------------------------------------------------------------- //
// Context implementation

std::string
cg_context::get_name(const Decl* d)
{
  assert(d->get_name());
  std::stringstream ss;
  ss << *d->get_name();
  return ss.str();
}

/// Generate the corresponding Type.
llvm::Type* 
cg_context::get_type(const Type* t)
{
  // Make sure we're looking at the semantic, not lexical Type.
  switch (t->get_name()) {
  case Type::type_bool:
    return get_bool_type(static_cast<const Bool_Type*>(t));
  case Type::type_char:
    return get_char_type(static_cast<const Char_Type*>(t));
  case Type::type_int:
    return get_int_type(static_cast<const Int_Type*>(t));
  case Type::type_float:
    return get_float_type(static_cast<const Float_Type*>(t));
  case Type::type_ref:
    return get_ref_type(static_cast<const Ref_Type*>(t));
  case Type::type_string:
    return get_str_type(static_cast<const String_Type*>(t));
  case Type::id_type:
    return get_id_type(static_cast<const Id_Type*>(t));
  case Type::type_ptr:
    return get_ptr_type(static_cast<const Ptr_Type*>(t));
  case Type::type_post:
    return get_post_type(static_cast<const Post_Type*>(t));
  case Type::type_unary:
    return get_unary_type(static_cast<const Unary_Type*>(t));
  case Type::type_func:
    return get_fn_type(static_cast<const Func_Type*>(t));
  default:
    throw std::logic_error("invalid Type");
  }
}

/// The corresponding Type is i1.
llvm::Type*
cg_context::get_bool_type(const Bool_Type* t)
{
  return llvm::Type::getInt1Ty(*ll);
}

/// The corresponding Type is i8.
llvm::Type*
cg_context::get_char_type(const Char_Type* t)
{
  return llvm::Type::getInt8Ty(*ll);
}

llvm::Type*
cg_context::get_str_type(const String_Type* t)
{
  return llvm::Type::getInt8Ty(*ll);
}

// assuming post_type would be i8
llvm::Type*
cg_context::get_str_type(const Post_Type* t)
{
  return llvm::Type::getInt8Ty(*ll);
}

// same here. Or would this be i32?
llvm::Type*
cg_context::get_str_type(const Unary_Type* t)
{
  return llvm::Type::getInt8Ty(*ll);
}

/// The corresponding Type is i32.
llvm::Type*
cg_context::get_int_type(const Int_Type* t)
{
  return llvm::Type::getInt32Ty(*ll);
}

/// The corresponding Type is float.
llvm::Type*
cg_context::get_float_type(const Float_Type* t)
{
  return llvm::Type::getFloatTy(*ll);
}

/// Returns a pointer to the object Type.
llvm::Type*
cg_context::get_ref_type(const Ref_Type* t)
{
  llvm::Type* obj = get_type(t->get_obj_type());
  return obj->getPointerTo();
}

llvm::Type*
cg_context::get_ptr_type(const Ptr_Type* t)
{
  llvm::Type* obj = get_type(t->get_el_type());
  return obj->getPointerTo();
}

/// Generate the Type as a pointer. The actual function Type can extracted
/// as needed for creating functions.
llvm::Type* 
cg_context::get_fn_type(const Func_Type* t)
{
  const type_list& ps = t->get_params();
  std::vector<llvm::Type*> parms(ps.size());
  std::transform(ps.begin(), ps.end(), parms.begin(), [this](const Type* p) {
    return get_type(p);
  });
  llvm::Type* ret = get_type(t->get_ret_type());
  llvm::Type* base = llvm::FunctionType::get(ret, parms, false);
  return base->getPointerTo();
}

llvm::Type*
cg_context::get_type(const Obj_Decl* d)
{
  return get_type(d->get_type());
}

// -------------------------------------------------------------------------- //
// Module implementation

/// \todo Derive the name of the output file from compiler options.
cg_module::cg_module(cg_context& cxt, const Prog_Decl* prog)
  : parent(&cxt), 
    prog(prog), 
    mod(new llvm::Module("a.ll", *get_context()))
{ }

void
cg_module::declare(const Decl* d, llvm::GlobalValue* v)
{
  assert(globals.count(d) == 0);
  globals.emplace(d, v);
}

llvm::GlobalValue*
cg_module::lookup(const Decl* d) const
{
  auto iter = globals.find(d);
  if (iter != globals.end())
    return llvm::cast<llvm::GlobalValue>(iter->second);
  else
    return nullptr;
}

/// Process top-level declarations.
void 
cg_module::generate()
{
  for (const Decl* d : prog->get_decls())
    generate(d);
}

void
cg_module::generate(const Decl* d)
{
  switch (d->get_kind()) {
  case Decl::var_decl:
  case Decl::let_decl:
    return generate_var_decl(static_cast<const Var_Decl*>(d));
  
  case Decl::function_decl:
    return generate_fn_decl(static_cast<const Function_Decl*>(d));

  default: 
    throw std::logic_error("invalid declaration");
  }
}

/// Generate a variable.
///
/// \todo To declare a global variable, we need to determine if it is
/// statically or dynamically initialized. A variable is statically 
/// initialized if it's initializer can be constant folded. Right now,
/// global variables are simply zero-initialized.
///
/// \todo Make a variable initialization context like we do for functions?
/// That might be pretty elegant.
void 
cg_module::generate_var_decl(const Var_Decl* d)
{
  std::string n = get_name(d);
  llvm::Type* t = get_type(d->get_type());
  llvm::Constant* c = llvm::Constant::getNullValue(t);
  llvm::GlobalVariable* var = new llvm::GlobalVariable(
      *mod, t, false, llvm::GlobalVariable::ExternalLinkage, c, n);

  // Create the binding.
  declare(d, var);
}

/// Generate a function from the fn expression.
void 
cg_module::generate_fn_decl(const Function_Decl* d)
{
  cg_function fn(*this, d);
  fn.define();
}

// -------------------------------------------------------------------------- //
// Function implementation

static llvm::FunctionType*
get_fn_type(llvm::Type* t)
{
  assert(llvm::isa<llvm::PointerType>(t));
  return llvm::cast<llvm::FunctionType>(t->getPointerElementType());
}

cg_function::cg_function(cg_module& m, const Function_Decl* d)
  : parent(&m), src(d), fn(), entry(), curr()
{
  std::string n = get_name(d);
  llvm::Type* t = get_type(d);
  fn = llvm::Function::Create(
      get_fn_type(t), llvm::Function::ExternalLinkage, n, get_module());

  // Create a binding in the module.
  parent->declare(d, fn);
  
  // Build and emit the entry block.
  entry = make_block("entry");
  emit_block(entry);

  llvm::IRBuilder<> ir(get_current_block());
  
  // Configure function parameters and declare them as locals.
  assert(d->get_parameters().size() == fn->arg_size());
  auto pi = d->get_parameters().begin();
  auto ai = fn->arg_begin();
  while (ai != fn->arg_end()) {
    const parm_decl* parm = static_cast<const parm_decl*>(*pi);
    llvm::Argument& arg = *ai;

    // Configure each parameter.
    arg.setName(get_name(parm));

    // Declare local variable for each parameter and initialize it
    // with wits corresponding value.
    llvm::Value* var = ir.CreateAlloca(arg.getType(), nullptr, arg.getName());
    declare(parm, var);

    // Initialize with the value of the argument.
    ir.CreateStore(&arg, var);
    
    ++ai;
    ++pi;
  }

}

void
cg_function::declare(const Decl* d, llvm::Value* v)
{
  assert(locals.count(d) == 0);
  locals.emplace(d, v);
}

llvm::Value*
cg_function::lookup(const Decl* d) const
{
  auto iter = locals.find(d);
  if (iter != locals.end())
    return iter->second;
  else
    return parent->lookup(d);
}

llvm::BasicBlock*
cg_function::make_block(const char* label)
{
  return llvm::BasicBlock::Create(*get_context(), label);
}

void
cg_function::emit_block(llvm::BasicBlock* bb)
{
  bb->insertInto(get_function());
  curr = bb;
}

/// Creates a return instruction for the expression.
void
cg_function::define()
{
  generate_stmt(src->get_body());
}

llvm::Value*
cg_function::generate_expr(const Expr* e)
{
  switch (e->get_kind()) {
  case Expr::bool_kind:
    return generate_bool_expr(static_cast<const Bool_Expr*>(e));
  case Expr::int_kind:
    return generate_int_expr(static_cast<const Int_Expr*>(e));
  case Expr::float_kind:
    return generate_float_expr(static_cast<const Float_Expr*>(e));
  case Expr::id_kind:
    return generate_id_expr(static_cast<const Id_Expr*>(e));
  case Expr::unary_kind:
    return generate_unop_expr(static_cast<const Unary_Expr*>(e));
  case Expr::binary_kind:
    return generate_binop_expr(static_cast<const Bin_Expr*>(e));
  case Expr::call_kind:
    return generate_call_expr(static_cast<const Call_Expr*>(e));
  case Expr::index_kind:
    return generate_index_expr(static_cast<const Index_Expr*>(e));
  case Expr::cond_kind:
    return generate_cond_expr(static_cast<const Conditional_Expr*>(e));
  case Expr::ass_kind:
    return generate_assign_expr(static_cast<const Assign_Expr*>(e));
  case Expr::conv_kind:
    return generate_conv_expr(static_cast<const Conversion_Expr*>(e));
  default: 
    throw std::runtime_error("invalid expression");
  }
}

llvm::Value*
cg_function::generate_bool_expr(const Bool_Expr* e)
{
  return llvm::ConstantInt::get(get_type(e), e->get_value(), false);
}

llvm::Value*
cg_function::generate_int_expr(const Int_Expr* e)
{
  return llvm::ConstantInt::get(get_type(e), e->get_value(), true);
}

llvm::Value*
cg_function::generate_float_expr(const Float_Expr* e)
{
  return llvm::ConstantFP::get(get_type(e), e->get_value());
}

llvm::Value*
cg_function::generate_id_expr(const Id_Expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_unop_expr(const Unary_Expr* e)
{
  return nullptr;
}

// Note that &e is equivalent to e. This is because e is already an address.
llvm::Value*
cg_function::generate_address_expr(const Unary_Expr* e)
{
  return nullptr;
}

// Note that *e is equivalent to e. This is because e is already an address.
llvm::Value*
cg_function::generate_deref_expr(const Unary_Expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_binop_expr(const Bin_Expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_relational_expr(const Bin_Expr* e)
{
  // llvm::Value* lhs = generate_expr(e->get_lhs());
  // llvm::Value* rhs = generate_expr(e->get_rhs());
  // llvm::IRBuilder<> ir(get_current_block());
  // switch (e->get_operator()) {
  // case op_eq:
  //   return ir.CreateICmpEQ(lhs, rhs);
  // case op_ne:
  //   return ir.CreateICmpNE(lhs, rhs);
  // case op_lt:
  //   return ir.CreateICmpSLT(lhs, rhs);
  // case op_gt:
  //   return ir.CreateICmpSGT(lhs, rhs);
  // case op_le:
  //   return ir.CreateICmpSLE(lhs, rhs);
  // case op_ge:
  //   return ir.CreateICmpSGE(lhs, rhs);
  // default:
  //   throw std::logic_error("invalid operator");
  // }
  return nullptr;
}

llvm::Value*
cg_function::generate_call_expr(const Call_Expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_index_expr(const Index_Expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_assign_expr(const Assign_Expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_cond_expr(const Conditional_Expr* e)
{
  return nullptr;
}

// FIXME: Clean this up.
llvm::Value*
cg_function::generate_conv_expr(const Conversion_Expr* c)
{
  return nullptr;
}

void
cg_function::generate_stmt(const Stmt* s)
{
  switch (s->get_kind()) {
  case Stmt::block_kind:
    return generate_block_stmt(static_cast<const Block_Stmt*>(s));
  case Stmt::if_kind:
    return generate_if_stmt(static_cast<const If_Stmt*>(s));
  case Stmt::if_else_kind:
    return generate_if_else_stmt(static_cast<const If_Else_Stmt*>(s));
  case Stmt::while_kind:
    return generate_while_stmt(static_cast<const While_Stmt*>(s));
  case Stmt::breaking_kind:
    return generate_ret_stmt(static_cast<const Breaking_Stmt*>(s));
  case Stmt::decl_kind:
    return generate_decl_stmt(static_cast<const Decl_Stmt*>(s));
  case Stmt::expr_kind:
    return generate_expr_stmt(static_cast<const Expr_Stmt*>(s));
  }
}

void
cg_function::generate_block_stmt(const Blo* s)
{
}

void
cg_function::generate_if_stmt(const If_Stmt* s)
{
}

void 
cg_function::generate_if_else_stmt(const If_Else_Stmt* s)
{
  
}

void
cg_function::generate_while_stmt(const While_Stmt* s)
{
}

void
cg_function::generate_break_stmt(const Breaking_Stmt* e)
{
}

void
cg_function::generate_cont_stmt(const Breaking_Stmt* e)
{
}

void
cg_function::generate_ret_stmt(const Breaking_Stmt* e)
{
}

void
cg_function::generate_decl_stmt(const Decl_Stmt* e)
{
}

void
cg_function::generate_expr_stmt(const Expr_Stmt* e)
{
}

void
generate(const Decl* d)
{
  assert(d->is_program());
  
  // Establish the translation context.
  cg_context cg;

  // Create the module, and generate its declarations.
  cg_module mod(cg, static_cast<const Prog_Decl*>(d));
  mod.generate();

  // Dump the generated module to 
  llvm::outs() << *mod.get_module();
}

