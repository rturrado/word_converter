#include "converter.h"

#include <gtest/gtest.h>
#include <iterator>  // istreambuf_iterator
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
TEST(converter_run, in_1_txt) {
    std::unique_ptr<input_reader> reader{ std::make_unique<file_reader>("../../res/in_1.txt") };
    std::ostringstream oss{};
    std::unique_ptr<output_writer> writer{ std::make_unique<stream_writer>(oss) };
    output_writer_up_list writers{};
    writers.push_back(std::move(writer));
    conversion_manager manager{std::make_unique<word_to_number_converter>() };
    manager.run(std::move(reader), writers);
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
    conversion_manager manager{std::make_unique<word_to_number_converter>() };
    manager.run(std::move(reader), writers);
    std::ifstream expected_output_ifs{ "../../res/out_2.txt" };
    std::string expected_output_str{ std::istreambuf_iterator{ expected_output_ifs }, {} };
    EXPECT_EQ(oss.str(), expected_output_str);
}


TEST(parse, zero) {
    word_to_number_converter c{};
    (void) c.parse("zero");
    EXPECT_EQ(c.parse("."), "0.");
}
TEST(parse, one) {
    word_to_number_converter c{};
    (void) c.parse("one");
    EXPECT_EQ(c.parse("."), "1.");
}
TEST(parse, nine) {
    word_to_number_converter c{};
    (void) c.parse("nine");
    EXPECT_EQ(c.parse("."), "9.");
}
TEST(parse, ten) {
    word_to_number_converter c{};
    (void) c.parse("ten");
    EXPECT_EQ(c.parse("."), "10.");
}
TEST(parse, nineteen) {
    word_to_number_converter c{};
    (void) c.parse("nineteen");
    EXPECT_EQ(c.parse("."), "19.");
}
TEST(parse, twenty) {
    word_to_number_converter c{};
    (void) c.parse("twenty");
    EXPECT_EQ(c.parse("."), "20.");
}
TEST(parse, twenty_one) {
    word_to_number_converter c{};
    (void) c.parse("twenty");
    (void) c.parse(" ");
    (void) c.parse("one");
    EXPECT_EQ(c.parse("."), "21.");
}
TEST(parse, twenty_dash_one) {
    word_to_number_converter c{};
    (void) c.parse("twenty");
    (void) c.parse("-");
    (void) c.parse("one");
    EXPECT_EQ(c.parse("."), "21.");
}
TEST(parse, twenty_nine) {
    word_to_number_converter c{};
    (void) c.parse("twenty");
    (void) c.parse(" ");
    (void) c.parse("nine");
    EXPECT_EQ(c.parse("."), "29.");
}
TEST(parse, twenty_dash_nine) {
    word_to_number_converter c{};
    (void) c.parse("twenty");
    (void) c.parse("-");
    (void) c.parse("nine");
    EXPECT_EQ(c.parse("."), "29.");
}
TEST(parse, ninety) {
    word_to_number_converter c{};
    (void) c.parse("ninety");
    EXPECT_EQ(c.parse("."), "90.");
}
TEST(parse, ninety_one) {
    word_to_number_converter c{};
    (void) c.parse("ninety");
    (void) c.parse(" ");
    (void) c.parse("one");
    EXPECT_EQ(c.parse("."), "91.");
}
TEST(parse, ninety_dash_one) {
    word_to_number_converter c{};
    (void) c.parse("ninety");
    (void) c.parse("-");
    (void) c.parse("one");
    EXPECT_EQ(c.parse("."), "91.");
}
TEST(parse, ninety_nine) {
    word_to_number_converter c{};
    (void) c.parse("ninety");
    (void) c.parse(" ");
    (void) c.parse("nine");
    EXPECT_EQ(c.parse("."), "99.");
}
TEST(parse, ninety_dash_nine) {
    word_to_number_converter c{};
    (void) c.parse("ninety");
    (void) c.parse("-");
    (void) c.parse("nine");
    EXPECT_EQ(c.parse("."), "99.");
}
TEST(parse, one_hundred) {
    word_to_number_converter c{};
    (void) c.parse("one");
    (void) c.parse(" ");
    (void) c.parse("hundred");
    EXPECT_EQ(c.parse("."), "100.");
}
TEST(parse, one_hundred_and_one) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one hundred and one." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "101.");
}
TEST(parse, one_hundred_and_ninety) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one hundred and ninety." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "190.");
}
TEST(parse, one_hundred_and_ninety_nine) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one hundred and ninety nine." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "199.");
}
TEST(parse, one_hundred_and_ninety_dash_nine) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one hundred and ninety-nine." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "199.");
}
TEST(parse, nine_hundred) {
    word_to_number_converter c{};
    (void) c.parse("nine");
    (void) c.parse(" ");
    (void) c.parse("hundred");
    EXPECT_EQ(c.parse("."), "900.");
}
TEST(parse, nine_hundred_and_one) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "nine hundred and one." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "901.");
}
TEST(parse, nine_hundred_and_ninety) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "nine hundred and ninety." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "990.");
}
TEST(parse, nine_hundred_and_ninety_nine) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "nine hundred and ninety nine." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "999.");
}
TEST(parse, nine_hundred_and_ninety_dash_nine) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "nine hundred and ninety-nine." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "999.");
}
TEST(parse, one_thousand) {
    word_to_number_converter c{};
    (void) c.parse("one");
    (void) c.parse(" ");
    (void) c.parse("thousand");
    EXPECT_EQ(c.parse("."), "1000.");
}
TEST(parse, one_million) {
    word_to_number_converter c{};
    (void) c.parse("one");
    (void) c.parse(" ");
    (void) c.parse("million");
    EXPECT_EQ(c.parse("."), "1000000.");
}
TEST(parse, one_billion) {
    word_to_number_converter c{};
    (void) c.parse("one");
    (void) c.parse(" ");
    (void) c.parse("billion");
    EXPECT_EQ(c.parse("."), "1000000000.");
}
TEST(parse, three_million_six_hundred_and_three_thousand_eight_hundred_and_two) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "three million six hundred and three thousand eight hundred and two." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "3603802.");
}
TEST(parse, seven_million_five_hundred_and_twelve_thousand_one_hundred_and_forty_dash_five) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "seven million five hundred and twelve thousand one hundred and forty-five." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "7512145.");
}
TEST(parse, three_million_eight_hundred_thousand_and_eighteen) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "three million eight hundred thousand and eighteen." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "3800018.");
}

TEST(parse, one_hundred_comma_and_one) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one hundred, and one." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "100, and 1.");
}
TEST(parse, eight_and_five) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "eight and five." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "8 and 5.");
}
TEST(parse, one_hundred_and_eight_and_five) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one hundred and eight and five." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "108 and 5.");
}
TEST(parse, one_and_four) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one and four." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "1 and 4.");
}
TEST(parse, one_hundred_and_one_and_four) {
    word_to_number_converter c{};
    tokenizer tokenizer{ "one hundred and one and four." };
    std::string output_text{};
    for (const auto& token : tokenizer.get_next_token()) {
        output_text += c.parse(token);
    }
    EXPECT_EQ(output_text, "101 and 4.");
}
