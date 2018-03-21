set(CMAKE_CXX_FLAGS "-std=c++1z -lstdc++")

add_executable(mc main.cpp Lexer.cpp Token.cpp Parser.cpp)
