#include "conversion_manager.h"

#include <gtest/gtest.h>
#include <iterator>  // istreambuf_iterator
#include <memory>  // make_unique, unique_ptr


TEST(converter_run, empty_text) {
    std::istringstream iss{ "" };
    std::unique_ptr<input_reader> reader{ std::make_unique<stream_reader>(iss)};
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    EXPECT_TRUE(oss.str().empty());
}
TEST(converter_run, text_with_only_a_period) {
    std::string text{ "." };
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> reader{ std::make_unique<stream_reader>(iss)};
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    EXPECT_EQ(oss.str(), text);
}
TEST(converter_run, text_and_a_period) {
    std::string text{ "blah." };
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> reader{ std::make_unique<stream_reader>(iss)};
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    EXPECT_EQ(oss.str(), text);
}
TEST(converter_run, text_and_no_period) {
    std::string text{ "blah" };
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> reader{ std::make_unique<stream_reader>(iss)};
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    EXPECT_EQ(oss.str(), text);
}
TEST(converter_run, text_with_multiline_sentence) {
    std::string text{ "blah\nfoo.meh" };
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> reader{ std::make_unique<stream_reader>(iss)};
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    EXPECT_EQ(oss.str(), text);
}
TEST(converter_run, in_1_txt) {
    std::unique_ptr<input_reader> reader{ std::make_unique<file_reader>("../../res/in_1.txt") };
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    std::ifstream expected_output_ifs{ "../../res/out_1.txt" };
    std::string expected_output_str{ std::istreambuf_iterator{ expected_output_ifs }, {} };
    EXPECT_EQ(oss.str(), expected_output_str);
}
TEST(converter_run, in_2_txt) {
    std::unique_ptr<input_reader> reader{ std::make_unique<file_reader>("../../res/in_2.txt") };
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager::run(std::move(reader), writers);
    std::ifstream expected_output_ifs{ "../../res/out_2.txt" };
    std::string expected_output_str{ std::istreambuf_iterator{ expected_output_ifs }, {} };
    EXPECT_EQ(oss.str(), expected_output_str);
}
