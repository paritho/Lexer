set(CMAKE_CXX_FLAGS "-std=c++1z -lstdc++")

add_executable(mc main.cpp 
                  Lexer.cpp 
                  Token.cpp 
                  Parser.cpp
                  Expr.cpp
                  Decl.cpp
                  Stmt.cpp
                  Type.cpp
                  Semantics.cpp
                  Scope.cpp)
