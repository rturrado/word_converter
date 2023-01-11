#include "parser.h"

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <memory>  // make_unique


// 'parse' will only receive sentences, i.e. texts ending in a period

TEST(parser_parse, empty_sentence) {
    std::string text{ "." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), ".");
}
TEST(parser_parse, two_empty_sentences) {
    std::string text{ ".." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "..");
}
TEST(parser_parse, empty_sentence_and_text) {
    std::string text{ ". foo." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), ". foo.");
}
TEST(parser_parse, empty_sentence_and_number) {
    std::string text{ ". one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), ". 1");
}
TEST(parser_parse, text_and_empty_sentence) {
    std::string text{ "foo.." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "foo..");
}
TEST(parser_parse, number_and_empty_sentence) {
    std::string text{ "one.." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "1..");
}

TEST(parser_parse, number) {
    std::string text{ "one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "1.");
}
TEST(parser_parse, text_number) {
    std::string text{ "foo one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "foo 1.");
}
TEST(parser_parse, number_text) {
    std::string text{ "one foo." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "1 foo.");
}
TEST(parser_parse, text_number_text) {
    std::string text{ "foo one meh." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "foo 1 meh.");
}
TEST(parser_parse, text_number_text_number) {
    std::string text{ "foo one meh two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "foo 1 meh 2.");
}
TEST(parser_parse, text_number_text_number_text) {
    std::string text{ "foo one meh two blah." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "foo 1 meh 2 blah.");
}

TEST(parser_parse, number_number) {
    std::string text{ "." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "");
}
 
TEST(parser_parse, zero) {
    std::string text{ "zero." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "0.");
}
TEST(parser_parse, one) {
    std::string text{ "one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "1.");
}
TEST(parser_parse, nine) {
    std::string text{ "nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "9.");
}
TEST(parser_parse, ten) {
    std::string text{ "ten." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "10.");
}
TEST(parser_parse, nineteen) {
    std::string text{ "nineteen." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "19.");
}
TEST(parser_parse, twenty) {
    std::string text{ "twenty." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "20.");
}
TEST(parser_parse, twenty_one) {
    std::string text{ "twenty one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "21.");
}
TEST(parser_parse, twenty_dash_one) {
    std::string text{ "twenty-one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "21.");
}
TEST(parser_parse, twenty_nine) {
    std::string text{ "twenty nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "29.");
}
TEST(parser_parse, twenty_dash_nine) {
    std::string text{ "twenty-nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "29.");
}
TEST(parser_parse, ninety) {
    std::string text{ "ninety." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "90.");
}
TEST(parser_parse, ninety_one) {
    std::string text{ "ninety one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "91.");
}
TEST(parser_parse, ninety_dash_one) {
    std::string text{ "ninety-one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "91.");
}
TEST(parser_parse, ninety_nine) {
    std::string text{ "ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "99.");
}
TEST(parser_parse, ninety_dash_nine) {
    std::string text{ "ninety-nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "99.");
}
TEST(parser_parse, one_hundred) {
    std::string text{ "one hundred." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "100.");
}
TEST(parser_parse, one_hundred_and_one) {
    std::string text{ "one hundred and one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "101.");
}
TEST(parser_parse, one_hundred_and_ninety) {
    std::string text{ "one hundred and ninety." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "190.");
}
TEST(parser_parse, one_hundred_and_ninety_nine) {
    std::string text{ "one hundred and ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "199.");
}
TEST(parser_parse, one_hundred_and_ninety_dash_nine) {
    std::string text{ "one hundred and ninety-nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "199.");
}
TEST(parser_parse, nine_hundred) {
    std::string text{ "nine hundred." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "900.");
}
TEST(parser_parse, nine_hundred_and_one) {
    std::string text{ "nine hundred and one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "901.");
}
TEST(parser_parse, nine_hundred_and_ninety) {
    std::string text{ "nine hundred and ninety." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "990.");
}
TEST(parser_parse, nine_hundred_and_ninety_nine) {
    std::string text{ "nine hundred and ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "999.");
}
TEST(parser_parse, nine_hundred_and_ninety_dash_nine) {
    std::string text{ "nine hundred and ninety dash nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "999.");
}
TEST(parser_parse, one_thousand) {
    std::string text{ "one thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000));
}
TEST(parser_parse, nine_thousand) {
    std::string text{ "nine thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 9'000));
}
TEST(parser_parse, nine_hundred_thousand) {
    std::string text{ "nine hundred thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 900'000));
}
TEST(parser_parse, nine_hundred_and_ninety_nine_thousand) {
    std::string text{ "nine hundred and ninety nine thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 999'000));
}
TEST(parser_parse, nine_hundred_and_ninety_dash_nine_thousand) {
    std::string text{ "nine hundred and ninety-nine thousand." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 999'000));
}
TEST(parser_parse, one_million) {
    std::string text{ "one million." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000'000));
}
TEST(parser_parse, nine_million) {
    std::string text{ "nine million." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 9'000'000));
}
TEST(parser_parse, nine_hundred_million) {
    std::string text{ "nine hundred million." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 900'000'000));
}
TEST(parser_parse, nine_hundred_and_ninety_nine_million) {
    std::string text{ "nine hundred and ninety nine thousand million." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 999'000'000));
}
TEST(parser_parse, nine_hundred_and_ninety_dash_nine_million) {
    std::string text{ "nine hundred and ninety-nine thousand million." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 999'000'000));
}
TEST(parser_parse, nine_hundred_and_ninety_nine_million_nine_hundred_and_ninety_thousand_nine_hundred_and_ninety_nine) {
    std::string text{ "nine hundred and ninety nine million nine hundred and ninety nine thousand nine hundred and ninety nine." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 999'999'999));
}
TEST(parser_parse, one_billion) {
    std::string text{ "one billion." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000'000'000));
}

TEST(parser_parse, one_hundred_and_two) {
    std::string text{ "one hundred and two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "102.");
}
TEST(parser_parse, one_thousand_and_two) {
    std::string text{ "one thousand and two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'002));
}
TEST(parser_parse, one_million_and_two) {
    std::string text{ "one million and two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000'002));
}
TEST(parser_parse, one_hundred_and_twenty_three) {
    std::string text{ "one hundred and twenty three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "123.");
}
TEST(parser_parse, one_hundred_and_twenty_dash_three) {
    std::string text{ "one hundred and twenty-three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "123.");
}
TEST(parser_parse, one_thousand_and_twenty_three) {
    std::string text{ "one thousand and twenty three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'023));
}
TEST(parser_parse, one_thousand_and_twenty_dash_three) {
    std::string text{ "one thousand and twenty-three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'023));
}
TEST(parser_parse, one_million_and_twenty_three) {
    std::string text{ "one million and twenty three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000'023));
}
TEST(parser_parse, one_million_and_twenty_dash_three) {
    std::string text{ "one million and twenty-three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000'023));
}
TEST(parser_parse, one_thousand_two_hundred_and_three) {
    std::string text{ "one thousand two hundred and three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'203));
}
TEST(parser_parse, one_million_two_hundred_and_three) {
    std::string text{ "one million two hundred and three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000'203));
}
TEST(parser_parse, one_million_two_thousand_and_three) {
    std::string text{ "one million two thousand and three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'002'003));
}
TEST(parser_parse, one_million_two_thousand_three_hundred_and_four) {
    std::string text{ "one million two thousand three hundred and four." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'002'304));
}

TEST(parser_parse, three_million_six_hundred_and_three_thousand_eight_hundred_and_two) {
    std::string text{ "three million six hundred and three thousand eight hundred and two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 3'603'802));
}
TEST(parser_parse, seven_million_five_hundred_and_twelve_thousand_one_hundred_and_forty_five) {
    std::string text{ "seven million five hundred and twelve thousand one hundred and forty five." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 7'512'145));
}
TEST(parser_parse, three_million_eight_hundred_thousand_and_eighteen) {
    std::string text{ "three million eight hundred thousand and eighteen." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 3'800'018));
}

TEST(parser_parse, Zero) {
    std::string text{ "Zero." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "0.");
}
TEST(parser_parse, ZERO) {
    std::string text{ "ZERO." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "0.");
}
TEST(parser_parse, twenty_dash_One) {
    std::string text{ "twenty dash One." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "21.");
}
TEST(parser_parse, twenty_dash_ONE) {
    std::string text{ "twenty dash ONE." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "21.");
}
TEST(parser_parse, one_BILLION) {
    std::string text{ "one BILLION." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), fmt::format("{}.", 1'000'000'000));
}

TEST(parser_parse, one_and_two) {
    std::string text{ "one and two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "1 and 2.");
}
TEST(parser_parse, two_and_one) {
    std::string text{ "two and one." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "2 and 1.");
}
TEST(parser_parse, one_hundred_comma_and_two) {
    std::string text{ "one hundred, and two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "100, and 2.");
}
TEST(parser_parse, one_hundred_and_two_and_three) {
    std::string text{ "one hundred and two and three." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "102 and 3.");
}
TEST(parser_parse, one_hundred_and_three_and_two) {
    std::string text{ "one hundred and three and two." };
    EXPECT_EQ(std::make_unique<parser>(std::move(text))->parse(), "103 and 2.");
}
TEST(parser_parse, one_hundred_two_hundred) {
    std::string text{ "one hundred two hundred." };
    EXPECT_THROW((void) std::make_unique<parser>(std::move(text))->parse(), invalid_token_error);
}
