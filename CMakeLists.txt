set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++14")

add_executable(mc main.cpp Lexer.cpp Token.cpp)