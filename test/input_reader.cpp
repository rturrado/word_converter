#include "input_reader.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>  // istringstream


TEST(file_reader_constructor, file_is_not_a_regular_file) {
    EXPECT_THROW((void) std::make_unique<file_reader>("foo.txt"), file_is_not_a_regular_file_error);
}
TEST(file_reader_read_sentence, empty_file) {
    std::unique_ptr<input_reader> file_reader_up{ std::make_unique<file_reader>("../../res/empty_file.txt")};
    EXPECT_EQ(file_reader_up->read(), "");
    EXPECT_TRUE(file_reader_up->fail());
    EXPECT_TRUE(file_reader_up->eof());
}
TEST(file_reader_read_sentence, file_with_only_a_period) {
    std::unique_ptr<input_reader> file_reader_up{ std::make_unique<file_reader>("../../res/file_with_only_a_period.txt")};
    EXPECT_EQ(file_reader_up->read(), ".");
    EXPECT_FALSE(file_reader_up->fail());
    EXPECT_FALSE(file_reader_up->eof());
}
TEST(file_reader_read_sentence, file_with_text_and_a_period) {
    std::unique_ptr<input_reader> file_reader_up{ std::make_unique<file_reader>("../../res/file_with_text_and_a_period.txt")};
    EXPECT_EQ(file_reader_up->read(), "blah.");
    EXPECT_FALSE(file_reader_up->fail());
    EXPECT_FALSE(file_reader_up->eof());
}
TEST(file_reader_read_sentence, file_with_text_and_no_period) {
    std::unique_ptr<input_reader> file_reader_up{ std::make_unique<file_reader>("../../res/file_with_text_and_no_period.txt")};
    EXPECT_EQ(file_reader_up->read(), "blah");
    EXPECT_FALSE(file_reader_up->fail());
    EXPECT_TRUE(file_reader_up->eof());
}
TEST(file_reader_read_sentence, file_with_multiline_sentence) {
    std::unique_ptr<input_reader> file_reader_up{ std::make_unique<file_reader>("../../res/file_with_multiline_sentence.txt")};
    EXPECT_EQ(file_reader_up->read(), "blah\nfoo.");
    EXPECT_FALSE(file_reader_up->fail());
    EXPECT_FALSE(file_reader_up->eof());
}

TEST(stream_reader_read_sentence, empty_string) {
    std::string text{};
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> stream_reader_up{ std::make_unique<stream_reader>(iss)};
    EXPECT_EQ(stream_reader_up->read(), text);
    EXPECT_TRUE(stream_reader_up->fail());
    EXPECT_TRUE(stream_reader_up->eof());
}
TEST(stream_reader_read_sentence, stream_with_only_a_period) {
    std::string text{ "." };
    std::istringstream iss{ text } ;
    std::unique_ptr<input_reader> stream_reader_up{ std::make_unique<stream_reader>(iss)};
    EXPECT_EQ(stream_reader_up->read(), text);
    EXPECT_FALSE(stream_reader_up->fail());
    EXPECT_FALSE(stream_reader_up->eof());
}
TEST(stream_reader_read_sentence, stream_with_text_and_a_period) {
    std::string text{ "blah." };
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> stream_reader_up{ std::make_unique<stream_reader>(iss)};
    EXPECT_EQ(stream_reader_up->read(), text);
    EXPECT_FALSE(stream_reader_up->fail());
    EXPECT_FALSE(stream_reader_up->eof());
}
TEST(stream_reader_read_sentence, stream_with_text_and_no_period) {
    std::string text{ "blah" };
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> stream_reader_up{ std::make_unique<stream_reader>(iss)};
    EXPECT_EQ(stream_reader_up->read(), text);
    EXPECT_FALSE(stream_reader_up->fail());
    EXPECT_TRUE(stream_reader_up->eof());
}
TEST(stream_reader_read_sentence, stream_with_multiline_sentence) {
    std::string text{ "blah\nfoo.meh" };
    std::istringstream iss{ text };
    std::unique_ptr<input_reader> stream_reader_up{ std::make_unique<stream_reader>(iss)};
    EXPECT_EQ(stream_reader_up->read(), "blah\nfoo.");
    EXPECT_FALSE(stream_reader_up->fail());
    EXPECT_FALSE(stream_reader_up->eof());
}
