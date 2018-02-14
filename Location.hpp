#pragma once
#include <string>

struct Location
{
    Location() = default;
    Location(std::string filepath)
        : file(filepath),line(-1), column(-1)
    {}

    ~Location() = default;

    void next_line() { ++line; }
    void next_column(){ ++column; }

    void set_column(int col) { column = col; }
    void set_path(std::string path) {file = path; }

    int get_line() {return line;}
    int get_column() {return column;}
    std::string get_file() {return file;}

    private:
        std::string file;
        int line;
        int column;
};