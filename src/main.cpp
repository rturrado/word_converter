#include "command_line_parser.h"
#include "input_reader.h"
#include "output_writer.h"
#include "parser.h"

#include <exception>
#include <fmt/ostream.h>
#include <iostream>  // cout
#include <memory>  // make_unique
#include <vector>


void print_usage(std::ostream& os) {
    fmt::print(os, "Usage:\n");
    fmt::print(os, "\tword_converter -i <INPUT_FILE_PATH> [-o <OUTPUT_FILE_PATH>]\n");
    fmt::print(os, "Where:\n");
    fmt::print(os, "\tINPUT_FILE_PATH   Path to an input text file.\n");
    fmt::print(os, "\tOUTPUT_FILE_PATH  Path to an output text file. This parameter is optional.\n");
    fmt::print(os, "Example:\n");
    fmt::print(os, "\tword_converter -i in.txt\n");
    fmt::print(os, "\tword_converter -i in.txt -o out.txt\n");
}


int main_impl(std::ostream& os, int argc, const char** argv) {
    try {
        // Parse command line options
        auto options{ command_line_parser::parse(argc, argv) };

        // Create a reader and a list of writers
        input_reader_up input_reader{ std::make_unique<file_reader>(options.input_file) };
        std::vector<output_writer_up> output_writers{};
        output_writers.push_back(std::make_unique<stream_writer>(os));
        if (options.output_file) {
            output_writers.push_back(std::make_unique<file_writer>(options.output_file.value()));
        }

        // Parse input text
        const auto& output_text{ std::make_unique<parser>(std::move(input_reader))->parse() };

        // Write out output text
        std::ranges::for_each(output_writers, [&output_text](auto& writer) { writer->write(output_text); });
    } catch (const std::exception& ex) {
        fmt::print(os, "Error: {}\n\n", ex.what());
        print_usage(os);
        return -1;
    }
    return 0;
}


int main(int argc, const char** argv) {
    return main_impl(std::cout, argc, argv);
}
