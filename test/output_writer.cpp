#include "output_writer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(file_writer_constructor, could_not_create_file) {
    EXPECT_THROW((void) std::make_unique<file_writer>("blah/foo.txt"), could_not_create_file_error);
}

TEST(stream_writer_write, write_empty_text) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string text{};
    output_writer_up->write(text);
    EXPECT_TRUE(oss.str().empty());
}
TEST(stream_writer_write, write_text) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string text{ "blah" };
    output_writer_up->write(text);
    EXPECT_EQ(oss.str(), text);
}
TEST(stream_writer_write, write_text_with_period) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string text{ "blah." };
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
TEST(stream_writer_write, write_multiline_text_with_period) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string text{ "blah\nfoo." };
    output_writer_up->write(text);
    EXPECT_EQ(oss.str(), text);
}
