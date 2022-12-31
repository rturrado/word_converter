#include "command_line_parser.h"

#include <exception>
#include <iostream>  // cout

using namespace wc::clp;


void print_usage(std::ostream& os) {
    os << "Usage:\n";
    os << "\tword_converter -i <INPUT_FILE_PATH> [-o <OUTPUT_FILE_PATH>]\n";
    os << "Where:\n";
    os << "\tINPUT_FILE_PATH   Path to an input text file.\n";
    os << "\tOUTPUT_FILE_PATH  Path to an output text file. This parameter is optional.\n";
    os << "Example:\n";
    os << "\tword_converter -i in.txt\n";
    os << "\tword_converter -i in.txt -o out.txt\n";
}


int main_impl(int argc, const char** argv, std::ostream& os) {
    try {
        auto options{ command_line_parser::parse(argc, argv) };
    } catch (const std::exception& ex) {
        os << "Error: " << ex.what() << "\n\n";
        print_usage(os);
        return -1;
    }
    return 0;
}


int main(int argc, const char** argv) {
    return main_impl(argc, argv, std::cout);
}
