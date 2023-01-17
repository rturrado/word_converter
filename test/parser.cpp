#include "input_reader.h"
#include "parser.h"

#include <fmt/format.h>
#include <filesystem>
#include <gtest/gtest.h>
#include <sstream>  // istringstream

namespace fs = std::filesystem;


// Empty input text
TEST(parser_parse, empty_input_text) {
    std::istringstream iss{ "" };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "");
}

// Text
TEST(parser_parse, text) {
    std::istringstream iss{ "foo" };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo");
}
TEST(parser_parse, multiline_text) {
    std::istringstream iss{ "foo\nmeh" };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo\nmeh");
}

// Sentence and text combinations
TEST(parser_parse, empty_sentence) {
    std::istringstream iss{ "." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), ".");
}
TEST(parser_parse, text_sentence) {
    std::istringstream iss{ "foo." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo.");
}
TEST(parser_parse, multiline_text_sentence) {
    std::istringstream iss{ "foo\nmeh." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo\nmeh.");
}
TEST(parser_parse, multiline_text_sentence_and_text) {
    std::istringstream iss{ "foo\nmeh.blah" };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo\nmeh.blah");
}

// Two sentences
TEST(parser_parse, two_empty_sentences) {
    std::istringstream iss{ ".." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "..");
}
TEST(parser_parse, empty_sentence_and_text_sentence) {
    std::istringstream iss{ ". foo." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), ". foo.");
}
TEST(parser_parse, empty_sentence_and_number_sentence) {
    std::istringstream iss{ ". one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), ". 1.");
}
TEST(parser_parse, text_sentence_and_empty_sentence) {
    std::istringstream iss{ "foo.." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo..");
}
TEST(parser_parse, number_sentence_and_empty_sentence) {
    std::istringstream iss{ "one.." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "1..");
}

// Files
TEST(parser_parse, in_1_txt) {
    fs::path input_file_path{ "../../res/in_1.txt" };
    std::ifstream expected_output_ifs{ "../../res/out_1.txt" };
    std::string expected_output_str{ std::istreambuf_iterator{ expected_output_ifs }, {} };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<file_reader>(input_file_path))->parse(), expected_output_str);
}
TEST(parser_parse, in_2_txt) {
    fs::path input_file_path{ "../../res/in_2.txt" };
    std::ifstream expected_output_ifs{ "../../res/out_2.txt" };
    std::string expected_output_str{ std::istreambuf_iterator{ expected_output_ifs }, {} };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<file_reader>(input_file_path))->parse(), expected_output_str);
}

// Text and numbers
TEST(parser_parse, number) {
    std::istringstream iss{ "one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "1.");
}
TEST(parser_parse, text_number) {
    std::istringstream iss{ "foo one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo 1.");
}
TEST(parser_parse, number_text) {
    std::istringstream iss{ "one foo." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "1 foo.");
}
TEST(parser_parse, text_number_text) {
    std::istringstream iss{ "foo one meh." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo 1 meh.");
}
TEST(parser_parse, text_number_text_number) {
    std::istringstream iss{ "foo one meh two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo 1 meh 2.");
}
TEST(parser_parse, text_number_text_number_text) {
    std::istringstream iss{ "foo one meh two blah." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "foo 1 meh 2 blah.");
}

// Numbers
TEST(parser_parse, zero) {
    std::istringstream iss{ "zero." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "0.");
}
TEST(parser_parse, one) {
    std::istringstream iss{ "one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "1.");
}
TEST(parser_parse, nine) {
    std::istringstream iss{ "nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "9.");
}
TEST(parser_parse, ten) {
    std::istringstream iss{ "ten." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "10.");
}
TEST(parser_parse, nineteen) {
    std::istringstream iss{ "nineteen." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "19.");
}
TEST(parser_parse, twenty) {
    std::istringstream iss{ "twenty." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "20.");
}
TEST(parser_parse, twenty_one) {
    std::istringstream iss{ "twenty one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "21.");
}
TEST(parser_parse, twenty_dash_one) {
    std::istringstream iss{ "twenty-one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "21.");
}
TEST(parser_parse, twenty_nine) {
    std::istringstream iss{ "twenty nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "29.");
}
TEST(parser_parse, twenty_dash_nine) {
    std::istringstream iss{ "twenty-nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "29.");
}
TEST(parser_parse, ninety) {
    std::istringstream iss{ "ninety." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "90.");
}
TEST(parser_parse, ninety_one) {
    std::istringstream iss{ "ninety one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "91.");
}
TEST(parser_parse, ninety_dash_one) {
    std::istringstream iss{ "ninety-one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "91.");
}
TEST(parser_parse, ninety_nine) {
    std::istringstream iss{ "ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "99.");
}
TEST(parser_parse, ninety_dash_nine) {
    std::istringstream iss{ "ninety-nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "99.");
}
TEST(parser_parse, one_hundred) {
    std::istringstream iss{ "one hundred." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "100.");
}
TEST(parser_parse, one_hundred_and_one) {
    std::istringstream iss{ "one hundred and one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "101.");
}
TEST(parser_parse, one_hundred_and_ninety) {
    std::istringstream iss{ "one hundred and ninety." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "190.");
}
TEST(parser_parse, one_hundred_and_ninety_nine) {
    std::istringstream iss{ "one hundred and ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "199.");
}
TEST(parser_parse, one_hundred_and_ninety_dash_nine) {
    std::istringstream iss{ "one hundred and ninety-nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "199.");
}
TEST(parser_parse, nine_hundred) {
    std::istringstream iss{ "nine hundred." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "900.");
}
TEST(parser_parse, nine_hundred_and_one) {
    std::istringstream iss{ "nine hundred and one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "901.");
}
TEST(parser_parse, nine_hundred_and_ninety) {
    std::istringstream iss{ "nine hundred and ninety." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "990.");
}
TEST(parser_parse, nine_hundred_and_ninety_nine) {
    std::istringstream iss{ "nine hundred and ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "999.");
}
TEST(parser_parse, nine_hundred_and_ninety_dash_nine) {
    std::istringstream iss{ "nine hundred and ninety-nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "999.");
}
TEST(parser_parse, one_thousand) {
    std::istringstream iss{ "one thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000));
}
TEST(parser_parse, nine_thousand) {
    std::istringstream iss{ "nine thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 9'000));
}
TEST(parser_parse, nine_hundred_thousand) {
    std::istringstream iss{ "nine hundred thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 900'000));
}
TEST(parser_parse, nine_hundred_and_ninety_nine_thousand) {
    std::istringstream iss{ "nine hundred and ninety nine thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 999'000));
}
TEST(parser_parse, nine_hundred_and_ninety_dash_nine_thousand) {
    std::istringstream iss{ "nine hundred and ninety-nine thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 999'000));
}
TEST(parser_parse, one_million) {
    std::istringstream iss{ "one million." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000'000));
}
TEST(parser_parse, nine_million) {
    std::istringstream iss{ "nine million." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 9'000'000));
}
TEST(parser_parse, nine_hundred_million) {
    std::istringstream iss{ "nine hundred million." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 900'000'000));
}
TEST(parser_parse, nine_hundred_and_ninety_nine_million) {
    std::istringstream iss{ "nine hundred and ninety nine million." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 999'000'000));
}
TEST(parser_parse, nine_hundred_and_ninety_dash_nine_million) {
    std::istringstream iss{ "nine hundred and ninety-nine million." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 999'000'000));
}
TEST(parser_parse, nine_hundred_and_ninety_nine_million_nine_hundred_and_ninety_thousand_nine_hundred_and_ninety_nine) {
    std::istringstream iss{ "nine hundred and ninety nine million nine hundred and ninety nine thousand nine hundred and ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 999'999'999));
}
TEST(parser_parse, one_billion) {
    std::istringstream iss{ "one billion." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000'000'000));
}

TEST(parser_parse, one_hundred_and_two) {
    std::istringstream iss{ "one hundred and two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "102.");
}
TEST(parser_parse, one_thousand_and_two) {
    std::istringstream iss{ "one thousand and two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'002));
}
TEST(parser_parse, one_million_and_two) {
    std::istringstream iss{ "one million and two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000'002));
}
TEST(parser_parse, one_hundred_and_twenty_three) {
    std::istringstream iss{ "one hundred and twenty three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "123.");
}
TEST(parser_parse, one_hundred_and_twenty_dash_three) {
    std::istringstream iss{ "one hundred and twenty-three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "123.");
}
TEST(parser_parse, one_thousand_and_twenty_three) {
    std::istringstream iss{ "one thousand and twenty three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'023));
}
TEST(parser_parse, one_thousand_and_twenty_dash_three) {
    std::istringstream iss{ "one thousand and twenty-three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'023));
}
TEST(parser_parse, one_million_and_twenty_three) {
    std::istringstream iss{ "one million and twenty three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000'023));
}
TEST(parser_parse, one_million_and_twenty_dash_three) {
    std::istringstream iss{ "one million and twenty-three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000'023));
}
TEST(parser_parse, one_thousand_two_hundred_and_three) {
    std::istringstream iss{ "one thousand two hundred and three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'203));
}
TEST(parser_parse, one_million_two_hundred_and_three) {
    std::istringstream iss{ "one million two hundred and three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000'203));
}
TEST(parser_parse, one_million_two_thousand_and_three) {
    std::istringstream iss{ "one million two thousand and three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'002'003));
}
TEST(parser_parse, one_million_two_thousand_three_hundred_and_four) {
    std::istringstream iss{ "one million two thousand three hundred and four." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'002'304));
}

TEST(parser_parse, three_million_six_hundred_and_three_thousand_eight_hundred_and_two) {
    std::istringstream iss{ "three million six hundred and three thousand eight hundred and two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 3'603'802));
}
TEST(parser_parse, seven_million_five_hundred_and_twelve_thousand_one_hundred_and_forty_five) {
    std::istringstream iss{ "seven million five hundred and twelve thousand one hundred and forty five." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 7'512'145));
}
TEST(parser_parse, three_million_eight_hundred_thousand_and_eighteen) {
    std::istringstream iss{ "three million eight hundred thousand and eighteen." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 3'800'018));
}

// Uppercase
TEST(parser_parse, Zero) {
    std::istringstream iss{ "Zero." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "0.");
}
TEST(parser_parse, ZERO) {
    std::istringstream iss{ "ZERO." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "0.");
}
TEST(parser_parse, twenty_dash_One) {
    std::istringstream iss{ "twenty-One." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "21.");
}
TEST(parser_parse, twenty_dash_ONE) {
    std::istringstream iss{ "twenty-ONE." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "21.");
}
TEST(parser_parse, one_BILLION) {
    std::istringstream iss{ "one BILLION." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), fmt::format("{}.", 1'000'000'000));
}

// And connector
TEST(parser_parse, one_and_two) {
    std::istringstream iss{ "one and two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "1 and 2.");
}
TEST(parser_parse, two_and_one) {
    std::istringstream iss{ "two and one." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "2 and 1.");
}
TEST(parser_parse, one_hundred_comma_and_two) {
    std::istringstream iss{ "one hundred, and two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "100, and 2.");
}
TEST(parser_parse, one_hundred_and_two_and_three) {
    std::istringstream iss{ "one hundred and two and three." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "102 and 3.");
}
TEST(parser_parse, one_hundred_and_three_and_two) {
    std::istringstream iss{ "one hundred and three and two." };
    EXPECT_EQ(std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), "103 and 2.");
}

// Malformed numbers
TEST(parser_parse, number_number) {
    std::istringstream iss{ "one two." };
    EXPECT_THROW((void) std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), invalid_token_error);
}
TEST(parser_parse, one_hundred_two_hundred) {
    std::istringstream iss{ "one hundred two hundred." };
    EXPECT_THROW((void) std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), invalid_token_error);
}
TEST(parser_parse, one_thousand_million) {
    std::istringstream iss{ "one thousand million." };
    EXPECT_THROW((void) std::make_unique<parser>(std::make_unique<stream_reader>(iss))->parse(), invalid_token_error);
}
