#include "command_line_parser.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace wc::clp;


TEST(parse, argc_equals_1) {
    int argc{ 1 };
    const char* argv[] = { "word_converter" };
    EXPECT_THROW((void) command_line_parser::parse(argc, argv), invalid_number_of_arguments_error);
}
TEST(parse, argc_equals_2) {
    int argc{ 2 };
    const char* argv[] = { "word_converter", "-i" };
    EXPECT_THROW((void) command_line_parser::parse(argc, argv), invalid_number_of_arguments_error);
}
TEST(parse, argc_equals_4) {
    int argc{ 4 };
    const char* argv[] = { "word_converter", "-i", "-o" };
    EXPECT_THROW((void) command_line_parser::parse(argc, argv), invalid_number_of_arguments_error);
}
TEST(parse, argc_equals_6) {
    int argc{ 6 };
    const char* argv[] = { "word_converter", "-i", "in.txt", "-o", "out.txt", "-h" };
    EXPECT_THROW((void) command_line_parser::parse(argc, argv), invalid_number_of_arguments_error);
}

TEST(parse, argc_equals_3_and_invalid_argv_1) {
    int argc{ 3 };
    const char* argv[] = { "word_converter", "-o", "out.txt" };
    EXPECT_THROW((void) command_line_parser::parse(argc, argv), invalid_argument_error);
}
TEST(parse, argc_equals_5_and_invalid_argv_1) {
    int argc{ 5 };
    const char* argv[] = { "word_converter", "-z", "out.txt", "-i", "in.txt" };
    EXPECT_THROW((void) command_line_parser::parse(argc, argv), invalid_argument_error);
}
TEST(parse, argc_equals_5_and_invalid_argv_3) {
    int argc{ 5 };
    const char* argv[] = { "word_converter", "-i", "in.txt", "-z", "out.txt" };
    EXPECT_THROW((void) command_line_parser::parse(argc, argv), invalid_argument_error);
}

TEST(parse, argc_equals_3_and_valid_argv) {
    int argc{ 3 };
    const char* argv[] = { "word_converter", "-i", "in.txt" };
    auto options{ command_line_parser::parse(argc, argv) };
    EXPECT_EQ(options.input_file, "in.txt");
}
TEST(parse, argc_equals_5_and_valid_argv) {
    int argc{ 5 };
    const char* argv[] = { "word_converter", "-i", "in.txt", "-o", "out.txt" };
    auto options{ command_line_parser::parse(argc, argv) };
    EXPECT_EQ(options.input_file, "in.txt");
    EXPECT_EQ(options.output_file, "out.txt");
}
TEST(parse, argc_equals_5_and_valid_argv_in_reverse_order) {
    int argc{ 5 };
    const char* argv[] = { "word_converter", "-o", "out.txt", "-i", "in.txt" };
    auto options{ command_line_parser::parse(argc, argv) };
    EXPECT_EQ(options.input_file, "in.txt");
    EXPECT_EQ(options.output_file, "out.txt");
}
