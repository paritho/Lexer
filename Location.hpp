#pragma once
#include <string>

struct Location
{
    Location(std::string filePath)
        : file(filepath),line(0), column(0)
    {}

    ~Location() = default;

    void next_line() { return ++line; }
    void next_column(){ return ++column; }

    private:
        std::string file;
        int line;
        int column;
}