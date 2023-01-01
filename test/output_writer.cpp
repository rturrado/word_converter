#include "output_writer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>  // make_unique


TEST(file_writer_constructor, could_not_create_file) {
    EXPECT_THROW(std::make_unique<file_writer>("blah/foo.txt"), could_not_create_file_error);
}

TEST(stream_writer_write, write_empty_text) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string text{};
    output_writer_up->write(text);
    EXPECT_TRUE(oss.str().empty());
}
TEST(stream_writer_write, write) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string text{ "blah" };
    output_writer_up->write(text);
    EXPECT_EQ(oss.str(), text);
}
TEST(stream_writer_write, write_multiline_text) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string text{ "blah\nfoo" };
    output_writer_up->write(text);
    EXPECT_EQ(oss.str(), text);
}

TEST(stream_writer_write_sentence, write_empty_sentence) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string sentence{};
    std::string expected_sentence{ "." };
    output_writer_up->write_sentence(sentence);
    EXPECT_EQ(oss.str(), expected_sentence);
}
TEST(stream_writer_write_sentence, write_sentence) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string sentence{ "blah" };
    std::string expected_sentence{ "blah." };
    output_writer_up->write_sentence(sentence);
    EXPECT_EQ(oss.str(), expected_sentence);
}
TEST(stream_writer_write_sentence, write_multiline_sentence) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string sentence{ "blah\nfoo" };
    std::string expected_sentence{ "blah\nfoo." };
    output_writer_up->write_sentence(sentence);
    EXPECT_EQ(oss.str(), expected_sentence);
}
