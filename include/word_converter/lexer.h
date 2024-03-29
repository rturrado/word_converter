#pragma once

#include "generator.hpp"
#include "input_reader.h"

#include <cstdint>  // int64_t
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <regex>
#include <rtc/string.h>
#include <ostream>
#include <unordered_map>
#include <string>
#include <string_view>


enum class lexeme_t {
    zero,
    one,
    two_to_nine,  // two, three, four, five, six, seven, eight, nine
    ten_to_nineteen,  // ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen, seventeen, eighteen, nineteen
    tens,  // twenty, thirty, forty, fifty, sixty, seventy, eighty, ninety
    hundred,  // a hundred
    thousand,  // a thousand
    million,  // a million
    billion,  // a billion
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
    { "hundred", lexeme_t::hundred },  // a hundred
    { "thousand", lexeme_t::thousand },  // a thousand
    { "million", lexeme_t::million },  // a million
    { "billion", lexeme_t::billion },  // a billion
    { "and", lexeme_t::and_connector }  // and
};


class tokenizer {
    input_reader_up reader_{};
private:
    [[nodiscard]] static std::generator<token_t> get_next_token(std::string sentence) {
        std::string_view space_pattern{ R"([ \t\r\n]+)" };
        std::string_view dash_pattern{ R"(\-)" };
        std::string_view period_pattern{ R"(\.)" };
        std::string_view word_pattern{ R"([a-zA-Z]+)" };
        static const std::regex pattern{ fmt::format("({})|({})|({})|({})",
            space_pattern,  // 1
            dash_pattern,  // 2
            period_pattern,  // 3
            word_pattern  // 4
        )};
        std::smatch sm{};
        while (std::regex_search(sentence, sm, pattern)) {
            const auto& prefix{ sm.prefix().str() };
            if (not prefix.empty()) {  // other
                token_t ret{ lexeme_t::other, prefix };
                co_yield ret;
            }
            if (sm[1].matched) {  // space
                token_t ret{ lexeme_t::space, sm[1].str() };
                co_yield ret;
            } else if (sm[2].matched) {  // dash
                token_t ret{ lexeme_t::dash, sm[2].str() };
                co_yield ret;
            } else if (sm[3].matched) {  // period
                token_t ret{ lexeme_t::period, sm[3].str() };
                co_yield ret;
            } else if (sm[4].matched) {  // word
                const auto &word{ sm[4].str() };
                auto word_lc{ rtc::string::to_lowercase(word) };
                if (word_to_lexeme_map.contains(word_lc)) {
                    token_t ret{ word_to_lexeme_map.at(word_lc), word };
                    co_yield ret;
                } else {
                    token_t ret{ lexeme_t::other, word };
                    co_yield ret;
                }
            }
            sentence = sm.suffix();
        }
        if (not sentence.empty()) {  // other
            token_t ret{ lexeme_t::other, sentence };
            co_yield ret;
        }
    }
public:
    explicit tokenizer(input_reader_up reader)
        : reader_{ std::move(reader) }
    {}
    [[nodiscard]] std::generator<token_t> operator()() {
        while (not reader_->eof()) {
            std::string sentence{ reader_->read() };
            for (auto&& token : get_next_token(std::move(sentence))) {
                co_yield token;
            }
        }
        token_t ret{ lexeme_t::end, {} };
        co_yield ret;
    }
};


class lexer {
    using generator_t = std::generator<token_t>;
    using generator_iter_t = decltype(std::declval<generator_t>().begin());
    using generator_sentinel_t =decltype(std::declval<generator_t>().end());
private:
    tokenizer tokenizer_;
    generator_t token_generator_;
    generator_iter_t current_token_it_;
    generator_sentinel_t end_token_it_;
    token_t current_token_;
public:
    explicit lexer(input_reader_up reader)
        : tokenizer_{ std::move(reader) }
        , token_generator_{ tokenizer_() }
        , current_token_it_{ token_generator_.begin() }
        , end_token_it_{ token_generator_.end() }
        , current_token_{ *current_token_it_ }
    {}
    void advance_to_next_token() {
        if (++current_token_it_ != end_token_it_) {
            current_token_ = *current_token_it_;
        }
    }
    [[nodiscard]] auto get_current_token() const {
        return current_token_;
    }
    [[nodiscard]] auto get_current_lexeme() const {
        return current_token_.lexeme;
    }
    [[nodiscard]] auto get_current_text() const {
        return current_token_.text;
    }
};
