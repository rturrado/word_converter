#include "converter.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>


TEST(converter_run, empty_text) {
    std::istringstream iss{ "" };
    std::unique_ptr<input_reader> reader{ std::make_unique<stream_reader>(iss)};
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    converter::run(std::move(reader), writers);
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
    converter::run(std::move(reader), writers);
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
    converter::run(std::move(reader), writers);
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
    converter::run(std::move(reader), writers);
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
    converter::run(std::move(reader), writers);
    EXPECT_EQ(oss.str(), text);
}
