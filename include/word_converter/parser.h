#pragma once

#include "lexer.h"

#include <fmt/core.h>
#include <memory>  // make_unique, unique_ptr
#include <numeric>  // accumulate
#include <ranges>
#include <stdexcept>  // runtime_error
#include <string>
#include <unordered_map>


inline static const std::unordered_map<std::string, int> word_to_number_map{
    { "zero", 0 },  // zero
    { "one", 1 },  // one
    { "two", 2 },  // two to nine
    { "three", 3 },
    { "four", 4 },
    { "five", 5 },
    { "six", 6 },
    { "seven", 7 },
    { "eight", 8 },
    { "nine", 9 },
    { "ten", 10 },  // ten to nineteen
    { "eleven", 11 },
    { "twelve", 12 },
    { "thirteen", 13 },
    { "fourteen", 14 },
    { "fifteen", 15 },
    { "sixteen", 16 },
    { "seventeen", 17 },
    { "eighteen", 18 },
    { "nineteen", 19 },
    { "twenty", 20 },  // tens
    { "thirty", 30 },
    { "forty", 40 },
    { "fifty", 50 },
    { "sixty", 60 },
    { "seventy", 70 },
    { "eighty", 80 },
    { "ninety", 90 },
    { "hundred", 100 },  // hundred
    { "thousand", 1'000 },  // thousand
    { "million", 1'000'000 },  // million
    { "billion", 1'000'000'000 }  // billion
};


struct invalid_token_error : public std::runtime_error {
    explicit invalid_token_error(const token_t& t) : std::runtime_error{ "" } {
        message_ += fmt::format("'{}'", t);
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "invalid token: " };
};

struct invalid_number_expression_error : public std::runtime_error {
    explicit invalid_number_expression_error(const std::string& message) : std::runtime_error{ "" } {
        message_ += fmt::format("'{}'", message);
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "invalid number expression error: " };
};


class number_expression_stack {
    std::vector<int> numbers_{};  // for a number expression
public:
    void clear() {
        numbers_.clear();
    }
    void push(int number) {
        if (numbers_.empty()) {
            numbers_.push_back(number);
        } else if (number > numbers_.back()) {
            int sum{ numbers_.back() };
            while (sum < number) {
                numbers_.pop_back();
                if (not numbers_.empty()) {
                    sum += numbers_.back();
                }
            }
            numbers_.push_back(number + sum);
        } else if (number < numbers_.back()) {
            numbers_.back() *= number;
        } else if (number == numbers_.back()) {
            throw invalid_number_expression_error{
                fmt::format("{} {}", std::to_string(number), std::to_string(numbers_.back()))
            };
        }
    }
    [[nodiscard]] int value() const {
        return std::accumulate(numbers_.begin(), numbers_.end(), 0);
    }
};


class parser {
    std::string input_text_{};  // text to parse
    std::string output_text_{};  // parsed text
    std::unique_ptr<lexer> lexer_{};
    std::string spaces_{};  // last token, in case it was whitespace
    number_expression_stack numbers_{};
private:
    void advance_to_next_token() {
        lexer_->advance_to_next_token();
        if (lexer_->get_current_lexeme() == lexeme_t::space) {
            spaces_ = lexer_->get_current_token().text;
            lexer_->advance_to_next_token();
        }
    }
    void flush_to_output_text() {
        output_text_ += spaces_;
        output_text_ += lexer_->get_current_token().text;
        spaces_.clear();
    }
    void flush_to_output_text(const std::string& number_str) {
        output_text_ += spaces_;
        output_text_ += number_str;
        output_text_ += lexer_->get_current_token().text;
        spaces_.clear();
    }
private:
    [[nodiscard]] bool and_connector() {
        if (lexer_->get_current_lexeme() == lexeme_t::and_connector) {
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool dash() {
        if (lexer_->get_current_lexeme() == lexeme_t::dash) {
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool period() {
        if (lexer_->get_current_lexeme() == lexeme_t::period) {
            flush_to_output_text();
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool other() {
        if (lexer_->get_current_lexeme() == lexeme_t::other) {
            flush_to_output_text();
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool zero() {
        if (lexer_->get_current_lexeme() == lexeme_t::zero) {
            auto zero_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(zero_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool one() {
        if (lexer_->get_current_lexeme() == lexeme_t::one) {
            auto one_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(one_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool two_to_nine() {
        if (lexer_->get_current_lexeme() == lexeme_t::two_to_nine) {
            auto one_to_nine_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(one_to_nine_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool one_to_nine() {
        return one() or two_to_nine();
    }
    [[nodiscard]] bool ten_to_nineteen() {
        if (lexer_->get_current_lexeme() == lexeme_t::ten_to_nineteen) {
            auto ten_to_nineteen_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(ten_to_nineteen_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool twenty_to_ninety_nine() {
        if (lexer_->get_current_lexeme() == lexeme_t::tens) {
            auto tens_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(tens_number);
            advance_to_next_token();
            if (dash()) {
                return one_to_nine();
            } else {
                (void) one_to_nine();
                return true;
            }
        }
        throw invalid_token_error{ lexer_->get_current_token() };
    }
    [[nodiscard]] bool ten_to_ninety_nine() {
        return ten_to_nineteen() or twenty_to_ninety_nine();
    }
    [[nodiscard]] bool one_to_ninety_nine() {
        return one_to_nine() or ten_to_ninety_nine();
    }
    [[nodiscard]] bool below_one_hundred() {
        return (lexer_->get_current_lexeme() == lexeme_t::end)
            or (and_connector() and one_to_ninety_nine());
    }
    [[nodiscard]] bool hundred() {
        if (lexer_->get_current_lexeme() == lexeme_t::hundred) {
            auto hundred_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(hundred_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool hundreds() {
        if (one_to_nine()) {
            if (hundred()) {
                return below_one_hundred();
            }
            return true;
        }
        return false;
    }
    [[nodiscard]] bool below_one_thousand() {
        return (lexer_->get_current_lexeme() == lexeme_t::end)
            or (and_connector() and one_to_ninety_nine())
            or (one_to_nine() and hundred() and and_connector() and one_to_ninety_nine());
    }
    [[nodiscard]] bool thousand() {
        if (lexer_->get_current_lexeme() == lexeme_t::thousand) {
            auto thousand_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(thousand_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool thousands() {
        if (hundreds()) {
            if (thousand()) {
                return below_one_thousand();
            }
            return true;
        }
        return false;
    }
    [[nodiscard]] bool below_one_million() {
        return (lexer_->get_current_lexeme() == lexeme_t::end)
            or (and_connector() and one_to_ninety_nine())
            or thousands();
    }
    [[nodiscard]] bool million() {
        if (lexer_->get_current_lexeme() == lexeme_t::million) {
            auto million_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(million_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool millions() {
        if (hundreds()) {
            if (million()) {
                return below_one_million();
            }
            return true;
        }
        return false;
    }
    [[nodiscard]] bool below_one_billion() {
        return (lexer_->get_current_lexeme() == lexeme_t::end)
            or (and_connector() and one_to_ninety_nine())
            or millions();
    }
    [[nodiscard]] bool billion() {
        if (lexer_->get_current_lexeme() == lexeme_t::billion) {
            auto billion_number{ word_to_number_map.at(lexer_->get_current_token().text) };
            numbers_.push(billion_number);
            advance_to_next_token();
            return true;
        }
        return false;
    }
    [[nodiscard]] bool billions() {
        if (hundreds()) {
            if (billion()) {
                return below_one_billion();
            }
            return true;
        }
        return false;
    }
    void number_expression() {
        auto start_source_location{ lexer_->get_source_location() };
        numbers_.clear();
        if (zero() or ten_to_nineteen() or billions()) {
            flush_to_output_text(std::to_string(numbers_.value()));
        } else {
            auto end_source_location{ lexer_->get_source_location() };
            auto source_sub_expression{ lexer_->get_source_text().substr(
                start_source_location, end_source_location - start_source_location ) };
            throw invalid_number_expression_error{ source_sub_expression };
        }
    }
    void text_without_number_expressions() {
        while (and_connector() or dash() or other()) {
        }
    }
    void sentence_body() {
        if (period()) {
            return;
        }
        number_expression();
        if (other()) {
            sentence_body();
        }
    }
    void sentence_prefix() {
        text_without_number_expressions();
    }
    void sentence() {
        sentence_prefix();
        sentence_body();
    }
    void start() {
        sentence();
    }
public:
    explicit parser(std::string text)
        : input_text_{ std::move(text) }
    {}

    [[nodiscard]] std::string parse() {
        lexer_ = std::make_unique<lexer>(std::move(input_text_));
        start();
        return output_text_;
    }
};
