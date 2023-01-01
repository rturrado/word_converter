#include "command_line_parser.h"
#include "input_reader.h"
#include "output_writer.h"

#include <exception>
#include <iostream>  // cout
#include <memory>  // unique_ptr
#include <vector>

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


int main_impl(std::ostream& os, int argc, const char** argv) {
    try {
        // Parse command line options
        auto options{ command_line_parser::parse(argc, argv) };

        // Create a reader and a list of writers
        auto input_reader{ std::make_unique<file_reader>(options.input_file) };
        using output_writer_list = std::vector<std::unique_ptr<output_writer>>;
        output_writer_list output_writers{};
        output_writers.push_back(std::make_unique<stream_writer>(std::cout));
        if (options.output_file) {
            output_writers.push_back(std::make_unique<file_writer>(options.output_file.value()));
        }

        /*
        // Read in text, convert it, and write it out
        converter::run(input_reader, output_writers);
        */
    } catch (const std::exception& ex) {
        os << "Error: " << ex.what() << "\n\n";
        print_usage(os);
        return -1;
    }
    return 0;
}


int main(int argc, const char** argv) {
    return main_impl(std::cout, argc, argv);
}
