#include "parser.h"

#include <gtest/gtest.h>


// 'parse' will only receive sentences, i.e. texts ending in a period

TEST(parser_parse, empty_sentence) { EXPECT_EQ(parser::parse("."), "."); }
TEST(parser_parse, two_empty_sentences) { EXPECT_EQ(parser::parse(".."), ".."); }
TEST(parser_parse, empty_sentence_and_text) { EXPECT_EQ(parser::parse(". foo."), ". foo."); }
TEST(parser_parse, empty_sentence_and_number) { EXPECT_EQ(parser::parse(". one."), ". 1."); }
TEST(parser_parse, text_and_empty_sentence) { EXPECT_EQ(parser::parse("foo.."), "foo.."); }
TEST(parser_parse, number_and_empty_sentence) { EXPECT_EQ(parser::parse("one.."), "1.."); }

TEST(parser_parse, number) { EXPECT_EQ(parser::parse("one."), "1."); }
TEST(parser_parse, text_number) { EXPECT_EQ(parser::parse("foo one."), "foo 1."); }
TEST(parser_parse, number_text) { EXPECT_EQ(parser::parse("one foo."), "1 foo."); }
TEST(parser_parse, text_number_text) { EXPECT_EQ(parser::parse("foo one meh."), "foo 1 meh."); }
TEST(parser_parse, text_number_text_number) { EXPECT_EQ(parser::parse("foo one meh two."), "foo 1 meh 2."); }
TEST(parser_parse, text_number_text_number_text) { EXPECT_EQ(parser::parse("foo one meh two blah."), "foo 1 meh 2 blah."); }

TEST(parser_parse, number_number) { EXPECT_EQ(parser::parse("."), ""); }
 
TEST(parser_parse, zero) { EXPECT_EQ(parser::parse("zero."), "0."); }
TEST(parser_parse, one) { EXPECT_EQ(parser::parse("one."), "1."); }
TEST(parser_parse, nine) { EXPECT_EQ(parser::parse("nine."), "9."); }
TEST(parser_parse, ten) { EXPECT_EQ(parser::parse("ten."), "."); }
TEST(parser_parse, nineteen) { EXPECT_EQ(parser::parse("nineteen."), "."); }
TEST(parser_parse, twenty) { EXPECT_EQ(parser::parse("twenty."), "10."); }
TEST(parser_parse, twenty_one) { EXPECT_EQ(parser::parse("twenty one."), "21."); }
TEST(parser_parse, twenty_dash_one) { EXPECT_EQ(parser::parse("twenty-one."), "21."); }
TEST(parser_parse, twenty_nine) { EXPECT_EQ(parser::parse("twenty nine."), "29."); }
TEST(parser_parse, twenty_dash_nine) { EXPECT_EQ(parser::parse("twenty-nine."), "29."); }
TEST(parser_parse, ninety) { EXPECT_EQ(parser::parse("ninety."), "90."); }
TEST(parser_parse, ninety_one) { EXPECT_EQ(parser::parse("ninety one."), "91."); }
TEST(parser_parse, ninety_dash_one) { EXPECT_EQ(parser::parse("ninety-one."), "91."); }
TEST(parser_parse, ninety_nine) { EXPECT_EQ(parser::parse("ninety nine."), "99."); }
TEST(parser_parse, ninety_dash_nine) { EXPECT_EQ(parser::parse("ninety-nine."), "99."); }
TEST(parser_parse, one_hundred) { EXPECT_EQ(parser::parse("one hundred."), "100."); }
TEST(parser_parse, one_hundred_and_one) { EXPECT_EQ(parser::parse("one hundred and one."), "101."); }
TEST(parser_parse, one_hundred_and_ninety) { EXPECT_EQ(parser::parse("one hundred and ninety."), "190."); }
TEST(parser_parse, one_hundred_and_ninety_nine) { EXPECT_EQ(parser::parse("one hundred and ninety nine."), "199."); }
TEST(parser_parse, one_hundred_and_ninety_dash_nine) { EXPECT_EQ(parser::parse("one hundred and ninety dash nine."), "199."); }
TEST(parser_parse, nine_hundred) { EXPECT_EQ(parser::parse("nine hundred."), "900."); }
TEST(parser_parse, nine_hundred_and_one) { EXPECT_EQ(parser::parse("nine hundred and one."), "901."); }
TEST(parser_parse, nine_hundred_and_ninety) { EXPECT_EQ(parser::parse("nine hundred and ninety."), "990."); }
TEST(parser_parse, nine_hundred_and_ninety_nine) { EXPECT_EQ(parser::parse("nine hundred and ninety nine."), "999."); }
TEST(parser_parse, nine_hundred_and_ninety_dash_nine) { EXPECT_EQ(parser::parse("nine hundred and ninety dash nine."), "999."); }
TEST(parser_parse, one_thousand) { EXPECT_EQ(parser::parse("one thousand."), "1000."); }
TEST(parser_parse, nine_thousand) { EXPECT_EQ(parser::parse("nine thousand."), "9000."); }
TEST(parser_parse, nine_hundred_thousand) { EXPECT_EQ(parser::parse("nine hundred thousand."), "900000."); }
TEST(parser_parse, nine_hundred_and_ninety_nine_thousand) { EXPECT_EQ(parser::parse("nine hundred and ninety nine thousand."), "999000."); }
TEST(parser_parse, nine_hundred_and_ninety_dash_nine_thousand) {
    EXPECT_EQ(parser::parse("nine hundred and ninety dash nine thousand."), "999000.");
}
TEST(parser_parse, one_million) { EXPECT_EQ(parser::parse("one million."), "1000000."); }
TEST(parser_parse, nine_million) { EXPECT_EQ(parser::parse("nine million."), "9000000."); }
TEST(parser_parse, nine_hundred_million) { EXPECT_EQ(parser::parse("nine hundred million."), "900000000."); }
TEST(parser_parse, nine_hundred_and_ninety_nine_million) {
    EXPECT_EQ(parser::parse("nine hundred and ninety nine thousand million."), "999000000.");
}
TEST(parser_parse, nine_hundred_and_ninety_dash_nine_million) {
    EXPECT_EQ(parser::parse("nine hundred and ninety dash nine thousand million."), "999000000.");
}
TEST(parser_parse, nine_hundred_and_ninety_nine_million_nine_hundred_and_ninety_thousand_nine_hundred_and_ninety_nine) {
    EXPECT_EQ(parser::parse("nine hundred and ninety nine million nine hundred and ninety nine thousand nine hundred and ninety nine."),
        "999999000000."
    );
}
TEST(parser_parse, one_billion) { EXPECT_EQ(parser::parse("one billion."), "1000000000."); }

TEST(parser_parse, one_hundred_and_two) { EXPECT_EQ(parser::parse("one hundred and two."), "102."); }
TEST(parser_parse, one_thousand_and_two) { EXPECT_EQ(parser::parse("one thousand and two."), "1002."); }
TEST(parser_parse, one_million_and_two) { EXPECT_EQ(parser::parse("one million and two."), "1000002."); }
TEST(parser_parse, one_hundred_and_twenty_three) { EXPECT_EQ(parser::parse("one hundred and twenty three."), "123."); }
TEST(parser_parse, one_hundred_and_twenty_dash_three) { EXPECT_EQ(parser::parse("one hundred and twenty dash three."), "123."); }
TEST(parser_parse, one_thousand_and_twenty_three) { EXPECT_EQ(parser::parse("one thousand and twenty three."), "1023."); }
TEST(parser_parse, one_thousand_and_twenty_dash_three) { EXPECT_EQ(parser::parse("one thousand and twenty dash three."), "1023."); }
TEST(parser_parse, one_million_and_twenty_three) { EXPECT_EQ(parser::parse("one million and twenty three."), "1000023."); }
TEST(parser_parse, one_million_and_twenty_dash_three) { EXPECT_EQ(parser::parse("one million and twenty dash three."), "1000023."); }
TEST(parser_parse, one_thousand_two_hundred_and_three) { EXPECT_EQ(parser::parse("one thousand two hundred and three."), "1203."); }
TEST(parser_parse, one_million_two_hundred_and_three) { EXPECT_EQ(parser::parse("one million two hundred and three."), "1000203."); }
TEST(parser_parse, one_million_two_thousand_and_three) { EXPECT_EQ(parser::parse("one million two thousand and three."), "1002003."); }
TEST(parser_parse, one_million_two_thousand_three_hundred_and_four) {
    EXPECT_EQ(parser::parse("one million two thousand three hundred and four."), "1002304.");
}

TEST(parser_parse, three_million_six_hundred_and_three_thousand_eight_hundred_and_two) {
    EXPECT_EQ(parser::parse("three million six hundred and three thousand eight hundred and two."), "3603802.");
}
TEST(parser_parse, seven_million_five_hundred_and_twelve_thousand_one_hundred_and_forty_five) {
    EXPECT_EQ(parser::parse("seven million five hundred and twelve thousand one hundred and forty five."), "7512145.");
}
TEST(parser_parse, three_million_eight_hundred_thousand_and_eighteen) {
    EXPECT_EQ(parser::parse("three million eight hundred thousand and eighteen."), "3800018.");
}

TEST(parser_parse, Zero) { EXPECT_EQ(parser::parse("Zero."), "0."); }
TEST(parser_parse, ZERO) { EXPECT_EQ(parser::parse("ZERO."), "0."); }
TEST(parser_parse, twenty_dash_One) { EXPECT_EQ(parser::parse("twenty dash One."), "21."); }
TEST(parser_parse, twenty_dash_ONE) { EXPECT_EQ(parser::parse("twenty dash ONE."), "21."); }
TEST(parser_parse, one_BILLION) { EXPECT_EQ(parser::parse("one BILLION."), "1000000000."); }

TEST(parser_parse, one_and_two) { EXPECT_EQ(parser::parse("one and two."), "1 and 2."); }
TEST(parser_parse, two_and_one) { EXPECT_EQ(parser::parse("two and one."), "2 and 1."); }
TEST(parser_parse, one_hundred_comma_and_two) { EXPECT_EQ(parser::parse("one hundred, and two."), "100, and 2."); }
TEST(parser_parse, one_hundred_and_two_and_three) { EXPECT_EQ(parser::parse("one hundred and two and three."), "102 and 3."); }
TEST(parser_parse, one_hundred_and_three_and_two) { EXPECT_EQ(parser::parse("one hundred and three and two."), "103 and 2."); }
TEST(parser_parse, one_hundred_two_hundred) { EXPECT_THROW(parser::parse("one hundred two hundred."), invalid_token_error); }
