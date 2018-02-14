set(CMAKE_CXX_COMPILER "/usr/bin/gcc-7")
set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(mc main.cpp Lexer.cpp Token.cpp)
