#pragma once
#include <string>

struct Location
{
    Location(std::string filePath)
        : file(filepath),line(-1), column(-1)
    {}

    ~Location() = default;

    void next_line() { return ++line; }
    void next_column(){ return ++column; }
    void set_column(int col) {return column = col; }
    void set_path(std::string path) { return file = path; }

    private:
        std::string file;
        int line;
        int column;
}