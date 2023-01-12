#include "conversion_manager.h"

#include <gtest/gtest.h>
#include <memory>  // make_unique, unique_ptr
#include <string>
#include <utility>  // pair


class conversion_manager_with_input_stream_reader : public ::testing::TestWithParam<std::pair<std::string, std::string>> {};

TEST_P(conversion_manager_with_input_stream_reader, checks_expected_output_text) {
    auto [input_text, expected_output_text] = GetParam();
    std::istringstream iss{ input_text };
    std::unique_ptr<input_reader> reader{ std::make_unique<stream_reader>(iss) };
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    EXPECT_EQ(oss.str(), expected_output_text);
}

INSTANTIATE_TEST_SUITE_P(
    run,
    conversion_manager_with_input_stream_reader,
    ::testing::Values(
        std::make_pair("", ""),  // empty text
        std::make_pair(".", "."),  // text_with_only_a_period
        std::make_pair("blah.", "blah."),  // text and a period
        std::make_pair("blah", "blah"),  // text and no period
        std::make_pair("blah\nfoo.meh", "blah\nfoo.meh"),  // text with multiline sentence
        std::make_pair("..", ".."),  // two empty sentences
        std::make_pair(". foo.", ". foo."),  // emtpy sentence and text
        std::make_pair(". one.", ". 1."),  // empty sentence and number
        std::make_pair("foo..", "foo.."),  // text and empty sentence
        std::make_pair("one..", "1..")  // number and empty sentence
    )
);

class conversion_manager_with_input_file_reader : public ::testing::TestWithParam<std::pair<std::string, std::string>> {};

TEST_P(conversion_manager_with_input_file_reader, checks_expected_output_text) {
    auto [in_file_path_str, out_file_path_str] = GetParam();
    std::unique_ptr<input_reader> reader{ std::make_unique<file_reader>(in_file_path_str) };
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    std::ifstream expected_output_ifs{ out_file_path_str };
    std::string expected_output_str{ std::istreambuf_iterator{ expected_output_ifs }, {} };
    EXPECT_EQ(oss.str(), expected_output_str);
}

INSTANTIATE_TEST_SUITE_P(
    run,
    conversion_manager_with_input_file_reader,
    ::testing::Values(
        std::make_pair("../../res/in_1.txt", "../../res/out_1.txt"),  // in_1.txt
        std::make_pair("../../res/in_2.txt", "../../res/out_2.txt")  // in_2.txt
    )
);
