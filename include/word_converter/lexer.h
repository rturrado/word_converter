#pragma once

#include "generator.hpp"

#include <algorithm>  // for_each
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <regex>
#include <rtc/string.h>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <string>


// Grammar
//
// start ::= sentence

// sentence ::= sentence_prefix sentence_body
// sentence_prefix ::= text_without_number_expressions
// sentence_body ::= number_expression rest_of_sentence_body
// rest_of_sentence_body ::= text_without_number_expression sentence_body
//     | period

// text_without_number_expressions ::= text_without_number_expression text_without_number_expressions
//    | nothing
// text_without_number_expression := other

// number_expression ::= billions | ten_to_ninety_nine | zero
//
// billions ::= hundreds rest_of_billions
// rest_of_billions ::= billion below_one_billion
//     | nothing
// billion ::= "billion"
// below_one_billion ::= millions
//    | and one_to_ninety_nine
//    | nothing
//
// millions ::= hundreds rest_of_millions
// rest_of_millions ::= million below_one_million
//     | nothing
// million ::= "million"
// below_one_million ::= thousands
//    | and one_to_ninety_nine
//    | nothing
//
// thousands ::= hundreds rest_of_thousands
// rest_of_thousands ::= thousand below_one_thousand
//     | nothing
// thousand ::= "thousand"
// below_one_thousand ::= digit hundred and one_to_ninety_nine
//     | and one_to_ninety_nine
//     | nothing
//
// hundreds ::= one_to_nine rest_of_hundreds
// rest_of_hundreds ::= hundred below_one_hundred
//     | nothing
// hundred ::= "hundred"
// below_one_hundred ::= and one_to_ninety_nine
//     | nothing
//
// one_to_ninety_nine ::= one_to_nine | ten_to_ninety_nine
// ten_to_ninety_nine ::= ten_to_nineteen | tens rest_of_tens
// rest_of_tens ::= dash one_to_nine
//     | one_to_nine
//     | nothing
// tens ::= "twenty" | "thirty" | "forty" | "fifty" | "sixty" | "seventy" | "eighty" | "ninety"
// ten_to_nineteen ::= "ten" | "eleven" | "twelve" | "thirteen" | "fourteen" | "fifteen" | "sixteen" | "seventeen" | "eighteen" | "nineteen"
// one_to_nine ::= one | two_to_nine
// two_to_nine ::= "two" | "three" | "four" | "five" | "six" | "seven" | "eight" | "nine"
// one ::= "one"
// zero ::= "zero"
//
// dash ::= '-'
// period ::= '.'
// other


enum class lexeme_t {
    zero,
    one,
    two_to_nine,  // two, three, four, five, six, seven, eight, nine
    ten_to_nineteen,  // ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen, seventeen, eighteen, nineteen
    tens,  // twenty, thirty, forty, fifty, sixty, seventy, eighty, ninety
    hundred,  // hundred
    thousand,  // thousand
    million,  // million
    billion,  // billion
    and_connector,  // and
    space,  // whitespace, tab, newline...
    dash,  // '-'
    period, // '.'
    other,  // anything else, whatever is not allowed in a word number expression
    end
};
inline std::ostream& operator<<(std::ostream& os, const lexeme_t& l) {
    switch (l) {
        case lexeme_t::zero: os << "zero"; break;
        case lexeme_t::one: os << "one"; break;
        case lexeme_t::two_to_nine: os << "two_to_nine"; break;
        case lexeme_t::ten_to_nineteen: os << "ten_to_nineteen"; break;
        case lexeme_t::tens: os << "tens"; break;
        case lexeme_t::hundred: os << "hundred"; break;
        case lexeme_t::thousand: os << "thousand"; break;
        case lexeme_t::million: os << "million"; break;
        case lexeme_t::billion: os << "billion"; break;
        case lexeme_t::and_connector: os << "and"; break;
        case lexeme_t::dash: os << "dash"; break;
        case lexeme_t::period: os << "period"; break;
        case lexeme_t::space: os << "space"; break;
        case lexeme_t::other: os << "other"; break;
        case lexeme_t::end: os << "end"; break;
    }
    return os;
}
template <>
struct fmt::formatter<lexeme_t> : fmt::ostream_formatter {};


struct token_t {
    lexeme_t lexeme{};
    std::string text{};
    token_t(lexeme_t l, std::string t) : lexeme{ l }, text{ std::move(t) } {}
    token_t(token_t& other) = default;
    token_t(token_t&&) = delete;
    ~token_t() = default;
};
inline std::ostream& operator<<(std::ostream& os, const token_t& t) {
    auto escape_escape_sequences = [](std::string str) {
        static const std::string escape_sequences{ "\n\r\t" };
        static const std::string substitutions{ "nrt" };
        std::string ret{};
        std::ranges::for_each(str, [&ret](char c) {
            if (auto pos{ escape_sequences.find(c) }; pos != std::string::npos) {
                ret += std::string{ '\\', substitutions[pos] };
            } else {
                ret += c;
            }
        });
        return ret;
    };
    return os << fmt::format("({}, '{}')", t.lexeme, escape_escape_sequences(t.text));
}
template <>
struct fmt::formatter<token_t> : fmt::ostream_formatter {};


inline static const std::unordered_map<std::string, lexeme_t> word_to_lexeme_map{
    { "zero", lexeme_t::zero },  // zero
    { "one", lexeme_t::one },  // one
    { "two", lexeme_t::two_to_nine },  // two to nine
    { "three", lexeme_t::two_to_nine },
    { "four", lexeme_t::two_to_nine },
    { "five", lexeme_t::two_to_nine },
    { "six", lexeme_t::two_to_nine },
    { "seven", lexeme_t::two_to_nine },
    { "eight", lexeme_t::two_to_nine },
    { "nine", lexeme_t::two_to_nine },
    { "ten", lexeme_t::ten_to_nineteen },  // ten to nineteen
    { "eleven", lexeme_t::ten_to_nineteen },
    { "twelve", lexeme_t::ten_to_nineteen },
    { "thirteen", lexeme_t::ten_to_nineteen },
    { "fourteen", lexeme_t::ten_to_nineteen },
    { "fifteen", lexeme_t::ten_to_nineteen },
    { "sixteen", lexeme_t::ten_to_nineteen },
    { "seventeen", lexeme_t::ten_to_nineteen },
    { "eighteen", lexeme_t::ten_to_nineteen },
    { "nineteen", lexeme_t::ten_to_nineteen },
    { "twenty", lexeme_t::tens },  // tens
    { "thirty", lexeme_t::tens },
    { "forty", lexeme_t::tens },
    { "fifty", lexeme_t::tens },
    { "sixty", lexeme_t::tens },
    { "seventy", lexeme_t::tens },
    { "eighty", lexeme_t::tens },
    { "ninety", lexeme_t::tens },
    { "hundred", lexeme_t::hundred },  // hundred
    { "thousand", lexeme_t::thousand },  // thousand
    { "million", lexeme_t::million },  // million
    { "billion", lexeme_t::billion },  // billion
    { "and", lexeme_t::and_connector }  // and
};


class lexer {
    std::string text_{};
public:
    explicit lexer(std::string text)
        : text_{ std::move(text) }
    {}
    std::generator<token_t> get_next_token() {
        std::string space_pattern{ R"([ \t\r\n]+)" };
        std::string dash_pattern{ R"(\-)" };
        std::string period_pattern{ R"(\.)" };
        std::string word_pattern{ R"([a-zA-Z]+)" };
        std::regex pattern{ fmt::format("({})|({})|({})|({})",
            space_pattern,  // 1
            dash_pattern,  // 2
            period_pattern,  // 3
            word_pattern  // 4
        )};
        std::smatch sm{};
        while (std::regex_search(text_, sm, pattern)) {
            const auto& prefix{ sm.prefix().str() };
            if (not prefix.empty()) {  // other
                co_yield token_t{lexeme_t::other, prefix };
            }
            if (sm[1].matched) {  // space
                co_yield token_t{ lexeme_t::space, sm[1].str() };
            } else if (sm[2].matched) {  // dash
                co_yield token_t{ lexeme_t::dash, sm[2].str() };
            } else if (sm[3].matched) {  // period
                co_yield token_t{ lexeme_t::period, sm[3].str() };
            } else if (sm[4].matched) {  // word
                const auto &word{ sm[4].str() };
                auto word_lc{ rtc::string::to_lowercase(word) };
                if (word_to_lexeme_map.contains(word_lc)) {
                    co_yield token_t{ word_to_lexeme_map.at(word_lc), word };
                } else {
                    co_yield token_t{ lexeme_t::other, word };
                }
            }
            text_ = sm.suffix();
        }
        if (not text_.empty()) {  // other
            co_yield token_t{ lexeme_t::other, text_ };
        }
        co_yield token_t{ lexeme_t::end, {} };
    }
};
