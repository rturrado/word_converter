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
    conversion_manager manager{std::make_unique<word_to_number_converter>() };
    manager.run(std::move(reader), writers);
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
    conversion_manager manager{std::make_unique<word_to_number_converter>() };
    manager.run(std::move(reader), writers);
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
    conversion_manager manager{std::make_unique<word_to_number_converter>() };
    manager.run(std::move(reader), writers);
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
    conversion_manager manager{std::make_unique<word_to_number_converter>() };
    manager.run(std::move(reader), writers);
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
    conversion_manager manager{std::make_unique<word_to_number_converter>() };
    manager.run(std::move(reader), writers);
    EXPECT_EQ(oss.str(), text);
}


TEST(convert, zero) {
    EXPECT_EQ(word_to_number_converter{}.convert("zero"), "0");
}
TEST(convert, one) {
    EXPECT_EQ(word_to_number_converter{}.convert("one"), "1");
}
TEST(convert, nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("nine"), "9");
}
TEST(convert, ten) {
    EXPECT_EQ(word_to_number_converter{}.convert("ten"), "10");
}
TEST(convert, nineteen) {
    EXPECT_EQ(word_to_number_converter{}.convert("nineteen"), "19");
}
TEST(convert, twenty) {
    EXPECT_EQ(word_to_number_converter{}.convert("twenty"), "20");
}
TEST(convert, twenty_one) {
    EXPECT_EQ(word_to_number_converter{}.convert("twenty one"), "21");
}
TEST(convert, twenty_dash_one) {
    EXPECT_EQ(word_to_number_converter{}.convert("twenty-one"), "21");
}
TEST(convert, twenty_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("twenty nine"), "29");
}
TEST(convert, twenty_dash_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("twenty-nine"), "29");
}
TEST(convert, ninety) {
    EXPECT_EQ(word_to_number_converter{}.convert("ninety"), "90");
}
TEST(convert, ninety_one) {
    EXPECT_EQ(word_to_number_converter{}.convert("ninety one"), "91");
}
TEST(convert, ninety_dash_one) {
    EXPECT_EQ(word_to_number_converter{}.convert("ninety-one"), "91");
}
TEST(convert, ninety_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("ninety nine"), "99");
}
TEST(convert, ninety_dash_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("ninety-nine"), "99");
}
TEST(convert, one_hundred) {
    EXPECT_EQ(word_to_number_converter{}.convert("one hundred"), "100");
}
TEST(convert, one_hundred_and_one) {
    EXPECT_EQ(word_to_number_converter{}.convert("one hundred and one"), "101");
}
TEST(convert, one_hundred_and_ninety) {
    EXPECT_EQ(word_to_number_converter{}.convert("one hundred and ninety"), "190");
}
TEST(convert, one_hundred_and_ninety_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("one hundred and ninety nine"), "199");
}
TEST(convert, one_hundred_and_ninety_dash_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("one hundred and ninety-nine"), "199");
}
TEST(convert, nine_hundred) {
    EXPECT_EQ(word_to_number_converter{}.convert("nine hundred"), "900");
}
TEST(convert, nine_hundred_and_one) {
    EXPECT_EQ(word_to_number_converter{}.convert("nine hundred and one"), "901");
}
TEST(convert, nine_hundred_and_ninety) {
    EXPECT_EQ(word_to_number_converter{}.convert("nine hundred and ninety"), "990");
}
TEST(convert, nine_hundred_and_ninety_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("nine hundred and ninety nine"), "999");
}
TEST(convert, nine_hundred_and_ninety_dash_nine) {
    EXPECT_EQ(word_to_number_converter{}.convert("nine hundred and ninety-nine"), "999");
}
TEST(convert, one_thousand) {
    EXPECT_EQ(word_to_number_converter{}.convert("one thousand"), "1000");
}
TEST(convert, one_million) {
    EXPECT_EQ(word_to_number_converter{}.convert("one million"), "1000000");
}
TEST(convert, one_billion) {
    EXPECT_EQ(word_to_number_converter{}.convert("one billion"), "1000000000");
}
TEST(convert, three_million_six_hundred_and_three_thousand_eight_hundred_and_two) {
    EXPECT_EQ(word_to_number_converter{}.convert("three million six hundred and three thousand eight hundred and two"), "3603802");
}
TEST(convert, seven_million_five_hundred_and_twelve_thousand_one_hundred_and_forty_dash_five) {
    EXPECT_EQ(word_to_number_converter{}.convert("seven million five hundred and twelve thousand one hundred and forty-five"), "7512145");
}
TEST(convert, three_million_eight_hundred_thousand_and_eighteen) {
    EXPECT_EQ(word_to_number_converter{}.convert("three million eight hundred thousand and eighteen"), "3800018");
}

TEST(convert, one_hundred_comma_and_one) {
    EXPECT_EQ(word_to_number_converter{}.convert("one hundred, and one"), "100, and 1");
}
