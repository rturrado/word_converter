#include "output_writer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>  // make_unique


TEST(file_writer, could_not_create_file) {
    EXPECT_THROW(std::make_unique<file_writer>("blah/foo.txt"), could_not_create_file_error);
}

TEST(stream_writer, write_empty_sentence) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string sentence{};
    output_writer_up->write_sentence(sentence);
    EXPECT_TRUE(oss.str().empty());
}
TEST(stream_writer, write_sentence) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string sentence{ "blah." };
    output_writer_up->write_sentence(sentence);
    EXPECT_EQ(oss.str(), sentence);
}
TEST(stream_writer, write_multiline_sentence) {
    std::ostringstream oss{};
    std::unique_ptr<output_writer> output_writer_up{ std::make_unique<stream_writer>(oss) };
    std::string sentence{ "blah\nfoo." };
    output_writer_up->write_sentence(sentence);
    EXPECT_EQ(oss.str(), sentence);
}
